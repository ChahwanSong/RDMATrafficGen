#!/bin/bash

cecho(){  # source: https://stackoverflow.com/a/53463162/2886168
    RED="\033[0;31m"
    GREEN="\033[0;32m"
    YELLOW="\033[0;33m"
    # ... ADD MORE COLORS
    NC="\033[0m" # No Color

    printf "${!1}${2} ${NC}\n"
}

# Check if the script is being run on knowhere
hostname=`hostname`
if [[ "$hostname" != "knowhere" ]];then
    cecho "RED" "ERROR: This script needs to run on knowhere"
    exit 1
fi

WORKLOAD=$1
LOAD=$2
TIME=$3

# Check if expt name and number is passed as an argument
if [ $# -ne 3 ];then
    cecho "RED" "Usage: $0 <workload (e.g., Solar2022, GoogleRPC08, AliStorage2019, DCTCP2010, FbHdp2015, Uniform32KB)> <Load (e.g., 0.15, 0.3, etc)> <Time to run servers - (warmup + resting + main), in Seconds>"
    exit 1
fi


##### EXAMPLE CODES #####
# ./run_evaluation.sh GoogleRPC08 0.05 120




#################### NOTE #####################
# GoogleRPC08: l=0.01 -> 1.88 Gbps
# AliStorage2019: l=0.01 -> 0.32Gbps
# FbHdp2015: l=0.01 -> 0.35Gbps
# DCTCP2010: l=0.01 -> 0.25Gbps





cecho "GREEN" "---- INPUT WORKLOAD: ${WORKLOAD} ----"
cecho "GREEN" "---- INPUT LOAD: ${LOAD} ----"
cecho "GREEN" "---- INPUT SERVER RUNTIME: ${TIME} ----"
echo -e "\n\n"
sleep 3


# Clean All
cecho "YELLOW" "\n---------------------------- CLEAN ALL ----------------------------\n"
## lumos
cecho "GREEN" "[CleanUp] lumos"
ssh lumos "sudo killall -2 rdma_server"
ssh lumos "sudo killall -2 rdma_client"
ssh lumos "sudo killall -2 rdma_client_esg"
## caelus
cecho "GREEN" "[CleanUp] caelus"
ssh caelus "sudo killall -2 rdma_server"
ssh caelus "sudo killall -2 rdma_client"
ssh caelus "sudo killall -2 rdma_client_esg"
## patronus
cecho "GREEN" "[CleanUp] patronus"
ssh patronus "sudo killall -2 rdma_server"
ssh patronus "sudo killall -2 rdma_client"
ssh patronus "sudo killall -2 rdma_client_esg"
## p4campus-svr1
cecho "GREEN" "[CleanUp] p4campus-svr1"
ssh p4campus-svr1 "sudo killall -2 rdma_server"
ssh p4campus-svr1 "sudo killall -2 rdma_client"
ssh p4campus-svr1 "sudo killall -2 rdma_client_esg"
# ## hajime
# cecho "GREEN" "[CleanUp] hajime"
# ssh hajime "sudo killall -2 rdma_server_hajime"
# ssh hajime "sudo killall -2 rdma_client_esg_hajime"

echo -e "\n\n"
sleep 1

# Run RDMA Servers
cecho "YELLOW" "**********************************************"
cecho "YELLOW" "********     SET MTU=1024   ******************"
cecho "YELLOW" "**********************************************"
# RDMA-MTU configuration (1200 -> MTU=1024Byte)
MTU_SIZE=1200
cecho "YELLOW" "Set MTU size = ${MTU_SIZE} at all servers..."
## lumos
cecho "GREEN" "[MTU=${MTU_SIZE}] lumos"
ssh lumos "sudo ip netns exec cwh52a ifconfig ens2f0 mtu ${MTU_SIZE}"
ssh lumos "sudo ip netns exec cwh52b ifconfig ens2f1 mtu ${MTU_SIZE}"
ssh lumos "sudo ip netns exec cwh61a ifconfig ens3f0 mtu ${MTU_SIZE}"
ssh lumos "sudo ip netns exec cwh61b ifconfig ens3f1 mtu ${MTU_SIZE}"
## caelus
cecho "GREEN" "[MTU=${MTU_SIZE}] caelus"
ssh caelus "sudo ip netns exec cwh52a ifconfig ens1f0 mtu ${MTU_SIZE}"
ssh caelus "sudo ip netns exec cwh52b ifconfig ens1f1 mtu ${MTU_SIZE}"
ssh caelus "sudo ip netns exec cwh61a ifconfig enp177s0f0 mtu ${MTU_SIZE}"
ssh caelus "sudo ip netns exec cwh61b ifconfig enp177s0f1 mtu ${MTU_SIZE}"
## patronus
cecho "GREEN" "[MTU=${MTU_SIZE}] patronus"
ssh patronus "sudo ip netns exec cwh62a ifconfig ens2f0 mtu ${MTU_SIZE}"
ssh patronus "sudo ip netns exec cwh62b ifconfig ens2f1 mtu ${MTU_SIZE}"
ssh patronus "sudo ip netns exec cwh51a ifconfig ens3f0 mtu ${MTU_SIZE}"
ssh patronus "sudo ip netns exec cwh51b ifconfig ens3f1 mtu ${MTU_SIZE}"
## p4campus-svr1
cecho "GREEN" "[MTU=${MTU_SIZE}] p4campus-svr1"
ssh p4campus-svr1 "sudo ip netns exec cwh62a ifconfig ens114f0 mtu ${MTU_SIZE}"
ssh p4campus-svr1 "sudo ip netns exec cwh62b ifconfig ens114f1 mtu ${MTU_SIZE}"
ssh p4campus-svr1 "sudo ip netns exec cwh51a ifconfig ens36f0 mtu ${MTU_SIZE}"
ssh p4campus-svr1 "sudo ip netns exec cwh51b ifconfig ens36f1 mtu ${MTU_SIZE}"
# ## hajime
# cecho "GREEN" "[MTU=${MTU_SIZE}] hajime"
# ssh hajime "sudo ip netns exec cwh52a ifconfig enp6s0f0 mtu ${MTU_SIZE}"
# ssh hajime "sudo ip netns exec cwh52b ifconfig enp6s0f1 mtu ${MTU_SIZE}"

cecho "YELLOW" "Sleep 5 seconds..."
echo -e "\n\n"
sleep 5





# Rebuild RDMA Codes
cecho "YELLOW" "**********************************************"
cecho "YELLOW" "******** Rebuild TraffifGen ******************"
cecho "YELLOW" "**********************************************"
cecho "GREEN" "Rebuild CMake on lumos..."
ssh lumos "cd ~/workspace/slb_node/ourRDMA; ~/workspace/slb_node/ourRDMA/reset.sh"
# cecho "GREEN" "Rebuild CMake on hajime..."
# ssh hajime "cd ~/workspace/slb_node/ourRDMA; ~/workspace/slb_node/ourRDMA/reset.sh"

cecho "YELLOW" "Sleep 5 seconds..."
echo -e "\n\n"
sleep 5




# Run RDMA Servers
cecho "YELLOW" "**********************************************"
cecho "YELLOW" "********     Run Servers    ******************"
cecho "YELLOW" "**********************************************"
## lumos
cecho "GREEN" "[Server] lumos cwh52a"
ssh lumos "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.1" &
cecho "GREEN" "[Server] lumos cwh52b"
ssh lumos "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.2" &
cecho "GREEN" "[Server] lumos cwh61a"
ssh lumos "sudo ip netns exec cwh61a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.3" &
cecho "GREEN" "[Server] lumos cwh61b"
ssh lumos "sudo ip netns exec cwh61b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.4" &
## caelus
cecho "GREEN" "[Server] caelus cwh52a"
ssh caelus "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.5" &
cecho "GREEN" "[Server] caelus cwh52b"
ssh caelus "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.6" &
cecho "GREEN" "[Server] caelus cwh61a"
ssh caelus "sudo ip netns exec cwh61a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.7" &
cecho "GREEN" "[Server] caelus cwh61b"
ssh caelus "sudo ip netns exec cwh61b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.8" &
## patronus
cecho "GREEN" "[Server] patronus cwh62a"
ssh patronus "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.9" &
cecho "GREEN" "[Server] patronus cwh62b"
ssh patronus "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.10" &
cecho "GREEN" "[Server] patronus cwh51a"
ssh patronus "sudo ip netns exec cwh51a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.11" &
cecho "GREEN" "[Server] patronus cwh51b"
ssh patronus "sudo ip netns exec cwh51b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.12" &
## p4campus-svr1
cecho "GREEN" "[Server] p4campus-svr1 cwh62a"
ssh p4campus-svr1 "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.13" &
cecho "GREEN" "[Server] p4campus-svr1 cwh62b"
ssh p4campus-svr1 "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.14" &
cecho "GREEN" "[Server] p4campus-svr1 cwh51a"
ssh p4campus-svr1 "sudo ip netns exec cwh51a ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.15" &
cecho "GREEN" "[Server] p4campus-svr1 cwh51b"
ssh p4campus-svr1 "sudo ip netns exec cwh51b ~/workspace/slb_node/ourRDMA/bin/rdma_server -a 10.4.4.16" &
# ## hajime
# cecho "GREEN" "[Server] hajime cwh52a"
# ssh hajime "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_server_hajime -a 10.4.4.5" &
# cecho "GREEN" "[Server] hajime cwh52b"
# ssh hajime "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_server_hajime -a 10.4.4.6" &


cecho "YELLOW" "Sleep 5 seconds..."
echo -e "\n\n"
sleep 5




# Run RDMA Clients with 1 threading per connection
cecho "YELLOW" "*********************************************"
cecho "YELLOW" "********     Run Clients    *****************"
cecho "YELLOW" "*********************************************"
# ## lumos
cecho "GREEN" "[Client] lumos cwh52a"
ssh lumos "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.1 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] lumos cwh52b"
ssh lumos "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.2 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] lumos cwh61a"
ssh lumos "sudo ip netns exec cwh61a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.3 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] lumos cwh61b"
ssh lumos "sudo ip netns exec cwh61b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.4 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
## caelus
cecho "GREEN" "[Client] caelus cwh52a"
ssh caelus "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.5 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] caelus cwh52b"
ssh caelus "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.6 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] caelus cwh61a"
ssh caelus "sudo ip netns exec cwh61a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.7 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] caelus cwh61b"
ssh caelus "sudo ip netns exec cwh61b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.8 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
## patronus
cecho "GREEN" "[Client] patronus cwh62a"
ssh patronus  "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.9 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] patronus cwh62b"
ssh patronus "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.10 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] patronus cwh51a"
ssh patronus  "sudo ip netns exec cwh51a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.11 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] patronus cwh51b"
ssh patronus "sudo ip netns exec cwh51b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.12 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
## p4campus-svr1
cecho "GREEN" "[Client] p4campus-svr1 cwh62a"
ssh p4campus-svr1 "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.13 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] p4campus-svr1 cwh62b"
ssh p4campus-svr1 "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.14 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] p4campus-svr1 cwh51a"
ssh p4campus-svr1 "sudo ip netns exec cwh51a ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.15 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
cecho "GREEN" "[Client] p4campus-svr1 cwh51b"
ssh p4campus-svr1 "sudo ip netns exec cwh51b ~/workspace/slb_node/ourRDMA/bin/rdma_client -a 10.4.4.16 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &



# # Run RDMA Clients with Energy-Efficiency
# cecho "YELLOW" "**************************************************"
# cecho "YELLOW" "********     Run ESG Clients    ******************"
# cecho "YELLOW" "**************************************************"
# # ## lumos
# cecho "GREEN" "[Client] lumos cwh52a"
# ssh lumos "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.1 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# cecho "GREEN" "[Client] lumos cwh52b"
# ssh lumos "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.2 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# ## caelus
# cecho "GREEN" "[Client] caelus cwh52a"
# ssh caelus "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.5 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# cecho "GREEN" "[Client] caelus cwh52b"
# ssh caelus "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.6 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# ## patronus
# cecho "GREEN" "[Client] patronus cwh62a"
# ssh patronus  "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.9 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# cecho "GREEN" "[Client] patronus cwh62b"
# ssh patronus "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.10 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# ## p4campus-svr1
# cecho "GREEN" "[Client] p4campus-svr1 cwh62a"
# ssh p4campus-svr1 "sudo ip netns exec cwh62a ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.13 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# cecho "GREEN" "[Client] p4campus-svr1 cwh62b"
# ssh p4campus-svr1 "sudo ip netns exec cwh62b ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg -a 10.4.4.14 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# # ## hajime
# # cecho "GREEN" "[Client] hajime cwh52a"
# # ssh hajime "sudo ip netns exec cwh52a ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg_hajime -a 10.4.4.5 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &
# # cecho "GREEN" "[Client] hajime cwh52b"
# # ssh hajime "sudo ip netns exec cwh52b ~/workspace/slb_node/ourRDMA/bin/rdma_client_esg_hajime -a 10.4.4.6 -w /home/mason/workspace/slb_node/ourRDMA/utils/workloads/${WORKLOAD}.txt -l ${LOAD}" &


echo -e "\n\n"
cecho "RED" "Wait ${TIME} seconds then kill all servers..."
sleep ${TIME}
sleep 15 # extra time in rdma_client_esg.cpp


cecho "YELLOW" "Sleep extra 20 seconds, before kill ALL..."
sleep 20
# Clean All
cecho "YELLOW" "\n---------------------------- CLEAN ALL ----------------------------\n"
## lumos
cecho "GREEN" "[CleanUp] lumos"
ssh lumos "sudo killall -2 rdma_server"
ssh lumos "sudo killall -2 rdma_client"
ssh lumos "sudo killall -2 rdma_client_esg"
## caelus
cecho "GREEN" "[CleanUp] caelus"
ssh caelus "sudo killall -2 rdma_server"
ssh caelus "sudo killall -2 rdma_client"
ssh caelus "sudo killall -2 rdma_client_esg"
## patronus
cecho "GREEN" "[CleanUp] patronus"
ssh patronus "sudo killall -2 rdma_server"
ssh patronus "sudo killall -2 rdma_client"
ssh patronus "sudo killall -2 rdma_client_esg"
## p4campus-svr1
cecho "GREEN" "[CleanUp] p4campus-svr1"
ssh p4campus-svr1 "sudo killall -2 rdma_server"
ssh p4campus-svr1 "sudo killall -2 rdma_client"
ssh p4campus-svr1 "sudo killall -2 rdma_client_esg"
# ## hajime
# cecho "GREEN" "[CleanUp] hajime"
# ssh hajime "sudo killall -2 rdma_server_hajime"
# ssh hajime "sudo killall -2 rdma_client_esg_hajime"


# echo "Switch to high performance mode on lumos ..."
# sudo ip netns exec lr sh -c "echo 'GOVERNOR=\"performance\"' > /etc/default/cpufrequtils"
# sudo ip netns exec lr systemctl restart cpufrequtils.service
# echo "Switch to high performance mode on hajime ..."
# ssh hajime "sudo ip netns exec lr sh -c \"echo 'GOVERNOR=\"performance\"' > /etc/default/cpufrequtils\""
# ssh hajime "sudo ip netns exec lr systemctl restart cpufrequtils.service"

# echo "Switch to moderate performance mode on lumos ..."
# sudo ip netns exec lr sh -c "echo 'GOVERNOR=\"ondemand\"' > /etc/default/cpufrequtils"
# sudo ip netns exec lr systemctl restart cpufrequtils.service
# echo "Switch to moderate performance mode on hajime ..."
# ssh hajime "sudo ip netns exec lr sh -c \"echo 'GOVERNOR=\"ondemand\"' > /etc/default/cpufrequtils\""
# ssh hajime "sudo ip netns exec lr systemctl restart cpufrequtils.service"