# ConWeave Experiment Results

Experiment setup:
* MessageQueue size = 16K. Over than that, it would drop the message (failure).
* By default, we use 100us timegap for flowlets, and 12us deadline for ConWeave.
* DCQCN (e.g., 100-400-0.2)
* PFC (e.g., 600-40)
* Runtime (e.g., 30 seconds)
* Queue configuration is set by `run_pd_rpc.py`
* 2 connections between each pair of client-server
* **Full-bisection Bandwidth (FB), or Over-Subscription 1 (os1)**


**The file format is written as following**
```python3
{workload (e.g., solar)}_{load (e.g., 50)}_{lb_mode (e.g., ecmp)}
```

Folder Name: `FINAL_Solar_OS1_100-400-0.2_600-40_30s_2Conn_StaticRPC_output`

**Queue Configuration at Switches**
Leaf switch: `dp_list = [168, 176, 184, 192, 24, 25, 26, 27, 32, 34, 36, 38]`.
* Leaf switch -> 13000 cells * 176 bytes for Q0 of ports, 
* Spine switch -> 20000 cells * 80 bytes for Q0 of ports.



## Logging with Full-BiSection & Solar2022 & Load 50
TrafficGen Code: `./run_evaluation.sh Solar2022 0.5 320`



### ConWeave

```python3
python3 analysis.py -d 30 -o solar_50_conweave

Tx Matched Pkts: 301744115
Rx (debug)Matched Pkts: 307477601
Rx (debug)#newOoO: 1143284
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [301744115, 0]
reg_dummy_ts_now_32b: [171147703, 171147701]
reg_dummy_ts_now_16b: [4927, 0]
reg_dummy_ts_lop_16b: [0, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 17] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 8] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 1143260] (LOP before timeout)

reg_debug_resubmit: [19129188, 17]
reg_debug_recirc: [0, 21546306]
reg_debug_late_reply: [2417118, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 1637937] (at RxToR)

reg_debug_eg_cntr1: [0, 2417176] (reply of LOP)
reg_debug_eg_cntr2: [0, 19129130] (reply of INIT)
reg_debug_eg_cntr3: [0, 496155] (feedback)
reg_debug_eg_cntr4: [0, 1143267] (cwctrl)
reg_debug_eg_cntr5: [0, 1143260] (resume Queue by LOP)



-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        63819591|         1922955|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        63776456|         1921720|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        66733219|         2006833|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        66945975|         2013991|*
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         6678336|        67519774|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         7957330|        64092500|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         5503797|        65228339|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         9272214|        64433769|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        67519774|         6678340|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        64092500|         7957344|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        65228339|         5503803|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        64433769|         9272218|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         1973642|        65539217|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         1979672|        65635595|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         1938485|        64577652|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         1973588|        65522046|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|          496155|          496155|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        46409788|        46409788|


dev_port:24, queue_id:2, pg_id:3, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:24, queue_id:3, pg_id:3, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1981
dev_port:24, queue_id:4, pg_id:3, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2142
dev_port:24, queue_id:5, pg_id:3, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2079
dev_port:24, queue_id:6, pg_id:3, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2128
dev_port:24, queue_id:7, pg_id:3, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2111
dev_port:24, queue_id:8, pg_id:3, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:24, queue_id:9, pg_id:3, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2272
dev_port:24, queue_id:10, pg_id:3, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1624
dev_port:24, queue_id:11, pg_id:3, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1806
dev_port:24, queue_id:12, pg_id:3, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1743
dev_port:24, queue_id:13, pg_id:3, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1435
dev_port:25, queue_id:2, pg_id:3, pg_queue:18, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2814
dev_port:25, queue_id:3, pg_id:3, pg_queue:19, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:25, queue_id:4, pg_id:3, pg_queue:20, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2142
dev_port:25, queue_id:5, pg_id:3, pg_queue:21, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2387
dev_port:25, queue_id:6, pg_id:3, pg_queue:22, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2583
dev_port:25, queue_id:7, pg_id:3, pg_queue:23, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2506
dev_port:25, queue_id:8, pg_id:3, pg_queue:24, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:25, queue_id:9, pg_id:3, pg_queue:25, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1723
dev_port:25, queue_id:10, pg_id:3, pg_queue:26, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1876
dev_port:25, queue_id:11, pg_id:3, pg_queue:27, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1281
dev_port:25, queue_id:12, pg_id:3, pg_queue:28, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1621
dev_port:25, queue_id:13, pg_id:3, pg_queue:29, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1799
dev_port:26, queue_id:2, pg_id:3, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2492
dev_port:26, queue_id:3, pg_id:3, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2443
dev_port:26, queue_id:4, pg_id:3, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:26, queue_id:5, pg_id:3, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2233
dev_port:26, queue_id:6, pg_id:3, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2170
dev_port:26, queue_id:7, pg_id:3, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:26, queue_id:8, pg_id:3, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1967
dev_port:26, queue_id:9, pg_id:3, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1862
dev_port:26, queue_id:10, pg_id:3, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:994
dev_port:26, queue_id:11, pg_id:3, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:961
dev_port:26, queue_id:12, pg_id:3, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2051
dev_port:26, queue_id:13, pg_id:3, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1470
dev_port:27, queue_id:2, pg_id:3, pg_queue:50, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2275
dev_port:27, queue_id:3, pg_id:3, pg_queue:51, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2044
dev_port:27, queue_id:4, pg_id:3, pg_queue:52, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2142
dev_port:27, queue_id:5, pg_id:3, pg_queue:53, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2300
dev_port:27, queue_id:6, pg_id:3, pg_queue:54, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2069
dev_port:27, queue_id:7, pg_id:3, pg_queue:55, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1628
dev_port:27, queue_id:8, pg_id:3, pg_queue:56, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1925
dev_port:27, queue_id:9, pg_id:3, pg_queue:57, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2296
dev_port:27, queue_id:10, pg_id:3, pg_queue:58, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1386
dev_port:27, queue_id:11, pg_id:3, pg_queue:59, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1600
dev_port:27, queue_id:12, pg_id:3, pg_queue:60, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1187
dev_port:27, queue_id:13, pg_id:3, pg_queue:61, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1596

--> absolute flow completion time (total)
Avg:	69.13us
p50:	47.16us
p95:	161.93us
p99:	421.24us
p99.9:	1729.21us
p99.99:	3142.68us
stdev:	132.04

--> absolute flow completion time (short)
Avg:	35.17us
p50:	25.30us
p95:	70.96us
p99:	136.96us
p99.9:	1364.57us
p99.99:	1745.79us
stdev:	71.34

--> absolute flow completion time (long)
Avg:	108.01us
p50:	75.39us
p95:	235.40us
p99:	732.55us
p99.9:	1963.17us
p99.99:	3707.69us
stdev:	169.50


```

