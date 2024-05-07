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

cecho "RED" "\n======================= S H U T D O W N  A L L =============================\n"

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