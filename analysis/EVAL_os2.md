# ConWeave Experiment Results

Message Queue size = 16K. Over than that, it would drop the message (failure)

## File title format (Leaf Switch)

{LB Mode}_{oversubscription}_{Workload}_{Load}_{DCQCN (e.g., 50-200-0.2)}_{PFC (e.g., 400-40)}_{Time (e.g., 300 seconds)}_{Parameter (e.g., deadline for ConWeave, or timegap for flowlets)}

## Logging with Over-subscription 2:1 & Solar2022

### Loggings for Over-Subscription 2:1 & Solar2022 & Load 50 & DCQCN 50-200-0.2 & PFC 400-40
`./run_evaluation.sh Solar2022 0.25 320`

* Conweave Solar2022, 0.5, 50-200-0.2, 600-40, 300 seconds, 12us deadline

```python3
python3 analysis.py -d 300 -o conweave_2_solar_0.5_50-200-0.2_600-40_300_12

--> absolute flow completion time (total)
Avg:	271.30us
p50:	71.67us
p95:	1658.85us
p99:	3079.34us
p99.9:	5439.11us
p99.99:	8130.65us
stdev:	636.55

--> absolute flow completion time (short)
Avg:	113.46us
p50:	32.33us
p95:	247.37us
p99:	1856.84us
p99.9:	3191.20us
p99.99:	5031.34us
stdev:	357.06

--> absolute flow completion time (long)
Avg:	452.63us
p50:	142.34us
p95:	2220.94us
p99:	3762.14us
p99.9:	6246.98us
p99.99:	9136.40us
stdev:	814.08


Rx (debug)#newOoO: 13231303
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [2951384321, 0]
reg_dummy_ts_now_32b: [520541834, 520541831]
reg_dummy_ts_now_16b: [13828, 0]
reg_dummy_ts_lop_16b: [0, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 2553] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 657] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 13228749] (LOP before timeout)

reg_debug_resubmit: [209900892, 2553]
reg_debug_recirc: [0, 234776765]
reg_debug_late_reply: [24875873, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 20292960] (at RxToR)

reg_debug_eg_cntr1: [0, 24875846] (reply of LOP)
reg_debug_eg_cntr2: [0, 209900919] (reply of INIT)
reg_debug_eg_cntr3: [0, 69705312] (feedback)
reg_debug_eg_cntr4: [0, 13228750] (cwctrl)
reg_debug_eg_cntr5: [0, 13228749] (resume Queue by LOP)

-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       307927128|        15594979|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       313255657|        15760291|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       337183531|        16789625|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       342114892|        17057616|
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       279252204|        13072090|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       292327396|        13907192|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       317363258|        14550558|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       317724863|        14953334|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        44365253|       641086857|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        91490244|       625686920|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        96291922|       594429087|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       124314519|       645946033|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       641086857|        44365309|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       625686920|        91490468|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       594429087|        96291960|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       645946033|       124314555|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30310334|       626572388|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30513742|       624802085|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30483831|       625379197|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        30377298|       630395483|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        69705312|        69705312|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|       670105783|       670105783|

```

* ECMP solar2022, 0.5, 50-200-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o ecmp_2_solar_0.5_50-200-0.2_600-40_300

--> absolute flow completion time (total)
Avg:	265.47us
p50:	76.97us
p95:	1566.72us
p99:	3114.07us
p99.9:	5739.12us
p99.99:	9199.05us
stdev:	645.94

--> absolute flow completion time (short)
Avg:	89.93us
p50:	30.42us
p95:	132.19us
p99:	1608.62us
p99.9:	2968.87us
p99.99:	4836.51us
stdev:	325.19

--> absolute flow completion time (long)
Avg:	467.13us
p50:	179.50us
p95:	2163.23us
p99:	3862.18us
p99.9:	6627.59us
p99.99:	10666.59us
stdev:	836.07

```


* Letflow Solar2022, 0.5, 50-200-0.2, 600-40, 300 seconds, 100us timegap

```python3
python3 analysis.py -d 300 -o letflow_2_solar_0.5_50-200-0.2_600-40_300_100

--> absolute flow completion time (total)	
Avg:	259.76us
p50:	76.00us
p95:	1491.93us
p99:	2985.30us
p99.9:	5428.68us
p99.99:	8970.47us
stdev:	620.97
	
