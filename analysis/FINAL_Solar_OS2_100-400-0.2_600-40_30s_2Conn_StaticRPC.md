# ConWeave Experiment Results

Experiment setup:
* MessageQueue size = 16K. Over than that, it would drop the message (failure).
* By default, we use 100us timegap for flowlets, and 12us deadline for ConWeave.
* DCQCN (e.g., 100-400-0.2)
* PFC (e.g., 600-40)
* Runtime (e.g., 30 seconds)
* Queue configuration is set by `run_pd_rpc.py`
* 2 connections between each pair of client-server
* **Over-Subscription 2:1**


**The file format is written as following**
```python3
{workload (e.g., solar)}_{load (e.g., 50)}_{lb_mode (e.g., ecmp)}
```

Folder Name: `FINAL_Solar_OS2_100-400-0.2_600-40_30s_2Conn_StaticRPC_output`

**Queue Configuration at Switches**
Leaf switch: `dp_list = [168, 176, 184, 192, 24, 25, 26, 27, 32, 34, 36, 38]`.
* Leaf switch -> 13000 cells * 176 bytes for Q0 of ports, 
* Spine switch -> 20000 cells * 80 bytes for Q0 of ports.



## Logging with Full-BiSection & Solar2022 & Load 50
TrafficGen Code: `./run_evaluation.sh Solar2022 0.25 320`


