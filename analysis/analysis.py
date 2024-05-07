import os
import sys
import numpy as np
import glob
import argparse

RDMA_MTU = 1024
BYTES_TO_KBYTES = 1024

def save_full_info(v:list, filename: str):
    if len(v) == 0: # if empty list, then do nothing
        print("save_full_info --- got empty list")
        return
    
    with open(filename, 'w') as f:
        for val in v:
            if len(val) == 3:
                var = str(val[0]) + " " + str(val[1]) + " " + str(val[2]) + "\n"
            elif len(val) == 4:
                var = str(val[0]) + " " + str(val[1]) + " " + str(val[2]) + " " + str(val[3]) + "\n"
            elif len(val) == 5:
                var = str(val[0]) + " " + str(val[1]) + " " + str(val[2]) + " " + str(val[3]) + " " + str(val[4]) + "\n"
            else:
                print(len(val))
                exit(1)
            f.write(var)
    print("[INFO - save full info] Save to {filename}".format(filename=filename))

def save_with_cdf(v: list, filename: str):
    if len(v) == 0: # if empty list, then do nothing
        return
    
    """ 
    calculate cdf
    """
    v_sorted = np.sort(v)
    p = 1. * np.arange(len(v)) / (len(v) - 1)

    od = []
    bkt = [0,0,0,0]
    n_accum = 0
    for i in range(len(v_sorted)):
        key = v_sorted[i]/1000.0  # nanoseconds -> microseconds
        n_accum += 1
        if bkt[0] == key:
            bkt[1] += 1
            bkt[2] = n_accum
            bkt[3] = p[i]
        else:
            od.append(bkt)
            bkt = [0,0,0,0]
            bkt[0] = key
            bkt[1] = 1
            bkt[2] = n_accum
            bkt[3] = p[i]
    if od[-1][0] != bkt[0]:
        od.append(bkt)
    od.pop(0)

    with open(filename, 'w') as f:
        for bkt in od:
            var = str(bkt[0]) + " " + str(bkt[1]) + " " + str(bkt[2]) + " " + str(bkt[3]) + "\n"
            f.write(var)
    print("[INFO - save with cdf] Save to {filename}".format(filename=filename))



def print_arr_summary(fct_arr: list, tag: str, record=False):
    if len(fct_arr) == 0: # if empty list, then do nothing
        return
    
    prnt = "Avg:\t{avg:.2f}us\np50:\t{p50:.2f}us\np95:\t{p95:.2f}us\np99:\t{p99:.2f}us\np99.9:\t{p999:.2f}us\np99.99:\t{p9999:.2f}us\nstdev:\t{stdev:.2f}\n".format(
        avg=np.average(fct_arr)/1000.0,
        p50=np.percentile(fct_arr, 50)/1000.0,
        p95=np.percentile(fct_arr, 95)/1000.0,
        p99=np.percentile(fct_arr, 99)/1000.0,
        p999=np.percentile(fct_arr, 99.9)/1000.0,
        p9999=np.percentile(fct_arr, 99.99)/1000.0,
        stdev=np.std(fct_arr)/1000.0)
    print(tag + prnt)
    
    
    if record != False:
        record.write(tag + prnt)




