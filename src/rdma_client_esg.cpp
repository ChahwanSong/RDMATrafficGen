#include "../utils/customrand.h"
#include "../utils/readerwriterqueue.h"
#include "common.hpp"

using namespace congweave;

#define MSG_FINISH_WARMUP (UINT32_MAX - 1)
#define MSG_FINISH_MAIN (UINT32_MAX)
#define CLIENT_FILENAME ("/src/rdma_client_esg.cpp")

void usage() {
    printf("---Usage---\n");
    printf("rdma_client_esg: \n[-a <client ip of this node>] \n[-c <config file directory (absolute)>] \n[-w <workload file directory (absolute)>] \n[-l <load (0.0 ~ 1.0)>]\n");
    printf("Default config: `pwd`/config/config.txt\n");
    printf("Default workload: `pwd`/utils/workloads/GoogleRPC08.txt\n");
    printf("Default load: 0.00001\n");
}

inline void encode_msg(uint64_t &_encoded_msg, const uint32_t &_size, const uint32_t &_conn_idx) {
    _encoded_msg = ((uint64_t)_size << 32) + (uint64_t)_conn_idx;
}
inline void decode_msg(const uint64_t &_encoded_msg, uint32_t &_size, uint32_t &_conn_idx) {
    _size = (uint32_t)(_encoded_msg >> 32);
    _conn_idx = (uint32_t)(_encoded_msg & 0x00000000FFFFFFFF);
}