### ECMP

```python3
python3 analysis.py -d 30 -o solar_50_ecmp


-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:4850
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:4452
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:2178
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:4359
dev_port:56 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:64 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:40 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:48 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:136 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:137 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:138 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:139 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:144 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:146 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:148 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:150 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:4655
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:4714
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:4650
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:4698
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0


--> absolute flow completion time (total)
Avg:	79.46us
p50:	49.50us
p95:	183.62us
p99:	632.59us
p99.9:	2091.82us
p99.99:	4730.73us
stdev:	178.28

--> absolute flow completion time (short)
Avg:	36.08us
p50:	24.50us
p95:	74.88us
p99:	136.08us
p99.9:	1453.25us
p99.99:	2858.27us
stdev:	90.14

--> absolute flow completion time (long)
Avg:	129.10us
p50:	83.83us
p95:	280.75us
p99:	1478.01us
p99.9:	2913.90us
p99.99:	5350.80us
stdev:	232.90



```

### LETFLOW

```python3
python3 analysis.py -d 30 -o solar_50_letflow

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:5102
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:4525
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:3640
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:2207
dev_port:56 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:64 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:40 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:48 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:136 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:137 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:138 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:139 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:144 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:146 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:148 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:150 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:4796
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:4645
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:3563
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:4999
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0

--> absolute flow completion time (total)
Avg:	74.86us
p50:	48.83us
p95:	177.70us
p99:	448.21us
p99.9:	1837.94us
p99.99:	3894.00us
stdev:	149.78

--> absolute flow completion time (short)
Avg:	34.14us
p50:	24.15us
p95:	73.54us
p99:	123.78us
p99.9:	1378.77us
p99.99:	2289.71us
stdev:	76.59

--> absolute flow completion time (long)
Avg:	121.47us
p50:	85.27us
p95:	261.17us
p99:	1265.14us
p99.9:	2188.00us
p99.99:	4764.06us
stdev:	193.19


```


