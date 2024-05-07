#include "common.hpp"

using namespace congweave;

void usage() {
    printf("---Usage---\n");
    printf("rdma_server: \n[-a <server ip of this node>] \n[-c <config file directory (absolute)>]\n");
    printf("Default config directory: `pwd`/config/config\n");
}

int main(int argc, char **argv) {
/* ignore warning message by strncpy() */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"

    int option;
    char *node_ip = NULL, *config_dir = NULL;
    while ((option = getopt(argc, argv, "a:c:")) != -1) {
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
            default:
                usage();
                return 0;
        }
    }
#pragma GCC diagnostic pop

    /* sanity check */
    if (!node_ip) {
        printf("ERROR - Specify client ip of this node.\n\n");
        usage();
        exit(1);
    }

    /* if no input, set default directory */
    if (!config_dir) {
        /* get filepath */
        std::string filepath(__FILE__);
        replace(filepath, "/src/rdma_server.cpp", "/config/config");
        config_dir = str_copy(filepath.c_str());
        printf("Use default config directory - %s\n\n", config_dir);
    }

    /* Read the configuration file */
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
            if (server_ip == std::string(node_ip)) {
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
                printf("\n\n");
            }
            fflush(stdout);
        }
        conf.close();
    } else {
        std::cerr << "Error: require a config file\n";
        fflush(stdout);
        return 1;
    }

    uint32_t n_servers = server_port_pairs.size();
    uint32_t n_connections = server_port_all_pairs.size();
    printf("Finished reading config.\nNumber of servers is : %u, connections is : %u\n\n", n_servers, n_connections);

    /* run persistent connection to each server's port */
    std::thread conn_threads[n_connections];
    for (uint32_t i = 0; i < n_connections; i++) {
        conn_threads[i] = std::thread([&](int i) {
            printf("Start server [idx:%u] with port %u \n", i, server_port_all_pairs[i].second);
            ServerRDMA r_server;
            int ret;
            uint32_t n_run = 0;
            while (true) {
                usleep(100000);  // 0.1sec timeout retry to make connection
                printf("[Server %u] Run %u -> server ip: %s, server port: %u -*-*-*\n",
                       i, n_run++, server_port_all_pairs[i].first.c_str(), server_port_all_pairs[i].second);
                ret = r_server.start(server_port_all_pairs[i].first.c_str(), std::to_string(server_port_all_pairs[i].second).c_str());
                if (!ret) continue;
                ret = r_server.send_server_metadata();
                if (!ret) continue;
                ret = r_server.disconnect_and_cleanup();
                if (!ret) continue;
            }
        },
                                      i);
    }

    /* execute threads */
    for (uint32_t i = 0; i < n_connections; i++) {
        conn_threads[i].join();
    }
}