int main(int argc, char **argv) {
    // Taken from answer to "How can I catch a ctrl-c event? (C++)"
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sig_to_exception;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

/* ignore warning message by strncpy() */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
    int option;
    char *node_ip = NULL, *config_dir = NULL, *workload_dir = NULL, *avg_load = NULL;
    char cwd[100];
    while ((option = getopt(argc, argv, "a:c:w:l:")) != -1) {
        switch (option) {
            case 'a':
                printf("[INFO] Passed node ip is : %s \n", optarg);
                node_ip = (char *)calloc(strlen(optarg), 1);
                strncpy(node_ip, optarg, strlen(optarg));
                break;
            case 'c':
                printf("[INFO] Passed config file is : %s \n", optarg);
                config_dir = (char *)calloc(strlen(optarg), 1);
                strncpy(config_dir, optarg, strlen(optarg));
                break;
            case 'w':
                printf("[INFO] Passed workload file is : %s \n", optarg);
                workload_dir = (char *)calloc(strlen(optarg), 1);
                strncpy(workload_dir, optarg, strlen(optarg));
                break;
            case 'l':
                printf("[INFO] Passed avg_load is : %s \n", optarg);
                avg_load = (char *)calloc(strlen(optarg), 1);
                strncpy(avg_load, optarg, strlen(optarg));
                break;
            default:
                usage();
                return 0;
        }
    }
#pragma GCC diagnostic pop

    /* sanity check */
    if (!node_ip) {
        printf("ERROR - Specify client ip of this node.\n");
        usage();
        exit(1);
    }

    /* if no input, set default */
    if (!config_dir) {
        /* get filepath */
        std::string filepath(__FILE__);
        replace(filepath, CLIENT_FILENAME, "/config/config");
        config_dir = str_copy(filepath.c_str());
        printf("* Use default config directory - %s\n", config_dir);
    }

    /* if no input, set default */
    if (!workload_dir) {
        /* get filepath */
        std::string filepath(__FILE__);
        replace(filepath, CLIENT_FILENAME, "/utils/workloads/GoogleRPC08.txt");
        workload_dir = str_copy(filepath.c_str());
        printf("* Use default workload - %s\n", workload_dir);
    }

    /* if no input, set default */
    if (!avg_load) {
        /* get load value */
        avg_load = str_copy("0.00001");
        printf("* Use default avg_load - %s\n\n", avg_load);
    }

    /* Read the configuration file */
    printf("--- Start reading config (E2E #max.conn:%d)\n", NUM_CONNECTIONS);
    uint32_t num_lines;
    std::ifstream conf;

    std::vector<std::pair<std::string, uint32_t> > server_port_pairs;      // exactly as config file (x1)
    std::vector<std::pair<std::string, uint32_t> > server_port_all_pairs;  // all persistent connections (xNUM_CONNECTIONS)
    std::set<uint32_t> port_duplicate_check;                               // for sanity check

    if (argc > 1) {
        conf.open(config_dir);
        conf >> num_lines;
        uint32_t curr_line = 0;
        while (!conf.eof() && curr_line < num_lines) {
            curr_line++;
            std::string client_ip, server_ip;
            uint32_t server_port;
            conf >> client_ip >> server_ip >> server_port;
            if (client_ip == std::string(node_ip)) {
                server_port_pairs.push_back(std::make_pair(server_ip, server_port));
                printf("client_ip: %s, server_ip: %s, port: ", client_ip.c_str(), server_ip.c_str());
                for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                    if (port_duplicate_check.find(server_port + j) != port_duplicate_check.end()) {  // sanity check
                        printf("ERROR - port is assigned multiple times\n");
                        exit(1);
                    }
                    port_duplicate_check.insert(server_port + j);
                    server_port_all_pairs.push_back(std::make_pair(server_ip, server_port + j));
                    printf("%u, ", server_port + j);
                }
                printf("\n");
            }
            fflush(stdout);
        }
        conf.close();
    } else {
        std::cerr << "Error: require a config file\n";
        fflush(stdout);
        return 1;
    }

    /***********************************************
     ***            R D M A   C L I E N T        ***
     ***********************************************/
    uint32_t n_servers = server_port_pairs.size();
    uint32_t n_connections = server_port_all_pairs.size();
    printf("-----------------------------------------------\n");
    printf("Finished reading config.\n");
    printf("Number of servers is : %u, connections is : %u\n", n_servers, n_connections);
    printf("-----------------------------------------------\n\n");
    if (n_connections == 0) {
        printf("Nothing to run, so quit\n");
        exit(0);
    }
    sleep(5);
    moodycamel::BlockingReaderWriterQueue<uint64_t> msg_queues[n_servers]; /* Single-Producer Single-Consumer Queue */

    std::thread conn_threads[n_servers];
    /* run persistent connection to each client */
    for (uint32_t i = 0; i < n_servers; i++) {
        /***********************************************
         ***      M U L T I - T H R E A D I N G      ***
         ************************************************/

        conn_threads[i] = std::thread([&](int i) {
            rdma_info("Start client [%u] to server %s \n", i, server_port_pairs[i].first.c_str());
            ClientRDMA r_client[NUM_CONNECTIONS];
            int ret;
            uint32_t n_run = -1, msg_size = 0, conn_idx = 0, n_epoch = 0, finish_warmup = 0, finish_main = 0, start_main = 0;
            FILE *client_output = NULL;
            char *client_output_file;
            struct timespec ts_global_start, ts_local_start, ts_local_finish;
            uint64_t nanosec_global_start = 0, nanosec_local_start = 0, nanosec_local_finish = 0;
            uint64_t encoded_msg;
            clock_gettime(CLOCK_REALTIME, &ts_global_start);
            nanosec_global_start = timespec_to_ns(ts_global_start);
            while (true) {
                if (n_run != -1) {
                    usleep(1000000); /* 1 sec timeout retry to make new connection */

                    /** DEBUGGIN - RUN ONLY ONCE **/
                    printf("[Client %u[%s)] Failed to connect, so return\n", i, node_ip);
                    return;
                }

                /* start making connection */
                printf("[Client %u(%s)] Run %u -> server ip: %s with %u connections -*-*-*\n",
                       i, node_ip, ++n_run, server_port_pairs[i].first.c_str(), NUM_CONNECTIONS);
                for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                    ret = r_client[j].start(server_port_all_pairs[i * NUM_CONNECTIONS + j].first.c_str(),
                                            std::to_string(server_port_all_pairs[i * NUM_CONNECTIONS + j].second).c_str());
                    if (!ret) {
                        printf("[Client %u(%s) Failed to start -> server ip: %s, connection index: %u\n", i, node_ip, server_port_pairs[i].first.c_str(), j);
                        continue;
                    }
                    ret = r_client[j].send_client_metadata();
                    if (!ret) {
                        ret = r_client[j].disconnect_and_cleanup();
                        printf("[Client %u(%s) Failed to exchange metadata -> server ip: %s, connection index: %u\n", i, node_ip, server_port_pairs[i].first.c_str(), j);
                        continue;
                    }
                }

                clock_gettime(CLOCK_REALTIME, &ts_local_start);
                nanosec_local_start = timespec_to_ns(ts_local_start);
                while (true) {
                    /* time checking to decide when to finish */
                    msg_queues[i].wait_dequeue(encoded_msg);
                    decode_msg(encoded_msg, msg_size, conn_idx);
                    debug("[Client %u(%s)] --- received msg_size: %u to conn_idx: %u\n",
                          i, node_ip, msg_size, conn_idx);
                    clock_gettime(CLOCK_REALTIME, &ts_local_finish);
                    nanosec_local_finish = timespec_to_ns(ts_local_finish);

                    if (finish_warmup == 1 && start_main == 0) {
                        printf("[Client %u(%s)] ## Start main simulation, server ip: %s -*-*-*\n",
                               i, node_ip, server_port_pairs[i].first.c_str());
                        start_main = 1;
                        clock_gettime(CLOCK_REALTIME, &ts_local_start);
                        nanosec_local_start = timespec_to_ns(ts_local_start);
                        for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                            r_client[j].set_wall_clock(nanosec_local_finish);  // update wall clock, to calc relative elapsed time
                        }
                    }

                    if (finish_warmup == 0 && conn_idx == MSG_FINISH_WARMUP) {
                        /***********************************************
                         ***      W A R M - U P  F I N I S H E D     ***
                         ***********************************************/
                        finish_warmup = 1;
                        for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                            r_client[j].clear_fct_records();  // clean up fct records during warm-up
                        }
                        printf("[Client %u(%s)] ## Finished Warm-up simulation, server ip: %s -*-*-*\n",
                               i, node_ip, server_port_pairs[i].first.c_str());
                        continue;
                    }

                    if (finish_warmup == 1 && conn_idx == MSG_FINISH_MAIN) { /* got finish signal from msg-gen*/
                        /***********************************************
                         ***          M A I N   F I N I S H E D      ***
                         ***********************************************/
                        finish_main = 1;

                        for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                            std::string filepath(__FILE__);
                            replace(filepath, CLIENT_FILENAME, "/log/fct_clientIp_serverIp_portId_epochId.log");
                            replace(filepath, "clientIp", node_ip);
                            replace(filepath, "serverIp", server_port_all_pairs[i * NUM_CONNECTIONS + j].first.c_str());
                            replace(filepath, "portId", std::to_string(server_port_all_pairs[i * NUM_CONNECTIONS + j].second).c_str());
                            replace(filepath, "epochId", std::to_string(n_run * 1000000 + n_epoch).c_str());
                            client_output_file = str_copy(filepath.c_str());

                            client_output = fopen(client_output_file, "w");
                            uint64_t n_msg_finished = r_client[j].flush_fct_records(client_output);  // write records to file
                            fprintf(client_output, "%lu %lu\n",                                      /* record start/finish time */
                                    nanosec_local_start - nanosec_global_start,                      /* relative timestamps */
                                    nanosec_local_finish - nanosec_global_start);
                            fflush(client_output);
                            fclose(client_output);
                            printf("[Client %u(%s)] ## Finished Logging, filename: %s, total (recorded)msgs: %lu (cf. also see failed msgs)\n",
                                   i, node_ip, client_output_file, n_msg_finished);
                        }
                        n_epoch++;
                        clock_gettime(CLOCK_REALTIME, &ts_local_start);  // renew
                        nanosec_local_start = timespec_to_ns(ts_local_start);
                        break;
                    }
                    assert(finish_main == 0); /* sanity check */

                    /* SEND RDMA MESSAGE */
                    ret = r_client[conn_idx].remote_memory_ops(IBV_WR_RDMA_WRITE, msg_size);  // IBV_WR_RDMA_WRITE, IBV_WR_RDMA_READ
                    if (!ret) {
                        /* if one connection has error, finish all connections */
                        for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                            ret = r_client[j].disconnect_and_cleanup();
                        }
                        continue;
                    }
                }  // end of while

                for (uint32_t j = 0; j < NUM_CONNECTIONS; j++) {
                    ret = r_client[j].disconnect_and_cleanup();
                }

                if (finish_main == 1) {
                    break;
                }
            }  // end of while

            printf("[Client %u(%s)] ## Finishing Main, Run %u -> server ip: %s -*-*-*\n",
                   i, node_ip, n_run, server_port_pairs[i].first.c_str());
        },
                                      i);
    }

    printf("sleep 10 seconds to establish connections\n");
    sleep(10);  // take a time to make connection

    /***********************************************
     ***    T R A F F I C   G E N E R A T O R    ***
     ***********************************************/
    /* traffic generation based on possion distribution */
    std::thread msggen_thread = std::thread([&]() {
        /* define random generator */
        double load = std::stod(std::string(avg_load));
        std::random_device rd;   // uniformly-distributed integer random number generator (usage: std::mt19937 rng(rd()))
        std::mt19937 rng(rd());  // mt19937: Pseudo-random number generation
        CustomRand customRand;
        customRand.loadCdfGrid(workload_dir);
        double avg = customRand.getAvg();              // median~average flow size (given by HPCC)
        double lambda = BW_GBPS * load / (8.0 * avg);  // lambda for exponential function
        std::exponential_distribution<double> exp(lambda);
        printf("Average is : %f, EXP lambda is : %e \n", avg, lambda);

        /* define logging file */
        FILE *msggen_output = NULL;
        char *msggen_output_file;
        std::string filepath(__FILE__);
        replace(filepath, CLIENT_FILENAME, "/log/msggen_clientIp.log");
        replace(filepath, "clientIp", node_ip);
        msggen_output_file = str_copy(filepath.c_str());
        msggen_output = fopen(msggen_output_file, "w");

        uint32_t msg_size = 0, server_idx = 0, conn_idx = 0, num_failure = 0, finish_warmup = 0, finish_main = 0;
        struct timespec ts_global, ts_local;
        uint64_t nanosec_global = 0, nanosec_local = 0, nanosec_next_msg_gen = 0, nanosec_inter_msg_gap = 0;
        uint64_t encoded_msg = 0, total_msg_size = 0;

        /* msg-gen failure history */
        std::vector<std::vector<std::string> > failed_msg_history;

        clock_gettime(CLOCK_REALTIME, &ts_global);
        nanosec_global = timespec_to_ns(ts_global);
        printf("[MsgGen (%s)] Start warm-up...\n", node_ip);

        while (true) {
            clock_gettime(CLOCK_REALTIME, &ts_local);
            nanosec_local = timespec_to_ns(ts_local);

            /* bit wrap-around */
            uint64_t time_elapsed = (nanosec_local > nanosec_global) ? nanosec_local - nanosec_global
                                                                     : nanosec_local + (UINT64_MAX - nanosec_global);

            if (finish_warmup == 0 && time_elapsed > (TIME_TO_WARM_UP_MSG_GEN)*uint64_t(1000000000)) {
                /***********************************************
                 ***      W A R M - U P  F I N I S H E D     ***
                 ***********************************************/
                finish_warmup = 1;
                for (uint32_t k = 0; k < server_port_pairs.size(); k++) {
                    encode_msg(encoded_msg, 0, MSG_FINISH_WARMUP);
                    while (!msg_queues[k].try_enqueue(encoded_msg)) {
                    } /* send FIN message to workers */
                }
                printf("[MsgGen (%s)] Finished warm-up!\n", node_ip);

                /* clean (failed) msg_history */
                failed_msg_history.clear();
                total_msg_size = 0;
                printf("[MsgGen (%s)] Failed #.msgs during warm-up: %lu\n", node_ip, failed_msg_history.size());

                /***** SLEEP *****/
                printf("[MsgGen (%s)] Sleep %u seconds...\n", node_ip, uint32_t(TIME_TO_WARM_UP_RESTING));
                sleep(TIME_TO_WARM_UP_RESTING);  // take a rest for a while
                /*****************/

                clock_gettime(CLOCK_REALTIME, &ts_global);
                nanosec_global = timespec_to_ns(ts_global);
                printf("[MsgGen (%s)] Start main simulation!\n", node_ip);
                continue;
            }

            if (finish_warmup == 1 && time_elapsed > (TIME_TO_SIMULATE)*uint64_t(1000000000)) {
                /***********************************************
                 ***          M A I N   F I N I S H E D      ***
                 ***********************************************/
                finish_main = 1;
                for (auto vec : failed_msg_history) {
                    fprintf(msggen_output, "%s %s %s %s\n",
                            vec[0].c_str() /* msg size */,
                            vec[1].c_str() /* server_ip */,
                            vec[2].c_str() /* port */,
                            vec[3].c_str() /* elapsed time */);
                }
                fprintf(msggen_output, "%lu\n", total_msg_size);
                fflush(msggen_output);
                fclose(msggen_output);
                encode_msg(encoded_msg, 0, MSG_FINISH_MAIN);
                for (uint32_t k = 0; k < server_port_pairs.size(); k++) {
                    while (!msg_queues[k].try_enqueue(encoded_msg)) {
                    } /* send FIN message to workers */
                }
                printf("[MsgGen (%s)] Finished main simulation!\n", node_ip);
                break;
            }
            assert(finish_main == 0); /* sanity check */

            if (nanosec_next_msg_gen < nanosec_local) { /* it's time to generate message */
                msg_size = customRand.randgen();
                nanosec_inter_msg_gap = uint64_t(exp.operator()(rng));
                debug("[MsgGen (%s)]MsgSize: %u, NextArrival(us): %u\n",      // nanoseconds -> microseconds
                      node_ip, msg_size, nanosec_inter_msg_gap / 1000);       /* inter-arrival-time */
                nanosec_next_msg_gen = nanosec_local + nanosec_inter_msg_gap; /* next time to generate message */

                server_idx = rand() % server_port_pairs.size();  // where to send
                conn_idx = rand() % NUM_CONNECTIONS;             // which port to send
                debug("[MsgGen (%s)] MsgSize: %u -> Server %s Conn %u -> QSize: %lu\n",
                      node_ip /* client_ip */,
                      msg_size /* msg size */,
                      server_port_pairs[server_idx].first.c_str() /* server_ip */,
                      conn_idx /* conn idx of server_ip */,
                      msg_queues[server_idx].size_approx() /* Queue size */);

                /************ MESSAGE QUEUE FAILURE **************/
                total_msg_size += msg_size;
                encode_msg(encoded_msg, msg_size, conn_idx);
                if (!(msg_queues[server_idx].try_enqueue(encoded_msg))) { /* msg queue is full */
                    rdma_info("[%s/%s/%u] Failed to create message job, QSize: %lu, total failures: %u \n",
                              node_ip /* client_ip */,
                              server_port_all_pairs[server_idx * NUM_CONNECTIONS + conn_idx].first.c_str() /* server_ip */,
                              server_port_all_pairs[server_idx * NUM_CONNECTIONS + conn_idx].second /* port */,
                              msg_queues[server_idx].size_approx() /* queue size */,
                              num_failure++);
                    clock_gettime(CLOCK_REALTIME, &ts_local);
                    /* logging */
                    failed_msg_history.push_back({std::to_string(msg_size) /* msg size */,
                                                  server_port_all_pairs[server_idx * NUM_CONNECTIONS + conn_idx].first /* server_ip */,
                                                  std::to_string(server_port_all_pairs[server_idx * NUM_CONNECTIONS + conn_idx].second) /* port */,
                                                  std::to_string(timespec_to_ns(ts_local) - timespec_to_ns(ts_global)) /* elapsed time */});
                }
            }
        }                         // end of while
        assert(finish_main == 1); /* sanity check */
    });

    /* execute threads (#total_thread = n_servers + 1) */
    for (uint32_t i = 0; i < server_port_pairs.size(); i++) {
        conn_threads[i].join();
    }
    msggen_thread.join();

    return 0;
}