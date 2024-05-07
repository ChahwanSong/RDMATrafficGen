#include <getopt.h>

#include "common.hpp"

using namespace congweave;

void usage() {
    printf("---Usage---\n");
    printf("rdma: \n[-s <server_ip>] \n[-c <server_ip>] \n[-p <server_port>]\n");
    printf("- server_ip is required\n");
    printf("- default server_port is %d \n", DEFAULT_RDMA_PORT);
}

int main(int argc, char **argv) {
/* ignore warning message by strncpy() */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"

    int option;
    bool rdma_server = false, rdma_client = false, rdma_port = false;
    char *server_ip, *server_port;
    while ((option = getopt(argc, argv, "s:c:p:h")) != -1) {
        switch (option) {
            case 's':
                rdma_server = true;
                printf("[Server] Passed server ip is : %s \n", optarg);
                server_ip = (char *)calloc(strlen(optarg), 1);
                strncpy(server_ip, optarg, strlen(optarg));
                break;
            case 'c':
                rdma_client = true;
                printf("[Client] Passed server ip is : %s \n", optarg);
                server_ip = (char *)calloc(strlen(optarg), 1);
                strncpy(server_ip, optarg, strlen(optarg));
                break;
            case 'p':
                rdma_port = true;
                printf("Passed port is : %s \n", optarg);
                server_port = (char *)calloc(strlen(optarg), 1);
                strncpy(server_port, optarg, strlen(optarg));
                break;
            default:
                usage();
                return 0;
        }
    }

    /* catch parsing error */
    if (rdma_server + rdma_client != 1) {
        printf("ERROR - Only either one of -s or -c should be specified.\n\n");
        usage();
        exit(1);
    }

    if (!rdma_port) {
        std::string default_port = std::to_string(DEFAULT_RDMA_PORT);
        char const *pchar = default_port.c_str();
        server_port = (char *)calloc(strlen(pchar), 1);
        strncpy(server_port, pchar, strlen(pchar));
        printf("Use default port %s \n", server_port);
    }
#pragma GCC diagnostic pop

    int ret = 0;
    uint64_t n_run = 0;
    if (rdma_server) {
        printf("Start server\n");
        ServerRDMA r_server;
        while (true) {
            usleep(100000); // 0.1sec timeout retry to make connection
            printf("*-*-*- Run %lu -*-*-*\n", n_run++);
            ret = r_server.start(server_ip, server_port);
            if (!ret) continue;
            ret = r_server.send_server_metadata();
            if (!ret) continue;
            ret = r_server.disconnect_and_cleanup();
            if (!ret) continue;
        }
    }

    if (rdma_client) {
        printf("Start client\n");
        ClientRDMA r_client;
        while (true) {
            usleep(100000); // 0.1 sec timeout retry to make connection
            printf("*-*-*- Run %lu -*-*-*\n", n_run++);
            ret = r_client.start(server_ip, server_port);
            if (!ret) continue;
            ret = r_client.send_client_metadata();
            if (!ret) {
                ret = r_client.disconnect_and_cleanup();
                continue;
            }
            while (true) {
                getchar();
                ret = r_client.remote_memory_ops(IBV_WR_RDMA_WRITE, 10000);  // IBV_WR_RDMA_WRITE, IBV_WR_RDMA_READ
                if (!ret) {
                    ret = r_client.disconnect_and_cleanup();
                    continue;
                }
            }
            ret = r_client.disconnect_and_cleanup();
        }
    }

    printf("==End==\n");
}