### DRILL


```python3
python3 analysis.py -d 30 -o solar_50_drill


-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:119
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:84
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:112
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:67
dev_port:56 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:64 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:40 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:48 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:136 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:137 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:138 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:139 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:144 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:146 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:148 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:150 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:1515
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:1366
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:1058
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:1009
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:91
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0



--> absolute flow completion time (total)
Avg:	312.79us
p50:	141.41us
p95:	643.65us
p99:	1903.30us
p99.9:	2249.77us
p99.99:	2468.66us
stdev:	349.00

--> absolute flow completion time (short)
Avg:	70.92us
p50:	31.08us
p95:	163.46us
p99:	243.46us
p99.9:	277.50us
p99.99:	320.10us
stdev:	56.84

--> absolute flow completion time (long)
Avg:	589.16us
p50:	508.55us
p95:	1467.90us
p99:	2062.31us
p99.9:	2328.08us
p99.99:	2545.18us
stdev:	337.72


FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:74202, failed goodput:0.798 Gbps, failed throughput:0.845 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.14		#.failed msgs:70634, failed goodput:0.761 Gbps, failed throughput:0.805 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.9		#.failed msgs:68034, failed goodput:0.736 Gbps, failed throughput:0.779 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:63852, failed goodput:0.687 Gbps, failed throughput:0.727 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.10		#.failed msgs:64420, failed goodput:0.699 Gbps, failed throughput:0.740 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.9		#.failed msgs:62433, failed goodput:0.673 Gbps, failed throughput:0.712 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.13		#.failed msgs:63893, failed goodput:0.694 Gbps, failed throughput:0.735 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.14		#.failed msgs:60879, failed goodput:0.659 Gbps, failed throughput:0.698 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:85987, failed goodput:0.922 Gbps, failed throughput:0.976 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:87239, failed goodput:0.938 Gbps, failed throughput:0.993 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:72836, failed goodput:0.783 Gbps, failed throughput:0.829 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:67362, failed goodput:0.727 Gbps, failed throughput:0.770 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:56023, failed goodput:0.606 Gbps, failed throughput:0.642 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.13		#.failed msgs:53325, failed goodput:0.576 Gbps, failed throughput:0.610 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:50559, failed goodput:0.546 Gbps, failed throughput:0.578 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:49937, failed goodput:0.539 Gbps, failed throughput:0.571 Gbps


```


## Logging with Full-BiSection & Solar2022 & Load 80
TrafficGen Code: `./run_evaluation.sh Solar2022 0.8 320`


### ConWeave