--> absolute flow completion time (short)	
Avg:	87.14us
p50:	29.92us
p95:	142.75us
p99:	1548.27us
p99.9:	2860.36us
p99.99:	4886.40us
stdev:	321.74
	
--> absolute flow completion time (long)	
Avg:	458.06us
p50:	207.59us
p95:	1989.10us
p99:	3590.87us
p99.9:	6314.85us
p99.99:	10539.51us
stdev:	797.56

```


* DRILL Solar2022, 0.5, 50-200-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o drill_2_solar_0.5_50-200-0.2_600-40_300

```




### Loggings for Over-Subscription 2:1 & Solar2022 & Load 80
`./run_evaluation.sh Solar2022 0.4 320`

* Conweave Solar2022, 0.8, 50-200-0.2, 600-40, 300 seconds, 12us deadline

```python3
python3 analysis.py -d 300 -o conweave_2_solar_0.8_50-200-0.2_600-40_300_12

--> absolute flow completion time (total)
Avg:	337.79us
p50:	108.83us
p95:	1829.98us
p99:	3319.49us
p99.9:	5633.74us
p99.99:	8612.37us
stdev:	675.95

--> absolute flow completion time (short)
Avg:	114.70us
p50:	43.17us
p95:	200.00us
p99:	1890.28us
p99.9:	3307.19us
p99.99:	4990.80us
stdev:	340.17

--> absolute flow completion time (long)
Avg:	594.19us
p50:	263.73us
p95:	2396.64us
p99:	3951.08us
p99.9:	6523.90us
p99.99:	9681.39us
stdev:	852.11



FAILED: Src:10.4.4.6 -> Dst:10.4.4.9		#.failed msgs:805140, failed goodput:0.868 Gbps, failed throughput:0.919 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:810041, failed goodput:0.872 Gbps, failed throughput:0.923 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:426302, failed goodput:0.459 Gbps, failed throughput:0.486 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.14		#.failed msgs:37910, failed goodput:0.040 Gbps, failed throughput:0.043 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.9		#.failed msgs:618915, failed goodput:0.667 Gbps, failed throughput:0.706 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:529336, failed goodput:0.569 Gbps, failed throughput:0.603 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.13		#.failed msgs:618686, failed goodput:0.665 Gbps, failed throughput:0.704 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.14		#.failed msgs:399536, failed goodput:0.430 Gbps, failed throughput:0.455 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.9		#.failed msgs:142064, failed goodput:0.153 Gbps, failed throughput:0.161 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.10		#.failed msgs:69373, failed goodput:0.075 Gbps, failed throughput:0.079 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.13		#.failed msgs:81861, failed goodput:0.087 Gbps, failed throughput:0.093 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.14		#.failed msgs:4759, failed goodput:0.005 Gbps, failed throughput:0.005 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.13		#.failed msgs:128039, failed goodput:0.138 Gbps, failed throughput:0.146 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.10		#.failed msgs:62933, failed goodput:0.068 Gbps, failed throughput:0.072 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.9		#.failed msgs:10198, failed goodput:0.011 Gbps, failed throughput:0.012 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:741788, failed goodput:0.797 Gbps, failed throughput:0.844 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:506747, failed goodput:0.546 Gbps, failed throughput:0.578 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:461738, failed goodput:0.497 Gbps, failed throughput:0.527 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:311067, failed goodput:0.335 Gbps, failed throughput:0.354 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.14		#.failed msgs:594787, failed goodput:0.639 Gbps, failed throughput:0.676 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.9		#.failed msgs:597754, failed goodput:0.643 Gbps, failed throughput:0.680 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.13		#.failed msgs:641800, failed goodput:0.691 Gbps, failed throughput:0.731 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.10		#.failed msgs:423397, failed goodput:0.455 Gbps, failed throughput:0.482 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.14		#.failed msgs:189234, failed goodput:0.203 Gbps, failed throughput:0.215 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.13		#.failed msgs:242577, failed goodput:0.262 Gbps, failed throughput:0.277 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.10		#.failed msgs:102878, failed goodput:0.111 Gbps, failed throughput:0.117 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.9		#.failed msgs:172123, failed goodput:0.186 Gbps, failed throughput:0.196 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:316543, failed goodput:0.340 Gbps, failed throughput:0.360 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.13		#.failed msgs:145030, failed goodput:0.157 Gbps, failed throughput:0.166 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:57663, failed goodput:0.062 Gbps, failed throughput:0.066 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:7012, failed goodput:0.008 Gbps, failed throughput:0.008 Gbps



Rx (debug)#newOoO: 21930098
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [3659630133, 0]
reg_dummy_ts_now_32b: [457891032, 457891030]
reg_dummy_ts_now_16b: [56609, 0]
reg_dummy_ts_lop_16b: [10847, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 3224] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 783] (reorder but no queue)
reg_debug_must_be_zero_5: [1, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 21926856] (LOP before timeout)

reg_debug_resubmit: [366416119, 3224]
reg_debug_recirc: [0, 407368173]
reg_debug_late_reply: [40952054, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 15247644] (at RxToR)

reg_debug_eg_cntr1: [0, 40952181] (reply of LOP)
reg_debug_eg_cntr2: [0, 366416000] (reply of INIT)
reg_debug_eg_cntr3: [0, 73606291] (feedback)
reg_debug_eg_cntr4: [0, 21926874] (cwctrl)
reg_debug_eg_cntr5: [0, 21926856] (resume Queue by LOP)



-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       365146926|        19782061|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       299537194|        17697132|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       395120707|        21639447|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       384812266|        21170598|*
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       343032740|        16934023|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       358449528|        16876650|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       368121927|        18699817|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       372170035|        18736076|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       123099444|       758487618|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       127085683|       698470626|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       150904842|       707843914|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       157813496|       721587657|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       758487618|       123099444|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       698470626|       127085683|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       707843914|       150904842|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       721587657|       157813496|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        36744205|       718658351|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        38751726|       724354795|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        37064896|       719522298|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        38974505|       723854627|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        73606291|        73606291|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|       861498237|       861498237|




```