### ConWeave
```python3
python3 analysis.py -d 30 -o solar_50_conweave

Tx Matched Pkts: 282264330
Rx (debug)Matched Pkts: 287447138
Rx (debug)#newOoO: 1091731
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [282264330, 0]
reg_dummy_ts_now_32b: [139533315, 139533313]
reg_dummy_ts_now_16b: [31668, 0]
reg_dummy_ts_lop_16b: [0, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 2] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 8] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 6] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 1091711] (LOP before timeout)

reg_debug_resubmit: [15111077, 8]
reg_debug_recirc: [0, 17586769]
reg_debug_late_reply: [2475692, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 2018578] (at RxToR)

reg_debug_eg_cntr1: [0, 2475693] (reply of LOP)
reg_debug_eg_cntr2: [0, 15111078] (reply of INIT)
reg_debug_eg_cntr3: [0, 1209926] (feedback)
reg_debug_eg_cntr4: [0, 1091723] (cwctrl)
reg_debug_eg_cntr5: [0, 1091711] (resume Queue by LOP)

-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30890698|          966966|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30936576|          967379|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        31769738|          993628|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        31701219|          991018|*
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30895578|          964619|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30995545|          968064|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        31690496|          990109|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        31750752|          991464|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         7227461|        62711058|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         7227106|        62705880|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         5951271|        62651085|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         5013154|        62562336|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        62711058|         7227461|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        62705880|         7227120|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        62651085|         5951271|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        62562336|         5013154|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          982614|        31456740|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          978244|        31247047|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          978280|        31365436|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          979880|        31404669|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          971315|        31036804|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          981914|        31475816|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          975851|        31130604|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |          984191|        31514267|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|         1209926|         1209926|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        37880822|        37880822|

dev_port:24, queue_id:2, pg_id:3, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1890
dev_port:24, queue_id:3, pg_id:3, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2079
dev_port:24, queue_id:4, pg_id:3, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2212
dev_port:24, queue_id:5, pg_id:3, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1918
dev_port:24, queue_id:6, pg_id:3, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1876
dev_port:24, queue_id:7, pg_id:3, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1569
dev_port:24, queue_id:8, pg_id:3, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1603
dev_port:24, queue_id:9, pg_id:3, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1421
dev_port:24, queue_id:10, pg_id:3, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:441
dev_port:24, queue_id:11, pg_id:3, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:455
dev_port:24, queue_id:12, pg_id:3, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:616
dev_port:24, queue_id:13, pg_id:3, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1239
dev_port:25, queue_id:2, pg_id:3, pg_queue:18, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2135
dev_port:25, queue_id:3, pg_id:3, pg_queue:19, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1974
dev_port:25, queue_id:4, pg_id:3, pg_queue:20, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2093
dev_port:25, queue_id:5, pg_id:3, pg_queue:21, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2069
dev_port:25, queue_id:6, pg_id:3, pg_queue:22, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1568
dev_port:25, queue_id:7, pg_id:3, pg_queue:23, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1554
dev_port:25, queue_id:8, pg_id:3, pg_queue:24, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1551
dev_port:25, queue_id:9, pg_id:3, pg_queue:25, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1495
dev_port:25, queue_id:10, pg_id:3, pg_queue:26, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:392
dev_port:25, queue_id:11, pg_id:3, pg_queue:27, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:777
dev_port:25, queue_id:12, pg_id:3, pg_queue:28, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:522
dev_port:25, queue_id:13, pg_id:3, pg_queue:29, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:749
dev_port:26, queue_id:2, pg_id:3, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1824
dev_port:26, queue_id:3, pg_id:3, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1743
dev_port:26, queue_id:4, pg_id:3, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1939
dev_port:26, queue_id:5, pg_id:3, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1645
dev_port:26, queue_id:6, pg_id:3, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1470
dev_port:26, queue_id:7, pg_id:3, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1647
dev_port:26, queue_id:8, pg_id:3, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2307
dev_port:26, queue_id:9, pg_id:3, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1792
dev_port:26, queue_id:10, pg_id:3, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:651
dev_port:26, queue_id:11, pg_id:3, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1061
dev_port:26, queue_id:12, pg_id:3, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:860
dev_port:26, queue_id:13, pg_id:3, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:42
dev_port:27, queue_id:2, pg_id:3, pg_queue:50, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2100
dev_port:27, queue_id:3, pg_id:3, pg_queue:51, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1834
dev_port:27, queue_id:4, pg_id:3, pg_queue:52, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2290
dev_port:27, queue_id:5, pg_id:3, pg_queue:53, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1645
dev_port:27, queue_id:6, pg_id:3, pg_queue:54, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1954
dev_port:27, queue_id:7, pg_id:3, pg_queue:55, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:27, queue_id:8, pg_id:3, pg_queue:56, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1596
dev_port:27, queue_id:9, pg_id:3, pg_queue:57, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1981
dev_port:27, queue_id:10, pg_id:3, pg_queue:58, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1533
dev_port:27, queue_id:11, pg_id:3, pg_queue:59, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:840
dev_port:27, queue_id:12, pg_id:3, pg_queue:60, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1652
dev_port:27, queue_id:13, pg_id:3, pg_queue:61, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:732
dev_port:32, queue_id:2, pg_id:4, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1897
dev_port:32, queue_id:3, pg_id:4, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1624
dev_port:32, queue_id:4, pg_id:4, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1939
dev_port:32, queue_id:5, pg_id:4, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2065
dev_port:32, queue_id:6, pg_id:4, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1871
dev_port:32, queue_id:7, pg_id:4, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1915
dev_port:32, queue_id:8, pg_id:4, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2058
dev_port:32, queue_id:9, pg_id:4, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1463
dev_port:32, queue_id:10, pg_id:4, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:833
dev_port:32, queue_id:11, pg_id:4, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2002
dev_port:32, queue_id:12, pg_id:4, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:728
dev_port:32, queue_id:13, pg_id:4, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1141
dev_port:34, queue_id:2, pg_id:4, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2177
dev_port:34, queue_id:3, pg_id:4, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1943
dev_port:34, queue_id:4, pg_id:4, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1355
dev_port:34, queue_id:5, pg_id:4, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2065
dev_port:34, queue_id:6, pg_id:4, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1982
dev_port:34, queue_id:7, pg_id:4, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1894
dev_port:34, queue_id:8, pg_id:4, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1814
dev_port:34, queue_id:9, pg_id:4, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1437
dev_port:34, queue_id:10, pg_id:4, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:630
dev_port:34, queue_id:11, pg_id:4, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1330
dev_port:34, queue_id:12, pg_id:4, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1764
dev_port:34, queue_id:13, pg_id:4, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1479
dev_port:36, queue_id:2, pg_id:4, pg_queue:66, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1719
dev_port:36, queue_id:3, pg_id:4, pg_queue:67, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1835
dev_port:36, queue_id:4, pg_id:4, pg_queue:68, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2233
dev_port:36, queue_id:5, pg_id:4, pg_queue:69, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2170
dev_port:36, queue_id:6, pg_id:4, pg_queue:70, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:36, queue_id:7, pg_id:4, pg_queue:71, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1877
dev_port:36, queue_id:8, pg_id:4, pg_queue:72, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1645
dev_port:36, queue_id:9, pg_id:4, pg_queue:73, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1512
dev_port:36, queue_id:10, pg_id:4, pg_queue:74, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:945
dev_port:36, queue_id:11, pg_id:4, pg_queue:75, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:692
dev_port:36, queue_id:12, pg_id:4, pg_queue:76, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1666
dev_port:36, queue_id:13, pg_id:4, pg_queue:77, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2688
dev_port:38, queue_id:2, pg_id:4, pg_queue:98, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1841
dev_port:38, queue_id:3, pg_id:4, pg_queue:99, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1904
dev_port:38, queue_id:4, pg_id:4, pg_queue:100, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2258
dev_port:38, queue_id:5, pg_id:4, pg_queue:101, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2177
dev_port:38, queue_id:6, pg_id:4, pg_queue:102, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1530
dev_port:38, queue_id:7, pg_id:4, pg_queue:103, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1869
dev_port:38, queue_id:8, pg_id:4, pg_queue:104, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1432
dev_port:38, queue_id:9, pg_id:4, pg_queue:105, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2079
dev_port:38, queue_id:10, pg_id:4, pg_queue:106, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:900
dev_port:38, queue_id:11, pg_id:4, pg_queue:107, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:618
dev_port:38, queue_id:12, pg_id:4, pg_queue:108, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:784
dev_port:38, queue_id:13, pg_id:4, pg_queue:109, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1278
dev_port:152, queue_id:0, pg_id:3, pg_queue:0, pipe:1 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:160, queue_id:0, pg_id:4, pg_queue:0, pipe:1 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:8, queue_id:0, pg_id:1, pg_queue:0, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:3
dev_port:16, queue_id:0, pg_id:2, pg_queue:0, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:6


--> absolute flow completion time (total)
Avg:	55.90us
p50:	40.82us
p95:	121.96us
p99:	214.04us
p99.9:	1371.47us
p99.99:	10167.27us
stdev:	389.79

--> absolute flow completion time (short)
Avg:	31.63us
p50:	21.66us
p95:	63.13us
p99:	106.81us
p99.9:	339.44us
p99.99:	5798.88us
stdev:	332.90

--> absolute flow completion time (long)
Avg:	83.69us
p50:	63.50us
p95:	159.64us
p99:	262.58us
p99.9:	1590.56us
p99.99:	20495.00us
stdev:	444.46

```