def process_output(name_outfile: str, thres_short_msg: float, main_runtime: int):
    dir_path = os.path.dirname(os.path.realpath(__file__))
    print("Current directory: {}".format(dir_path))

    # save into .dat files
    outfile_summary = dir_path + "/output/{}_summary.dat".format(name_outfile) # summary
    outfile_fct_all = dir_path + "/output/{}_fct_all.dat".format(name_outfile) # all
    outfile_fct_short = dir_path + "/output/{}_fct_short_{}KB.dat".format(name_outfile, int(thres_short_msg/BYTES_TO_KBYTES)) # short
    outfile_fct_long = dir_path + "/output/{}_fct_long_{}KB.dat".format(name_outfile, int(thres_short_msg/BYTES_TO_KBYTES)) # long
    outfile_full_info = dir_path + "/output/{}_full_info.dat".format(name_outfile) # full info
    outfile_rate = dir_path + "/output/{}_rate.dat".format(name_outfile) # rate
    outfile_failed_msgs = dir_path + "/output/{}_failed_msgs.dat".format(name_outfile) # queue-failed msgs
    
    f_summary = open(outfile_summary, "w")
    
    fct_dict = {
        # key: (src_ip, dst_ip) -> val: list( (msg_size, opcode, FCT) )
    }

    goodput_dict = {
        # key: (src_ip, dst_ip) -> val: rate
    }
    
    throughput_dict = {
        # key: (src_ip, dst_ip) -> val: rate
    }
    
    outdate_fct_dict = {
        # key: (src_ip, dst_ip) -> val: list( (msg_size, opcode, FCT, timestamp) )
    }

    failed_msg_dict = {
        # key: (src_ip, dst_ip) -> val: list( (msg_size, port, elapsed_time) )
    }
    
    total_msg_dict = {
        # key: src_ip -> val: rate
    }
    
    fct_all_str = ""
    
    for abs_filename in glob.iglob(dir_path + '/../log/*.log', recursive=True):
        filename = abs_filename.split("/")[-1]
        parsed_filename = filename.replace(".log", "").split("_")
        output_type = parsed_filename[0]
        src_ip = parsed_filename[1]
        if len(parsed_filename) > 2:
            dst_ip = parsed_filename[2]
        if len(parsed_filename) > 3:
            port_conn = parsed_filename[3]
        
        if output_type == "msggen":
            # msg_size, server_ip, port, elapsed_time 
            with open(abs_filename, "r") as f:
                for line in f.readlines():
                    parsed_line = line.split(" ")
                    if len(parsed_line) == 4:
                        msg_size = int(parsed_line[0])
                        dst_ip = parsed_line[1]
                        port_conn = parsed_line[2]
                        elapsed_time = parsed_line[3]
                        
                        key = (src_ip, dst_ip) # key 
                        if key not in failed_msg_dict.keys():
                            failed_msg_dict[key] = list()
                        failed_msg_dict[key].append((msg_size, port_conn, elapsed_time))
                    elif len(parsed_line) == 1:
                        total_msg_dict[src_ip] = int(parsed_line[0])
        
        elif output_type == "fct":
            key = (src_ip, dst_ip) # key (src_ip, dst_ip)
            if key not in fct_dict.keys():
                fct_dict[key] = list()
                goodput_dict[key] = 0.0
                throughput_dict[key] = 0.0
                outdate_fct_dict[key] = list()
                
            # open file
            with open(abs_filename, "r") as f:
                total_tx_bytes = 0
                total_send_time = 0
                total_normed_tx_bytes = 0
                for line in f.readlines():
                    parsed_line = line.split(" ")
                    # FCT data
                    if len(parsed_line) == 5: # data
                        msg_size = int(parsed_line[0])
                        msg_opcode = int(parsed_line[1])
                        msg_fct_abs = int(parsed_line[3])
                        msg_start_time = int(parsed_line[4])
                        
                        if main_runtime < msg_start_time:
                            # print(parsed_line, " -> Time passed, so do not consider this msg")
                            outdate_fct_dict[key].append((msg_size, msg_opcode, msg_fct_abs, msg_start_time)) # append (size, opcode, fct, timestamp)
                            continue
                        
                        # throughput
                        if msg_size < RDMA_MTU:
                            normalized_msg_size = msg_size + 60
                        else:
                            last_msg_size = 0
                            if msg_size - int(msg_size / RDMA_MTU) * RDMA_MTU > 0:
                                last_msg_size = msg_size - int(msg_size / RDMA_MTU) * RDMA_MTU + 60
                            normalized_msg_size = int(msg_size / RDMA_MTU) * 1084 + last_msg_size
                        total_normed_tx_bytes += normalized_msg_size
                        
                        # fct
                        fct_dict[key].append((msg_size, msg_opcode, msg_fct_abs, msg_start_time)) # append (size, opcode, fct, timestamp)
                        fct_all_str += "{} {} {} {}\n".format(msg_size, msg_opcode, msg_fct_abs, msg_start_time)
                    elif len(parsed_line) == 1: # tx bytes
                        # goodput
                        total_tx_bytes = float(parsed_line[0])
                    elif len(parsed_line) == 2: # tx time
                        # goodput
                        total_send_time = float(parsed_line[1]) - float(parsed_line[0])
                
                goodput_per_port = float(total_tx_bytes * 8 / total_send_time)
                goodput_dict[key] += goodput_per_port
                throughput_per_port = float(total_normed_tx_bytes * 8 / total_send_time)
                throughput_dict[key] += throughput_per_port
                print("Filename: {filename}\t->\tAvg.goodput:{goodput:.3f} Gbps, Avg.throughput:{throughput:.3f} Gbps".format(
                    filename=filename, goodput=goodput_per_port, throughput=throughput_per_port))
    
    
    total_fct_arr_abs = []
    total_fct_arr_abs_short = []
    total_fct_arr_abs_long = []
    total_rate_info = []
    total_failed_msgs = []
    
    print("\n\n")
    for k, v in fct_dict.items():
        print("{k} -> number of flows: {num}".format(k=k, num=len(v)))
        fct_arr_abs = [val[2] for val in v]
        fct_arr_abs_short = [i for i in [val[2] if val[0] <= thres_short_msg else 0 for val in v] if i != 0]
        fct_arr_abs_long = [i for i in [val[2] if val[0] > thres_short_msg else 0 for val in v] if i != 0]
        
        print_arr_summary(fct_arr_abs, "--> absolute flow completion time (total)\n")
        print_arr_summary(fct_arr_abs_short, "--> absolute flow completion time (short)\n")
        print_arr_summary(fct_arr_abs_long, "--> absolute flow completion time (long)\n")
        
        total_fct_arr_abs += fct_arr_abs
        total_fct_arr_abs_short += fct_arr_abs_short
        total_fct_arr_abs_long += fct_arr_abs_long

    print("\n" + "=" * 20 + "\n")
    print("TOTAL FCT RESULT")
    print_arr_summary(total_fct_arr_abs, "--> absolute flow completion time (total)\n", record=f_summary)
    print_arr_summary(total_fct_arr_abs_short, "--> absolute flow completion time (short)\n", record=f_summary)
    print_arr_summary(total_fct_arr_abs_long, "--> absolute flow completion time (long)\n", record=f_summary)
        
    print("\n" + "=" * 20 + "\n")
    print("GOODPUT FROM SRC_IP to DST_IP")
    for k, v in goodput_dict.items():
        rate_prnt = "TOTAL: Src:{src} -> Dst:{dst}\t\tAvg.rate:{rate:.2f} Gbps (goodput)\n".format(src=k[0], dst=k[1], rate=v)
        print(rate_prnt)
        f_summary.write(rate_prnt)
        total_rate_info.append((k[0], k[1], v))
    
    print("")
    print("THROUGHPUT FROM SRC_IP to DST_IP")
    for k, v in throughput_dict.items():
        rate_prnt = "THROUGHPUT: Src:{src} -> Dst:{dst}\t\tAvg.rate:{rate:.2f} Gbps (throughput)\n".format(src=k[0], dst=k[1], rate=v)
        print(rate_prnt)
        f_summary.write(rate_prnt)
        total_rate_info.append((k[0], k[1], v))
    
    print("")
    print("FAILED RATE FROM SRC_IP to DST_IP")
    for k, v in failed_msg_dict.items():
        failed_tx_bytes = 0
        failed_normed_tx_bytes = 0
        for val in v:
            msg_size = val[0]
            failed_tx_bytes += msg_size
            total_failed_msgs.append((k[0], k[1], msg_size, val[1], val[2])) # src_ip, dst_ip, msg_size, port_conn, elapsed_time
            failed_normed_tx_bytes
            if msg_size < RDMA_MTU:
                normalized_msg_size = msg_size + 60
            else:
                last_msg_size = 0
                if msg_size - int(msg_size / RDMA_MTU) * RDMA_MTU > 0:
                    last_msg_size = msg_size - int(msg_size / RDMA_MTU) * RDMA_MTU + 60
                normalized_msg_size = int(msg_size / RDMA_MTU) * 1084 + last_msg_size
            failed_normed_tx_bytes += normalized_msg_size
            
        failed_goodput = failed_tx_bytes * 8 / main_runtime # (float(v[-1][2]) - float(v[0][2]))
        failed_throughput = failed_normed_tx_bytes * 8 / main_runtime # (float(v[-1][2]) - float(v[0][2]))
        failed_prnt = "FAILED: Src:{src} -> Dst:{dst}\t\t#.failed msgs:{nMsg}, failed goodput:{goodput:.3f} Gbps, failed throughput:{throughput:.3f} Gbps\n".format(src=k[0], dst=k[1], nMsg=len(v), goodput=failed_goodput, throughput=failed_throughput)
        print(failed_prnt)
        f_summary.write(failed_prnt)
    
    print("")
    print("AVERAGE MSG_GEN SPEED")
    for k, v in total_msg_dict.items():
        msggen_rate = "MSGGEN: Src:{src}\t\tAvg.Speed:{speed:.3f} Gbps (goodput)\n".format(src=k, speed=v * 8.0 / main_runtime)
        print(msggen_rate)
        f_summary.write(msggen_rate)

    print("")
    save_with_cdf(total_fct_arr_abs, outfile_fct_all)
    save_with_cdf(total_fct_arr_abs_short, outfile_fct_short)
    save_with_cdf(total_fct_arr_abs_long, outfile_fct_long)
    save_full_info(total_rate_info, outfile_rate)
    save_full_info(total_failed_msgs, outfile_failed_msgs)
    print("[INFO] save to {filename}".format(filename=outfile_summary))
    with open(outfile_full_info, "w") as f:
        f.write(fct_all_str)
    f_summary.close()
    
def main():
    # make directory if not exists
    isExist = os.path.exists(os.getcwd() + "/output/")
    if not isExist:
        os.makedirs(os.getcwd() + "/output/")
        print("The new directory is created - {}".format(os.getcwd() + "/output/"))

    parser = argparse.ArgumentParser(prog='FCT analysis',
                                     description='Calculating FCTs of RDMA.')
    parser.add_argument('-o', required=True, help='[Required] name of outfile (one word, e.g., ecmp_ali_L10)')
    parser.add_argument('-d', required=True, help='[Required] time of main simulation runtime (e.g., 100 seconds)')
    parser.add_argument('-s', required=False, default=32, help='size of message to divide short or long, in KBytes (default: 32 KBytes)')
    args = parser.parse_args()

    name_outfile = str(args.o)
    main_runtime = int(args.d) * 1e9
    thres_short_msg = float(int(args.s) * 1024) # bytes
    process_output(name_outfile, thres_short_msg, main_runtime)

if __name__ == '__main__':
    main()