* ECMP solar2022, 0.8, 50-200-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o ecmp_2_solar_0.8_50-200-0.2_600-40_300

--> absolute flow completion time (total)
Avg:	235.36us
p50:	114.79us
p95:	748.54us
p99:	2020.72us
p99.9:	3904.59us
p99.99:	6546.35us
stdev:	393.67

--> absolute flow completion time (short)
Avg:	68.67us
p50:	42.86us
p95:	151.01us
p99:	352.38us
p99.9:	2025.35us
p99.99:	3393.86us
stdev:	153.46

--> absolute flow completion time (long)
Avg:	426.90us
p50:	279.64us
p95:	1432.88us
p99:	2491.05us
p99.9:	4726.12us
p99.99:	7424.65us
stdev:	487.22




FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:646477, failed goodput:0.696 Gbps, failed throughput:0.736 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:1277587, failed goodput:1.375 Gbps, failed throughput:1.455 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.14		#.failed msgs:861696, failed goodput:0.926 Gbps, failed throughput:0.980 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.9		#.failed msgs:31924, failed goodput:0.034 Gbps, failed throughput:0.036 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.10		#.failed msgs:18752, failed goodput:0.020 Gbps, failed throughput:0.021 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.9		#.failed msgs:402437, failed goodput:0.433 Gbps, failed throughput:0.459 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.13		#.failed msgs:514322, failed goodput:0.551 Gbps, failed throughput:0.584 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.10		#.failed msgs:294112, failed goodput:0.316 Gbps, failed throughput:0.335 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:224497, failed goodput:0.241 Gbps, failed throughput:0.255 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:40020, failed goodput:0.043 Gbps, failed throughput:0.046 Gbps


```


* Letflow Solar2022, 0.8, 50-200-0.2, 600-40, 300 seconds, 100us timegap

```python3
python3 analysis.py -d 300 -o letflow_2_solar_0.8_50-200-0.2_600-40_300_100

--> absolute flow completion time (total)
Avg:	365.03us
p50:	119.46us
p95:	1957.39us
p99:	3509.20us
p99.9:	5991.77us
p99.99:	8922.84us
stdev:	728.93

--> absolute flow completion time (short)
Avg:	115.12us
p50:	42.62us
p95:	192.18us
p99:	1965.56us
p99.9:	3315.68us
p99.99:	4750.27us
stdev:	348.04