### ECMP

```python3
python3 analysis.py -d 30 -o solar_50_ecmp

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:3903
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:6137
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:5074
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:7867
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:3579
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:5091
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:4404
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:5468
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:3908
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:4369
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:4036
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:4443
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0

--> absolute flow completion time (total)
Avg:	74.63us
p50:	43.04us
p95:	158.65us
p99:	285.41us
p99.9:	2492.33us
p99.99:	47760.08us
stdev:	730.44

--> absolute flow completion time (short)
Avg:	43.87us
p50:	20.89us
p95:	85.47us
p99:	149.60us
p99.9:	1450.95us
p99.99:	47532.83us
stdev:	661.67

--> absolute flow completion time (long)
Avg:	109.85us
p50:	67.97us
p95:	203.96us
p99:	375.94us
p99.9:	3755.16us
p99.99:	47799.91us
stdev:	800.52

```

### LETFLOW

```python3
python3 analysis.py -d 30 -o solar_50_letflow

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:7916
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:6285
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:5315
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:5913
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:4800
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:4433
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:4609
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:5195
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:4239
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:5835
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:4515
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:3991
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0

--> absolute flow completion time (total)
Avg:	65.04us
p50:	40.37us
p95:	134.00us
p99:	239.58us
p99.9:	1756.50us
p99.99:	46202.58us
stdev:	626.59

--> absolute flow completion time (short)
Avg:	37.92us
p50:	20.13us
p95:	70.67us
p99:	124.50us
p99.9:	1313.42us
p99.99:	44077.34us
stdev:	567.09

--> absolute flow completion time (long)
Avg:	96.10us
p50:	63.13us
p95:	174.04us
p99:	306.63us
p99.9:	2262.35us
p99.99:	47584.73us
stdev:	687.10

```


### DRILL