```python3
python3 analysis.py -d 30 -o solar_80_conweave

Tx Matched Pkts: 467094778
Rx (debug)Matched Pkts: 560770412
Rx (debug)#newOoO: 4391566
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [467094778, 0]
reg_dummy_ts_now_32b: [169644084, 169644082]
reg_dummy_ts_now_16b: [15677, 0]
reg_dummy_ts_lop_16b: [0, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 205] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 62] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 4391361] (LOP before timeout)

reg_debug_resubmit: [34326214, 205]
reg_debug_recirc: [0, 43269084]
reg_debug_late_reply: [8942870, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 8151016] (at RxToR)

reg_debug_eg_cntr1: [0, 8942866] (reply of LOP)
reg_debug_eg_cntr2: [0, 34326218] (reply of INIT)
reg_debug_eg_cntr3: [0, 3029240] (feedback)
reg_debug_eg_cntr4: [0, 4391361] (cwctrl)
reg_debug_eg_cntr5: [0, 4391361] (resume Queue by LOP)

-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        93786180|         3372870|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        93369393|         3431746|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       101304769|         3595701|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       101322205|         3596995|
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        23611308|        95426251|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        10220610|       102975200|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        10262817|        95627657|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        13171533|        95753423|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95426251|        23611308|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       102975200|        10220610|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95627657|        10262817|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95753423|        13171533|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         3487778|        97756298|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         3502638|        97292598|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         3499997|        97179657|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         3506787|        97554106|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              32|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|         3029240|         3029240|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|       171270727|       171270727|



dev_port:24, queue_id:2, pg_id:3, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1148
dev_port:24, queue_id:3, pg_id:3, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1127
dev_port:24, queue_id:4, pg_id:3, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1470
dev_port:24, queue_id:5, pg_id:3, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1127
dev_port:24, queue_id:6, pg_id:3, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1477
dev_port:24, queue_id:7, pg_id:3, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1089
dev_port:24, queue_id:8, pg_id:3, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1057
dev_port:24, queue_id:9, pg_id:3, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:861
dev_port:24, queue_id:10, pg_id:3, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1022
dev_port:24, queue_id:11, pg_id:3, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:868
dev_port:24, queue_id:12, pg_id:3, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:644
dev_port:24, queue_id:13, pg_id:3, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:917
dev_port:25, queue_id:2, pg_id:3, pg_queue:18, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:973
dev_port:25, queue_id:3, pg_id:3, pg_queue:19, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1316
dev_port:25, queue_id:4, pg_id:3, pg_queue:20, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:25, queue_id:5, pg_id:3, pg_queue:21, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1250
dev_port:25, queue_id:6, pg_id:3, pg_queue:22, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1204
dev_port:25, queue_id:7, pg_id:3, pg_queue:23, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1197
dev_port:25, queue_id:8, pg_id:3, pg_queue:24, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1253
dev_port:25, queue_id:9, pg_id:3, pg_queue:25, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1344
dev_port:25, queue_id:10, pg_id:3, pg_queue:26, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:25, queue_id:11, pg_id:3, pg_queue:27, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:840
dev_port:25, queue_id:12, pg_id:3, pg_queue:28, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1036
dev_port:25, queue_id:13, pg_id:3, pg_queue:29, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:868
dev_port:26, queue_id:2, pg_id:3, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1029
dev_port:26, queue_id:3, pg_id:3, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1295
dev_port:26, queue_id:4, pg_id:3, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1043
dev_port:26, queue_id:5, pg_id:3, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1099
dev_port:26, queue_id:6, pg_id:3, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1064
dev_port:26, queue_id:7, pg_id:3, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1047
dev_port:26, queue_id:8, pg_id:3, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1155
dev_port:26, queue_id:9, pg_id:3, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:26, queue_id:10, pg_id:3, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1061
dev_port:26, queue_id:11, pg_id:3, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:840
dev_port:26, queue_id:12, pg_id:3, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1274
dev_port:26, queue_id:13, pg_id:3, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:676
dev_port:27, queue_id:2, pg_id:3, pg_queue:50, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1248
dev_port:27, queue_id:3, pg_id:3, pg_queue:51, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1645
dev_port:27, queue_id:4, pg_id:3, pg_queue:52, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1078
dev_port:27, queue_id:5, pg_id:3, pg_queue:53, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1309
dev_port:27, queue_id:6, pg_id:3, pg_queue:54, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1152
dev_port:27, queue_id:7, pg_id:3, pg_queue:55, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1099
dev_port:27, queue_id:8, pg_id:3, pg_queue:56, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1400
dev_port:27, queue_id:9, pg_id:3, pg_queue:57, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:868
dev_port:27, queue_id:10, pg_id:3, pg_queue:58, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:27, queue_id:11, pg_id:3, pg_queue:59, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:924
dev_port:27, queue_id:12, pg_id:3, pg_queue:60, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:637
dev_port:27, queue_id:13, pg_id:3, pg_queue:61, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:819



--> absolute flow completion time (total)
Avg:	105.98us
p50:	87.00us
p95:	238.13us
p99:	462.42us
p99.9:	839.44us
p99.99:	1809.06us
stdev:	94.98

--> absolute flow completion time (short)
Avg:	50.60us
p50:	44.79us
p95:	101.08us
p99:	137.25us
p99.9:	217.33us
p99.99:	1371.93us
stdev:	36.68

--> absolute flow completion time (long)
Avg:	169.56us
p50:	146.76us
p95:	361.00us
p99:	531.09us
p99.9:	1450.77us
p99.99:	1953.75us
stdev:	101.33



```


### ECMP