--> absolute flow completion time (long)
Avg:	652.20us
p50:	267.17us
p95:	2619.62us
p99:	4166.92us
p99.9:	6911.16us
p99.99:	9947.32us
stdev:	921.12



FAILED: Src:10.4.4.6 -> Dst:10.4.4.9		#.failed msgs:987183, failed goodput:1.062 Gbps, failed throughput:1.125 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.10		#.failed msgs:727406, failed goodput:0.782 Gbps, failed throughput:0.828 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:635445, failed goodput:0.684 Gbps, failed throughput:0.724 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.14		#.failed msgs:449110, failed goodput:0.482 Gbps, failed throughput:0.511 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.9		#.failed msgs:780618, failed goodput:0.841 Gbps, failed throughput:0.890 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:704409, failed goodput:0.758 Gbps, failed throughput:0.803 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.13		#.failed msgs:431595, failed goodput:0.465 Gbps, failed throughput:0.493 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.14		#.failed msgs:43584, failed goodput:0.047 Gbps, failed throughput:0.050 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.13		#.failed msgs:311561, failed goodput:0.334 Gbps, failed throughput:0.354 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.9		#.failed msgs:280851, failed goodput:0.301 Gbps, failed throughput:0.319 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.14		#.failed msgs:123733, failed goodput:0.133 Gbps, failed throughput:0.141 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.10		#.failed msgs:59279, failed goodput:0.064 Gbps, failed throughput:0.067 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.9		#.failed msgs:445455, failed goodput:0.480 Gbps, failed throughput:0.508 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.10		#.failed msgs:325112, failed goodput:0.349 Gbps, failed throughput:0.370 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.13		#.failed msgs:235285, failed goodput:0.254 Gbps, failed throughput:0.268 Gbps
FAILED: Src:10.4.4.2 -> Dst:10.4.4.14		#.failed msgs:11637, failed goodput:0.012 Gbps, failed throughput:0.013 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:990790, failed goodput:1.065 Gbps, failed throughput:1.127 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.9		#.failed msgs:780871, failed goodput:0.840 Gbps, failed throughput:0.889 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:608921, failed goodput:0.655 Gbps, failed throughput:0.693 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.14		#.failed msgs:652570, failed goodput:0.702 Gbps, failed throughput:0.744 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.10		#.failed msgs:499766, failed goodput:0.538 Gbps, failed throughput:0.570 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.9		#.failed msgs:547246, failed goodput:0.589 Gbps, failed throughput:0.623 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.14		#.failed msgs:116336, failed goodput:0.124 Gbps, failed throughput:0.132 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.13		#.failed msgs:13210, failed goodput:0.014 Gbps, failed throughput:0.015 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.10		#.failed msgs:556769, failed goodput:0.599 Gbps, failed throughput:0.634 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.9		#.failed msgs:160106, failed goodput:0.173 Gbps, failed throughput:0.183 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.13		#.failed msgs:13364, failed goodput:0.014 Gbps, failed throughput:0.015 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.9		#.failed msgs:299612, failed goodput:0.323 Gbps, failed throughput:0.341 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.10		#.failed msgs:247061, failed goodput:0.265 Gbps, failed throughput:0.281 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.13		#.failed msgs:231768, failed goodput:0.249 Gbps, failed throughput:0.264 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:17386, failed goodput:0.019 Gbps, failed throughput:0.020 Gbps


```


* DRILL Solar2022, 0.8, 50-200-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o drill_2_solar_0.8_50-200-0.2_600-40_300

```







### Loggings for Over-Subscription 2:1 & Solar2022 & Load 80
`./run_evaluation.sh Solar2022 0.25 320`