```python3
python3 analysis.py -d 30 -o solar_50_drill

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:4579
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:4558
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:4661
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:4566
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:2192
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:2444
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:1680
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:1983
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:2199
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:1978
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:2187
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:1995
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;53509, usage_cells:0, watermark_cells:36532
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0


--> absolute flow completion time (total)
Avg:	366.22us
p50:	157.04us
p95:	1477.89us
p99:	2974.29us
p99.9:	5995.37us
p99.99:	7176.93us
stdev:	623.21

--> absolute flow completion time (short)
Avg:	88.00us
p50:	50.79us
p95:	354.59us
p99:	535.50us
p99.9:	834.24us
p99.99:	1702.07us
stdev:	123.61

--> absolute flow completion time (long)
Avg:	684.59us
p50:	337.26us
p95:	1679.81us
p99:	4543.32us
p99.9:	6401.49us
p99.99:	7613.09us
stdev:	790.82



```




## Logging with Full-BiSection & Solar2022 & Load 80
`./run_evaluation.sh Solar2022 0.4 320`


### ConWeave
```python3
python3 analysis.py -d 30 -o solar_80_conweave

Tx Matched Pkts: 445510179
Rx (debug)Matched Pkts: 566620830
Rx (debug)#newOoO: 4151320
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [445510179, 0]
reg_dummy_ts_now_32b: [165106690, 165106688]
reg_dummy_ts_now_16b: [23239, 0]
reg_dummy_ts_lop_16b: [0, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 275] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 58] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 4151040] (LOP before timeout)

reg_debug_resubmit: [27803207, 275]
reg_debug_recirc: [0, 36854642]
reg_debug_late_reply: [9051435, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 11219621] (at RxToR)

reg_debug_eg_cntr1: [0, 9051421] (reply of LOP)
reg_debug_eg_cntr2: [0, 27803221] (reply of INIT)
reg_debug_eg_cntr3: [0, 14133121] (feedback)
reg_debug_eg_cntr4: [0, 4151045] (cwctrl)
reg_debug_eg_cntr5: [0, 4151040] (resume Queue by LOP)


-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        44553031|         2685019|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        44525889|         2687787|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        50879491|         3220786|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        50935818|         3230897|*
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        43495244|         2419074|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        47872184|         2682665|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        49907923|         2797062|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        50035286|         2797292|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        18676010|        95899263|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        12907399|        95416980|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        15527463|        95397322|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        12263328|        95491151|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95899263|        18676010|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95416980|        12907405|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95397322|        15527463|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        95491151|        12263338|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2824768|        48023192|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2804555|        47656239|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2812180|        47783497|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2810306|        47865628|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2807778|        47511411|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2818110|        47872863|*
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2812983|        47535364|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |         2828953|        47957546|*
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        14133121|        14133121|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|       185768310|       185768310|


dev_port:24, queue_id:2, pg_id:3, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:24, queue_id:3, pg_id:3, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:24, queue_id:4, pg_id:3, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1918
dev_port:24, queue_id:5, pg_id:3, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1645
dev_port:24, queue_id:6, pg_id:3, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1750
dev_port:24, queue_id:7, pg_id:3, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1750
dev_port:24, queue_id:8, pg_id:3, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1204
dev_port:24, queue_id:9, pg_id:3, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1687
dev_port:24, queue_id:10, pg_id:3, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1274
dev_port:24, queue_id:11, pg_id:3, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:777
dev_port:24, queue_id:12, pg_id:3, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1519
dev_port:24, queue_id:13, pg_id:3, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:504
dev_port:25, queue_id:2, pg_id:3, pg_queue:18, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:25, queue_id:3, pg_id:3, pg_queue:19, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1617
dev_port:25, queue_id:4, pg_id:3, pg_queue:20, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1575
dev_port:25, queue_id:5, pg_id:3, pg_queue:21, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1533
dev_port:25, queue_id:6, pg_id:3, pg_queue:22, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1841
dev_port:25, queue_id:7, pg_id:3, pg_queue:23, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1288
dev_port:25, queue_id:8, pg_id:3, pg_queue:24, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1470
dev_port:25, queue_id:9, pg_id:3, pg_queue:25, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1624
dev_port:25, queue_id:10, pg_id:3, pg_queue:26, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1743
dev_port:25, queue_id:11, pg_id:3, pg_queue:27, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1526
dev_port:25, queue_id:12, pg_id:3, pg_queue:28, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1096
dev_port:25, queue_id:13, pg_id:3, pg_queue:29, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1283
dev_port:26, queue_id:2, pg_id:3, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2034
dev_port:26, queue_id:3, pg_id:3, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1729
dev_port:26, queue_id:4, pg_id:3, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2079
dev_port:26, queue_id:5, pg_id:3, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2156
dev_port:26, queue_id:6, pg_id:3, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1737
dev_port:26, queue_id:7, pg_id:3, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1988
dev_port:26, queue_id:8, pg_id:3, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1618
dev_port:26, queue_id:9, pg_id:3, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1554
dev_port:26, queue_id:10, pg_id:3, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1176
dev_port:26, queue_id:11, pg_id:3, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1155
dev_port:26, queue_id:12, pg_id:3, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1604
dev_port:26, queue_id:13, pg_id:3, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:994
dev_port:27, queue_id:2, pg_id:3, pg_queue:50, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1856
dev_port:27, queue_id:3, pg_id:3, pg_queue:51, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:27, queue_id:4, pg_id:3, pg_queue:52, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1925
dev_port:27, queue_id:5, pg_id:3, pg_queue:53, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:27, queue_id:6, pg_id:3, pg_queue:54, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:27, queue_id:7, pg_id:3, pg_queue:55, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1701
dev_port:27, queue_id:8, pg_id:3, pg_queue:56, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1656
dev_port:27, queue_id:9, pg_id:3, pg_queue:57, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:27, queue_id:10, pg_id:3, pg_queue:58, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1596
dev_port:27, queue_id:11, pg_id:3, pg_queue:59, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1530
dev_port:27, queue_id:12, pg_id:3, pg_queue:60, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:938
dev_port:27, queue_id:13, pg_id:3, pg_queue:61, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1162
dev_port:32, queue_id:2, pg_id:4, pg_queue:2, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1680
dev_port:32, queue_id:3, pg_id:4, pg_queue:3, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1694
dev_port:32, queue_id:4, pg_id:4, pg_queue:4, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1099
dev_port:32, queue_id:5, pg_id:4, pg_queue:5, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1974
dev_port:32, queue_id:6, pg_id:4, pg_queue:6, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1519
dev_port:32, queue_id:7, pg_id:4, pg_queue:7, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:32, queue_id:8, pg_id:4, pg_queue:8, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:32, queue_id:9, pg_id:4, pg_queue:9, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1162
dev_port:32, queue_id:10, pg_id:4, pg_queue:10, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1248
dev_port:32, queue_id:11, pg_id:4, pg_queue:11, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:954
dev_port:32, queue_id:12, pg_id:4, pg_queue:12, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1516
dev_port:32, queue_id:13, pg_id:4, pg_queue:13, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:967
dev_port:34, queue_id:2, pg_id:4, pg_queue:34, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2058
dev_port:34, queue_id:3, pg_id:4, pg_queue:35, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2167
dev_port:34, queue_id:4, pg_id:4, pg_queue:36, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:34, queue_id:5, pg_id:4, pg_queue:37, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:854
dev_port:34, queue_id:6, pg_id:4, pg_queue:38, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1722
dev_port:34, queue_id:7, pg_id:4, pg_queue:39, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:665
dev_port:34, queue_id:8, pg_id:4, pg_queue:40, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2009
dev_port:34, queue_id:9, pg_id:4, pg_queue:41, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1575
dev_port:34, queue_id:10, pg_id:4, pg_queue:42, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1288
dev_port:34, queue_id:11, pg_id:4, pg_queue:43, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:875
dev_port:34, queue_id:12, pg_id:4, pg_queue:44, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1460
dev_port:34, queue_id:13, pg_id:4, pg_queue:45, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1323
dev_port:36, queue_id:2, pg_id:4, pg_queue:66, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:36, queue_id:3, pg_id:4, pg_queue:67, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1715
dev_port:36, queue_id:4, pg_id:4, pg_queue:68, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:36, queue_id:5, pg_id:4, pg_queue:69, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1862
dev_port:36, queue_id:6, pg_id:4, pg_queue:70, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:679
dev_port:36, queue_id:7, pg_id:4, pg_queue:71, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1540
dev_port:36, queue_id:8, pg_id:4, pg_queue:72, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1540
dev_port:36, queue_id:9, pg_id:4, pg_queue:73, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1376
dev_port:36, queue_id:10, pg_id:4, pg_queue:74, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1562
dev_port:36, queue_id:11, pg_id:4, pg_queue:75, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:896
dev_port:36, queue_id:12, pg_id:4, pg_queue:76, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1519
dev_port:36, queue_id:13, pg_id:4, pg_queue:77, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1337
dev_port:38, queue_id:2, pg_id:4, pg_queue:98, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1799
dev_port:38, queue_id:3, pg_id:4, pg_queue:99, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1593
dev_port:38, queue_id:4, pg_id:4, pg_queue:100, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:38, queue_id:5, pg_id:4, pg_queue:101, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1680
dev_port:38, queue_id:6, pg_id:4, pg_queue:102, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1218
dev_port:38, queue_id:7, pg_id:4, pg_queue:103, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1785
dev_port:38, queue_id:8, pg_id:4, pg_queue:104, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1631
dev_port:38, queue_id:9, pg_id:4, pg_queue:105, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1848
dev_port:38, queue_id:10, pg_id:4, pg_queue:106, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1337
dev_port:38, queue_id:11, pg_id:4, pg_queue:107, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:826
dev_port:38, queue_id:12, pg_id:4, pg_queue:108, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:1292
dev_port:38, queue_id:13, pg_id:4, pg_queue:109, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:2108
dev_port:152, queue_id:0, pg_id:3, pg_queue:0, pipe:1 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:160, queue_id:0, pg_id:4, pg_queue:0, pipe:1 -> drop_count_packets:0, usage_cells:0, watermark_cells:0
dev_port:8, queue_id:0, pg_id:1, pg_queue:0, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:4
dev_port:16, queue_id:0, pg_id:2, pg_queue:0, pipe:0 -> drop_count_packets:0, usage_cells:0, watermark_cells:11

--> absolute flow completion time (total)
Avg:	105.90us
p50:	77.78us
p95:	263.25us
p99:	510.21us
p99.9:	1037.26us
p99.99:	2621.18us
stdev:	139.65

--> absolute flow completion time (short)
Avg:	55.48us
p50:	43.12us
p95:	132.09us
p99:	183.25us
p99.9:	414.25us
p99.99:	1696.68us
stdev:	89.83

--> absolute flow completion time (long)
Avg:	163.52us
p50:	139.71us
p95:	343.73us
p99:	644.21us
p99.9:	1349.73us
p99.99:	3331.65us
stdev:	162.29



```