```python3
python3 analysis.py -d 30 -o solar_80_ecmp


-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:1844
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:2936
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:2641
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:1527
dev_port:56 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:64 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:40 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:48 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:136 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:137 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:138 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:139 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:144 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:146 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:148 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:150 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:2941
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:3679
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:3356
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:2871
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0

--> absolute flow completion time (total)
Avg:	111.90us
p50:	78.97us
p95:	269.04us
p99:	580.26us
p99.9:	1539.27us
p99.99:	2087.91us
stdev:	122.37

--> absolute flow completion time (short)
Avg:	45.71us
p50:	40.12us
p95:	92.25us
p99:	121.51us
p99.9:	200.20us
p99.99:	1449.64us
stdev:	37.22

--> absolute flow completion time (long)
Avg:	187.84us
p50:	154.68us
p95:	399.63us
p99:	736.84us
p99.9:	1742.61us
p99.99:	2265.50us
stdev:	140.60


FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:32316, failed goodput:0.346 Gbps, failed throughput:0.367 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:83879, failed goodput:0.901 Gbps, failed throughput:0.954 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:32349, failed goodput:0.345 Gbps, failed throughput:0.366 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:3123, failed goodput:0.034 Gbps, failed throughput:0.035 Gbps

```



### LETFLOW

```python3
python3 analysis.py -d 30 -o solar_80_letflow

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:3058
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:3933
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:1501
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:1701
dev_port:56 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:64 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:40 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:48 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:136 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:137 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:138 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:139 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:144 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:146 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:148 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:150 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:3075
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:3282
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:3264
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:3483
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0


--> absolute flow completion time (total)
Avg:	114.47us
p50:	74.25us
p95:	296.08us
p99:	602.42us
p99.9:	1568.34us
p99.99:	2165.85us
stdev:	129.93

--> absolute flow completion time (short)
Avg:	46.51us
p50:	40.96us
p95:	96.29us
p99:	127.42us
p99.9:	224.88us
p99.99:	1467.50us
stdev:	41.58

--> absolute flow completion time (long)
Avg:	192.47us
p50:	163.44us
p95:	425.11us
p99:	778.04us
p99.9:	1771.65us
p99.99:	2368.47us
stdev:	151.28



FAILED: Src:10.4.4.2 -> Dst:10.4.4.13		#.failed msgs:107289, failed goodput:1.159 Gbps, failed throughput:1.227 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:91096, failed goodput:0.986 Gbps, failed throughput:1.044 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:33919, failed goodput:0.366 Gbps, failed throughput:0.387 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:36317, failed goodput:0.389 Gbps, failed throughput:0.412 Gbps


```



### DRILL

```python3
python3 analysis.py -d 30 -o solar_80_drill


--> absolute flow completion time (total)
Avg:	322.31us
p50:	155.79us
p95:	709.21us
p99:	1947.64us
p99.9:	2502.02us
p99.99:	2787.06us
stdev:	364.34

--> absolute flow completion time (short)
Avg:	72.70us
p50:	33.17us
p95:	168.59us
p99:	254.88us
p99.9:	310.11us
p99.99:	359.32us
stdev:	60.72

--> absolute flow completion time (long)
Avg:	608.45us
p50:	527.38us
p95:	1511.10us
p99:	2194.81us
p99.9:	2616.73us
p99.99:	2858.72us
stdev:	356.91


FAILED: Src:10.4.4.6 -> Dst:10.4.4.14		#.failed msgs:284551, failed goodput:3.067 Gbps, failed throughput:3.247 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:256435, failed goodput:2.755 Gbps, failed throughput:2.917 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:231308, failed goodput:2.490 Gbps, failed throughput:2.636 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.9		#.failed msgs:218153, failed goodput:2.353 Gbps, failed throughput:2.491 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.14		#.failed msgs:229419, failed goodput:2.476 Gbps, failed throughput:2.621 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.9		#.failed msgs:225267, failed goodput:2.431 Gbps, failed throughput:2.573 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.13		#.failed msgs:221436, failed goodput:2.387 Gbps, failed throughput:2.528 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.10		#.failed msgs:219788, failed goodput:2.366 Gbps, failed throughput:2.505 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:259824, failed goodput:2.796 Gbps, failed throughput:2.960 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:254317, failed goodput:2.736 Gbps, failed throughput:2.897 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:243285, failed goodput:2.626 Gbps, failed throughput:2.780 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:235591, failed goodput:2.542 Gbps, failed throughput:2.691 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:236384, failed goodput:2.541 Gbps, failed throughput:2.690 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:234360, failed goodput:2.520 Gbps, failed throughput:2.668 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.13		#.failed msgs:202914, failed goodput:2.181 Gbps, failed throughput:2.309 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:171090, failed goodput:1.841 Gbps, failed throughput:1.949 Gbps

```