* Conweave Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds, 12us deadline
```python3
python3 analysis.py -d 300 -o conweave_2_solar_0.8_100-400-0.2_600-40_300_12


--> absolute flow completion time (total)
Avg:	226.03us
p50:	119.38us
p95:	740.97us
p99:	1987.19us
p99.9:	3476.14us
p99.99:	5466.50us
stdev:	359.60

--> absolute flow completion time (short)
Avg:	87.42us
p50:	58.27us
p95:	167.00us
p99:	1154.10us
p99.9:	2117.35us
p99.99:	4042.64us
stdev:	183.78

--> absolute flow completion time (long)
Avg:	385.31us
p50:	270.57us
p95:	1226.63us
p99:	2253.02us
p99.9:	3984.94us
p99.99:	6099.90us
stdev:	437.79


FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:176338, failed goodput:0.190 Gbps, failed throughput:0.201 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.13		#.failed msgs:46880, failed goodput:0.050 Gbps, failed throughput:0.053 Gbps
FAILED: Src:10.4.4.4 -> Dst:10.4.4.10		#.failed msgs:13867, failed goodput:0.015 Gbps, failed throughput:0.016 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.13		#.failed msgs:44904, failed goodput:0.049 Gbps, failed throughput:0.051 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.13		#.failed msgs:164230, failed goodput:0.177 Gbps, failed throughput:0.187 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.9		#.failed msgs:29633, failed goodput:0.032 Gbps, failed throughput:0.034 Gbps




Rx (debug)#newOoO: 29471615
--> status for reorder processing <--
Rx Ingress Enqueue Cntr: [0, 0]
Rx Egress Dequeue: 0 (idx: 61)
============     D E B U G G I N G    ============
reg_conweave_switch: [1, 1]
reg_debug_tx_data_matched_pkts: [3735717398, 0]
reg_dummy_ts_now_32b: [452646088, 452646086]
reg_dummy_ts_now_16b: [27238, 0]
reg_dummy_ts_lop_16b: [20001, 0]

reg_debug_must_be_zero_0: [0, 0] (Ctrl unmatch at TxToR)
reg_debug_must_be_zero_1: [0, 0] (New round but reorder unfinished)
reg_debug_must_be_zero_2: [0, 3617] (NewOoO failed to find empty-Q)
reg_debug_must_be_zero_3: [0, 0] (NewOoO but prev timer exists)
reg_debug_must_be_zero_4: [0, 737] (reorder but no queue)
reg_debug_must_be_zero_5: [0, 0] (something went wrong)
reg_debug_must_be_zero_6: [0, 29467998] (LOP before timeout)

reg_debug_resubmit: [241655143, 3617]
reg_debug_recirc: [0, 303313835]
reg_debug_late_reply: [61658692, 0] (at TxToR)
reg_debug_rx_prev_round: [0, 39331101] (at RxToR)

reg_debug_eg_cntr1: [0, 61658694] (reply of LOP)
reg_debug_eg_cntr2: [0, 241655176] (reply of INIT)
reg_debug_eg_cntr3: [0, 41837869] (feedback)
reg_debug_eg_cntr4: [0, 29467998] (cwctrl)
reg_debug_eg_cntr5: [0, 29467998] (resume Queue by LOP)


-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
PORT |MAC |D_P|P/PT|SPEED  |FEC |AN|KR|RDY|ADM|OPR|LPBK    |FRAMES RX       |FRAMES TX       |E
-----+----+---+----+-------+----+--+--+---+---+---+--------+----------------+----------------+-
1/0  | 9/0|136|1/ 8|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       365403370|        16575858|
1/1  | 9/1|137|1/ 9|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       322789337|        14588828|
1/2  | 9/2|138|1/10|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       445966562|        19346183|
1/3  | 9/3|139|1/11|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       435460924|        19082591|
2/0  |10/0|144|1/16|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       382482117|        16819583|
2/2  |10/2|146|1/18|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       395513154|        17017113|
2/4  |10/4|148|1/20|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       429258029|        18123137|
2/6  |10/6|150|1/22|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       414478002|        17781234|
3/0  |11/0|152|1/24|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
4/0  |12/0|160|1/32|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|               0|               0|
5/0  |13/0|168|1/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        78775519|       761322564|
6/0  |14/0|176|1/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        98830512|       775995544|
7/0  |15/0|184|1/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       103928675|       819781520|
8/0  |16/0|192|1/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       161113144|       834251835|
25/0 | 7/0| 56|0/56|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       761322564|        78775537|
26/0 | 8/0| 64|0/64|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       775995544|        98830546|
27/0 | 5/0| 40|0/40|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       819781520|       103928675|
28/0 | 6/0| 48|0/48|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |       834251835|       161113170|
29/0 | 3/0| 24|0/24|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        35120256|       800789653|
29/1 | 3/1| 25|0/25|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        34260431|       790067945|
29/2 | 3/2| 26|0/26|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        35035298|       806061931|
29/3 | 3/3| 27|0/27|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |        34918097|       794432190|
30/0 | 4/0| 32|0/32|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/2 | 4/2| 34|0/34|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/4 | 4/4| 36|0/36|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
30/6 | 4/6| 38|0/38|25G    |NONE|Ds|Au|YES|ENB|UP |  NONE  |               0|              64|
31/0 | 1/0|  8|0/ 8|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|        41837869|        41837869|
32/0 | 2/0| 16|0/16|400G   | RS |Ds|Au|YES|ENB|UP |Mac-near|      1465587454|      1465587454|

```