### ECMP

```python3
python3 analysis.py -d 30 -o solar_80_ecmp

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:7142
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:5960
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:5643
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:6766
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:3206
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:3416
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:3067
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:4118
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:3077
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:3823
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:4190
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:3952
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0

--> absolute flow completion time (total)
Avg:	159.13us
p50:	79.13us
p95:	523.75us
p99:	1072.51us
p99.9:	3023.32us
p99.99:	9715.95us
stdev:	421.71

--> absolute flow completion time (short)
Avg:	62.52us
p50:	38.14us
p95:	163.48us
p99:	268.45us
p99.9:	755.85us
p99.99:	5750.14us
stdev:	324.01

--> absolute flow completion time (long)
Avg:	269.48us
p50:	190.30us
p95:	799.72us
p99:	1393.04us
p99.9:	3559.49us
p99.99:	14253.07us
stdev:	488.07



```

### LETFLOW

```python3
python3 analysis.py -d 30 -o solar_80_letflow

-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:5571
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:4043
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:6717
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:4324
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:2928
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:3114
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:2789
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:2776
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:3955
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:2964
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:3738
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:2969
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0


--> absolute flow completion time (total)
Avg:	223.15us
p50:	96.99us
p95:	949.49us
p99:	1506.00us
p99.9:	4089.47us
p99.99:	10771.25us
stdev:	506.96

--> absolute flow completion time (short)
Avg:	75.68us
p50:	46.29us
p95:	237.63us
p99:	375.96us
p99.9:	740.52us
p99.99:	5712.13us
stdev:	304.44

--> absolute flow completion time (long)
Avg:	391.61us
p50:	221.12us
p95:	1129.62us
p99:	2650.06us
p99.9:	4520.45us
p99.99:	17179.77us
stdev:	625.70


```


