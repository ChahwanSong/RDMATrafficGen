# RDMA Traffic Generator

## Features
### Persistent RDMA Connections
In this project, we create a persistent connection in order to (1) avoid the idle time to create connection (e.g., handshaking and trading metadata), and (2) allow to maximize link utility and message generating rate.

:star: As we use persistent connection between client-server, there is no handshake or metadata communication before sending message.
This enables to achieve the maximum bandwidth saturation without an idle time.

## Flow Completion Time
We calculate flow completion time (a.k.a. FCT) from the time the first data is sent to the time the last data's ACK is received, which is a pretty standard way of FCT measurement in TCP. We monitor the timestamps at the application layer, putting a brick of code inside the RDMA implementation.

:exclamation: Note that inside the persistent connection, messages use same queue pair destination and addresses. This implies it may be difficult to distinguish messages. To hack this, we analyze the rdma code and put a timestamping code appropriately. Please refer to the code in `src/common.h`.

### Supported RDMA Operations
We support one-sided RDMA operations, e.g., `RDMA_WRITE` and `RDMA_READ`.
:exclamation: It would be easy to support `RDMA_SEND` by changing the `permission` in rdma source code (`src/common.hpp`) and correspondingly on client/server files (`src/rdma_client.hpp` and `src/rdma_server.hpp`).

### SPSC Message Queues for Multi-threading
We leverage Single-Producer Single-Consumer (SPSC) queue to leverage multi-threaded environments and generate messages from rdma clients.
To be specific, through the queue, a random traffic generator creates a message job with `<size, destination>`, then forward this information to one of clients.
The client starts rdma message sending once it receives the job from queue.  

## Configuration
Please find config file `src/config` and write information for the client-server connections. 
For example, given a client and server ip `10.2.2.1` and `10.2.2.2`, respectively, then config file can be written as following:
```
2
10.2.2.1 10.2.2.2 20000
10.2.2.2 10.2.2.1 20100
# client_ip, server_ip, server_ports (unit of 100)
```
The first line is the number of (1-directional, or client->server) connections.
From the second line, we write `<client_ip, server_ip, server_port>` information to create connection.
Note the port number is written by a unit of 100 (i.e., 20000, 20100, 20200, etc...) to avoid port-number collisions.

## Quick start

### Compile with CMake
This project needs minimum version of CMake `>= 3.9`. 
```
cmake .
make clean; make;
```

### First, Run Servers
```
./bin/rdma_server -a 10.2.2.2
```
Use `-h` to see help description.

### Next, Run Clients
```
./bin/rdma_client -a 10.2.2.1
```
or
```
./bin/rdma_client -a 10.2.2.1 -w `pwd`/utils/workloads/Uniform24387.txt -l 0.01
```
Use `-h` to see help description.

### Configure the Number of Persistent Connection For Each Client-Server
See `src/common.hpp:52` and change the number as you want (up to 100)
```
#define NUM_CONNECTIONS (1)
```

To see more details, please use `-h` option.


## Issues

### Workload filename length
When I use `Uniform24387.txt`, it is correctly read. But, when I changed it to `Uniform24387B.txt`, it did not work. Maybe the filename length is not read properly?


## Contacts
* Chahwan Song [skychahwan@gmail.com](skychahwan@gmail.com)