* ECMP solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o ecmp_2_solar_0.8_100-400-0.2_600-40_300

--> absolute flow completion time (total)
Avg:	216.81us
p50:	111.17us
p95:	613.90us
p99:	1583.56us
p99.9:	2737.74us
p99.99:	5220.94us
stdev:	368.72

--> absolute flow completion time (short)
Avg:	73.28us
p50:	54.38us
p95:	167.58us
p99:	260.64us
p99.9:	1584.47us
p99.99:	3282.41us
stdev:	212.31

--> absolute flow completion time (long)
Avg:	381.74us
p50:	323.15us
p95:	999.20us
p99:	1938.52us
p99.9:	3236.91us
p99.99:	6350.07us
stdev:	435.32



FAILED: Src:10.4.4.6 -> Dst:10.4.4.14		#.failed msgs:757392, failed goodput:0.814 Gbps, failed throughput:0.862 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.9		#.failed msgs:151246, failed goodput:0.162 Gbps, failed throughput:0.172 Gbps
FAILED: Src:10.4.4.6 -> Dst:10.4.4.13		#.failed msgs:40236, failed goodput:0.043 Gbps, failed throughput:0.046 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.13		#.failed msgs:571926, failed goodput:0.617 Gbps, failed throughput:0.653 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.10		#.failed msgs:51008, failed goodput:0.055 Gbps, failed throughput:0.058 Gbps
FAILED: Src:10.4.4.8 -> Dst:10.4.4.9		#.failed msgs:10202, failed goodput:0.011 Gbps, failed throughput:0.011 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.13		#.failed msgs:413858, failed goodput:0.446 Gbps, failed throughput:0.472 Gbps
FAILED: Src:10.4.4.5 -> Dst:10.4.4.10		#.failed msgs:255375, failed goodput:0.274 Gbps, failed throughput:0.290 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.14		#.failed msgs:879233, failed goodput:0.945 Gbps, failed throughput:1.000 Gbps
FAILED: Src:10.4.4.7 -> Dst:10.4.4.10		#.failed msgs:761570, failed goodput:0.818 Gbps, failed throughput:0.866 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.13		#.failed msgs:408124, failed goodput:0.438 Gbps, failed throughput:0.464 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.9		#.failed msgs:165759, failed goodput:0.178 Gbps, failed throughput:0.189 Gbps
FAILED: Src:10.4.4.3 -> Dst:10.4.4.10		#.failed msgs:8223, failed goodput:0.009 Gbps, failed throughput:0.009 Gbps
FAILED: Src:10.4.4.1 -> Dst:10.4.4.14		#.failed msgs:66553, failed goodput:0.072 Gbps, failed throughput:0.076 Gbps


```

* Letflow Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds, 100us timegap

```python3
python3 analysis.py -d 300 -o letflow_2_solar_0.8_100-400-0.2_600-40_300_100


```


* DRILL Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o drill_2_solar_0.8_100-400-0.2_600-40_300


```



<!-- 

### Loggings for Over-Subscription 2:1 & Solar2022 & Load 80
`./run_evaluation.sh Solar2022 0.25 320`

* Conweave Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds, 12us deadline
```python3
python3 analysis.py -d 300 -o conweave_2_solar_0.8_100-400-0.2_600-40_300_12

```

* ECMP solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o ecmp_2_solar_0.8_100-400-0.2_600-40_300


```

* Letflow Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds, 100us timegap

```python3
python3 analysis.py -d 300 -o letflow_2_solar_0.8_100-400-0.2_600-40_300_100


```


* DRILL Solar2022, 0.8, 100-400-0.2, 600-40, 300 seconds

```python3
python3 analysis.py -d 300 -o drill_2_solar_0.8_100-400-0.2_600-40_300


``` -->