### DRILL

```python3
python3 analysis.py -d 30 -o solar_80_drill


-------- EGRESS PORT STATUS --------
dev_port:168 ->drop_count_packets;0, usage_cells:0, watermark_cells:4103
dev_port:176 ->drop_count_packets;0, usage_cells:0, watermark_cells:3932
dev_port:184 ->drop_count_packets;0, usage_cells:0, watermark_cells:3996
dev_port:192 ->drop_count_packets;0, usage_cells:0, watermark_cells:3913
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
dev_port:24 ->drop_count_packets;0, usage_cells:0, watermark_cells:609
dev_port:25 ->drop_count_packets;0, usage_cells:0, watermark_cells:826
dev_port:26 ->drop_count_packets;0, usage_cells:0, watermark_cells:1085
dev_port:27 ->drop_count_packets;0, usage_cells:0, watermark_cells:351
dev_port:32 ->drop_count_packets;0, usage_cells:0, watermark_cells:603
dev_port:34 ->drop_count_packets;0, usage_cells:0, watermark_cells:578
dev_port:36 ->drop_count_packets;0, usage_cells:0, watermark_cells:1058
dev_port:38 ->drop_count_packets;0, usage_cells:0, watermark_cells:816
dev_port:152 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:160 ->drop_count_packets;29580, usage_cells:0, watermark_cells:36593
dev_port:8 ->drop_count_packets;0, usage_cells:0, watermark_cells:0
dev_port:16 ->drop_count_packets;0, usage_cells:0, watermark_cells:0


--> absolute flow completion time (total)
Avg:	596.29us
p50:	245.34us
p95:	1723.80us
p99:	4164.85us
p99.9:	6170.33us
p99.99:	7505.49us
stdev:	800.32

--> absolute flow completion time (short)
Avg:	154.77us
p50:	73.34us
p95:	456.13us
p99:	990.25us
p99.9:	2121.83us
p99.99:	3264.04us
stdev:	206.89

--> absolute flow completion time (long)
Avg:	1101.30us
p50:	1065.33us
p95:	2934.39us
p99:	4872.74us
p99.9:	6651.39us
p99.99:	7913.80us
stdev:	919.86



FAILED: Src:10.4.4.6 -> Dst:10.4.4.16		#.failed msgs:24061, failed goodput:0.259 Gbps, failed throughput:0.274 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:9780, failed goodput:0.104 Gbps, failed throughput:0.110 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.15		#.failed msgs:5902, failed goodput:0.063 Gbps, failed throughput:0.066 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:4947, failed goodput:0.053 Gbps, failed throughput:0.056 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.12		#.failed msgs:21562, failed goodput:0.233 Gbps, failed throughput:0.247 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.16		#.failed msgs:17343, failed goodput:0.187 Gbps, failed throughput:0.197 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:12115, failed goodput:0.132 Gbps, failed throughput:0.140 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.14		#.failed msgs:2547, failed goodput:0.027 Gbps, failed throughput:0.029 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.11		#.failed msgs:873, failed goodput:0.010 Gbps, failed throughput:0.010 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.16		#.failed msgs:26084, failed goodput:0.282 Gbps, failed throughput:0.298 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:10422, failed goodput:0.112 Gbps, failed throughput:0.119 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:4815, failed goodput:0.052 Gbps, failed throughput:0.055 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.15		#.failed msgs:4521, failed goodput:0.048 Gbps, failed throughput:0.051 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:360, failed goodput:0.004 Gbps, failed throughput:0.004 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.15		#.failed msgs:16105, failed goodput:0.176 Gbps, failed throughput:0.186 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.16		#.failed msgs:11211, failed goodput:0.120 Gbps, failed throughput:0.127 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.10		#.failed msgs:5141, failed goodput:0.056 Gbps, failed throughput:0.059 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.9		#.failed msgs:2030, failed goodput:0.021 Gbps, failed throughput:0.023 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.11		#.failed msgs:979, failed goodput:0.011 Gbps, failed throughput:0.011 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.12		#.failed msgs:349, failed goodput:0.005 Gbps, failed throughput:0.005 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.14		#.failed msgs:206, failed goodput:0.003 Gbps, failed throughput:0.003 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.16		#.failed msgs:24186, failed goodput:0.264 Gbps, failed throughput:0.280 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.12		#.failed msgs:20232, failed goodput:0.218 Gbps, failed throughput:0.231 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:16271, failed goodput:0.174 Gbps, failed throughput:0.184 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:14751, failed goodput:0.160 Gbps, failed throughput:0.169 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:10949, failed goodput:0.118 Gbps, failed throughput:0.124 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.13		#.failed msgs:10500, failed goodput:0.112 Gbps, failed throughput:0.118 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.15		#.failed msgs:8545, failed goodput:0.094 Gbps, failed throughput:0.099 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.11		#.failed msgs:2757, failed goodput:0.030 Gbps, failed throughput:0.032 Gbps



```



