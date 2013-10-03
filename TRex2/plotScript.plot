set terminal postscript enhance color font 20
#set terminal postscript enhance font 20
set pointsize 2
set line style 1 linewidth 3
set line style 2 linewidth 3
set line style 3 linewidth 3
set line style 4 linewidth 3
set line style 5 linewidth 3
set xlabel "Input Rate (k events/s)"
set ylabel "Throughput (events/s)"
set key left top
set format y "%7.0f"

# Length Test Case (Each)

set auto y
set auto x
set output "Graphs/LengthEachThroughput.ps"
plot "Avg/TRex_LengthEach_throughput_1" notitle with lines ls 1, "Avg/TRex_LengthEach_throughput_1" title "2 States" with yerrorbars ls 1, "Avg/TRex_LengthEach_throughput_2" notitle with lines ls 2, "Avg/TRex_LengthEach_throughput_2" title "3 States" with yerrorbars ls 2, "Avg/TRex_LengthEach_throughput_3" notitle with lines ls 3, "Avg/TRex_LengthEach_throughput_3" title "4 States" with yerrorbars ls 3, "Avg/TRex_LengthEach_throughput_4" notitle with lines ls 4, "Avg/TRex_LengthEach_throughput_4" title "5 States" with yerrorbars ls 4

# Length Test Case (Last)

set auto y
set auto x
set output "Graphs/LengthLastThroughput.ps"
plot "Avg/TRex_LengthLast_throughput_1" notitle with lines ls 1, "Avg/TRex_LengthLast_throughput_1" title "2 States" with yerrorbars ls 1, "Avg/TRex_LengthLast_throughput_2" notitle with lines ls 2, "Avg/TRex_LengthLast_throughput_2" title "3 States" with yerrorbars ls 2, "Avg/TRex_LengthLast_throughput_3" notitle with lines ls 3, "Avg/TRex_LengthLast_throughput_3" title "4 States" with yerrorbars ls 3, "Avg/TRex_LengthLast_throughput_4" notitle with lines ls 4, "Avg/TRex_LengthLast_throughput_4" title "5 States" with yerrorbars ls 4

# Length Test Case (First)

set auto y
set auto x
set output "Graphs/LengthFirstThroughput.ps"
plot "Avg/TRex_LengthFirst_throughput_1" notitle with lines ls 1, "Avg/TRex_LengthFirst_throughput_1" title "2 States" with yerrorbars ls 1, "Avg/TRex_LengthFirst_throughput_2" notitle with lines ls 2, "Avg/TRex_LengthFirst_throughput_2" title "3 States" with yerrorbars ls 2, "Avg/TRex_LengthFirst_throughput_3" notitle with lines ls 3, "Avg/TRex_LengthFirst_throughput_3" title "4 States" with yerrorbars ls 3, "Avg/TRex_LengthFirst_throughput_4" notitle with lines ls 4, "Avg/TRex_LengthFirst_throughput_4" title "5 States" with yerrorbars ls 4

# Width Test Case (Each)

set yr [0:1300000]
set auto x
set output "Graphs/WidthEachThroughput.ps"
plot "Avg/TRex_WidthEach_throughput_1" notitle with lines ls 1, "Avg/TRex_WidthEach_throughput_1" title "2 Sequences" with yerrorbars ls 1, "Avg/TRex_WidthEach_throughput_2" notitle with lines ls 2, "Avg/TRex_WidthEach_throughput_2" title "3 Sequences" with yerrorbars ls 2, "Avg/TRex_WidthEach_throughput_3" notitle with lines ls 3, "Avg/TRex_WidthEach_throughput_3" title "4 Sequences" with yerrorbars ls 3, "Avg/TRex_WidthEach_throughput_4" notitle with lines ls 4, "Avg/TRex_WidthEach_throughput_4" title "5 Sequences" with yerrorbars ls 4 

# Width Test Case (Last)

set auto y
set auto x
set output "Graphs/WidthLastThroughput.ps"
plot "Avg/TRex_WidthLast_throughput_1" notitle with lines ls 1, "Avg/TRex_WidthLast_throughput_1" title "2 Sequences" with yerrorbars ls 1, "Avg/TRex_WidthLast_throughput_2" notitle with lines ls 2, "Avg/TRex_WidthLast_throughput_2" title "3 Sequences" with yerrorbars ls 2, "Avg/TRex_WidthLast_throughput_3" notitle with lines ls 3, "Avg/TRex_WidthLast_throughput_3" title "4 Sequences" with yerrorbars ls 3, "Avg/TRex_WidthLast_throughput_4" notitle with lines ls 4, "Avg/TRex_WidthLast_throughput_4" title "5 Sequences" with yerrorbars ls 4

# NumRules Test Case (Each)

set yr[0:600000]
set auto x
set output "Graphs/NumRulesEachThroughput.ps"
plot "Avg/TRex_NumRulesEach_throughput_1" notitle with lines ls 1, "Avg/TRex_NumRulesEach_throughput_1" title "1000 Rules" with yerrorbars ls 1, "Avg/TRex_NumRulesEach_throughput_2" notitle with lines ls 2, "Avg/TRex_NumRulesEach_throughput_2" title "3000 Rules" with yerrorbars ls 2, "Avg/TRex_NumRulesEach_throughput_3" notitle with lines ls 3, "Avg/TRex_NumRulesEach_throughput_3" title "5000 Rules" with yerrorbars ls 3, "Avg/TRex_NumRulesEach_throughput_4" notitle with lines ls 4, "Avg/TRex_NumRulesEach_throughput_4" title "7000 Rules" with yerrorbars ls 4, "Avg/TRex_NumRulesEach_throughput_5" notitle with lines ls 5, "Avg/TRex_NumRulesEach_throughput_5" title "9000 Rules" with yerrorbars ls 5

# NumRules Test Case (Last)

set auto y
set auto x
set output "Graphs/NumRulesLastThroughput.ps"
plot "Avg/TRex_NumRulesLast_throughput_1" notitle with lines ls 1, "Avg/TRex_NumRulesLast_throughput_1" title "1000 Rules" with yerrorbars ls 1, "Avg/TRex_NumRulesLast_throughput_2" notitle with lines ls 2, "Avg/TRex_NumRulesLast_throughput_2" title "3000 Rules" with yerrorbars ls 2, "Avg/TRex_NumRulesLast_throughput_3" notitle with lines ls 3, "Avg/TRex_NumRulesLast_throughput_3" title "5000 Rules" with yerrorbars ls 3, "Avg/TRex_NumRulesLast_throughput_4" notitle with lines ls 4, "Avg/TRex_NumRulesLast_throughput_4" title "7000 Rules" with yerrorbars ls 4, "Avg/TRex_NumRulesLast_throughput_5" notitle with lines ls 5, "Avg/TRex_NumRulesLast_throughput_5" title "9000 Rules" with yerrorbars ls 5

# Win Test Case (Each)

set auto y
set xr[0:5.2]
set output "Graphs/WinEachThroughput.ps"
plot "Avg/TRex_WinEach_throughput_1" notitle with lines ls 1, "Avg/TRex_WinEach_throughput_1" title "2 s" with yerrorbars ls 1, "Avg/TRex_WinEach_throughput_2" notitle with lines ls 2, "Avg/TRex_WinEach_throughput_2" title "6 s" with yerrorbars ls 2, "Avg/TRex_WinEach_throughput_3" notitle with lines ls 3, "Avg/TRex_WinEach_throughput_3" title "10 s" with yerrorbars ls 3

# Win Test Case (Last)

set auto y
set auto x
set output "Graphs/WinLastThroughput.ps"
plot "Avg/TRex_WinLast_throughput_1" notitle with lines ls 1, "Avg/TRex_WinLast_throughput_1" title "2 s" with yerrorbars ls 1, "Avg/TRex_WinLast_throughput_2" notitle with lines ls 2, "Avg/TRex_WinLast_throughput_2" title "6 s" with yerrorbars ls 2, "Avg/TRex_WinLast_throughput_3" notitle with lines ls 3, "Avg/TRex_WinLast_throughput_3" title "10 s" with yerrorbars ls 3

# Selectivity Test Case (Each)

set yr[0:1000000]
set auto x
set output "Graphs/SelectivityEachThroughput.ps"
plot "Avg/TRex_SelectivityEach_throughput_5" notitle with lines ls 1, "Avg/TRex_SelectivityEach_throughput_5" title "10" with yerrorbars ls 1, "Avg/TRex_SelectivityEach_throughput_4" notitle with lines ls 2, "Avg/TRex_SelectivityEach_throughput_4" title "12.5" with yerrorbars ls 2, "Avg/TRex_SelectivityEach_throughput_3" notitle with lines ls 3, "Avg/TRex_SelectivityEach_throughput_3" title "16.5" with yerrorbars ls 3, "Avg/TRex_SelectivityEach_throughput_2" notitle with lines ls 4, "Avg/TRex_SelectivityEach_throughput_2" title "25" with yerrorbars ls 4, "Avg/TRex_SelectivityEach_throughput_1" notitle with lines ls 5, "Avg/TRex_SelectivityEach_throughput_1" title "50" with yerrorbars ls 5

# Selectivity Test Case (Last)

set yr[0:160000]
set auto x
set output "Graphs/SelectivityLastThroughput.ps"
plot "Avg/TRex_SelectivityLast_throughput_5" notitle with lines ls 1, "Avg/TRex_SelectivityLast_throughput_5" title "10" with yerrorbars ls 1, "Avg/TRex_SelectivityLast_throughput_4" notitle with lines ls 2, "Avg/TRex_SelectivityLast_throughput_4" title "12.5" with yerrorbars ls 2, "Avg/TRex_SelectivityLast_throughput_3" notitle with lines ls 3, "Avg/TRex_SelectivityLast_throughput_3" title "16.5" with yerrorbars ls 3, "Avg/TRex_SelectivityLast_throughput_2" notitle with lines ls 4, "Avg/TRex_SelectivityLast_throughput_2" title "25" with yerrorbars ls 4, "Avg/TRex_SelectivityLast_throughput_1" notitle with lines ls 5, "Avg/TRex_SelectivityLast_throughput_1" title "50" with yerrorbars ls 5

# NumCores Test Case (Each)

set yr[0:1200000]
set auto x
set output "Graphs/CoreEachThroughput.ps"
plot "Avg/TRex_CoreEach1_throughput_1" notitle with lines ls 1, "Avg/TRex_CoreEach1_throughput_1" title "1 Core" with yerrorbars ls 1, "Avg/TRex_CoreEach2_throughput_1" notitle with lines ls 2, "Avg/TRex_CoreEach2_throughput_1" title "2 Cores" with yerrorbars ls 2, "Avg/TRex_CoreEach3_throughput_1" notitle with lines ls 3, "Avg/TRex_CoreEach3_throughput_1" title "3 Cores" with yerrorbars ls 3, "Avg/TRex_CoreEach4_throughput_1" notitle with lines ls 4, "Avg/TRex_CoreEach4_throughput_1" title "4 Cores" with yerrorbars ls 4, "Avg/TRex_CoreEach5_throughput_1" notitle with lines ls 5, "Avg/TRex_CoreEach5_throughput_1" title "5 Cores" with yerrorbars ls 5

# NumCores Test Case (Last)

set auto y
set auto x
set output "Graphs/CoreLastThroughput.ps"
plot "Avg/TRex_CoreLast1_throughput_1" notitle with lines ls 1, "Avg/TRex_CoreLast1_throughput_1" title "1 Core" with yerrorbars ls 1, "Avg/TRex_CoreLast2_throughput_1" notitle with lines ls 2, "Avg/TRex_CoreLast2_throughput_1" title "2 Cores" with yerrorbars ls 2, "Avg/TRex_CoreLast3_throughput_1" notitle with lines ls 3, "Avg/TRex_CoreLast3_throughput_1" title "3 Cores" with yerrorbars ls 3, "Avg/TRex_CoreLast4_throughput_1" notitle with lines ls 4, "Avg/TRex_CoreLast4_throughput_1" title "4 Cores" with yerrorbars ls 4, "Avg/TRex_CoreLast5_throughput_1" notitle with lines ls 5, "Avg/TRex_CoreLast5_throughput_1" title "5 Cores" with yerrorbars ls 5

# Consuming Test Case (Each)

set auto y
set xr [:10]
set output "Graphs/ConsumingEachThroughput.ps"
plot "Avg/TRex_NoConsumingEach_throughput_1" notitle with lines ls 1, "Avg/TRex_NoConsumingEach_throughput_1" title "No Consuming" with yerrorbars ls 1, "Avg/TRex_EachConsuming_throughput_1" notitle with lines ls 2, "Avg/TRex_EachConsuming_throughput_1" title "Consuming" with yerrorbars ls 2

# Consuming Test Case (Last)

set auto y
set auto x
set output "Graphs/ConsumingLastThroughput.ps"
plot "Avg/TRex_LengthLast_throughput_1" notitle with lines ls 1, "Avg/TRex_LengthLast_throughput_1" title "No Consuming" with yerrorbars ls 1, "Avg/TRex_LastConsuming_throughput_1" notitle with lines ls 2, "Avg/TRex_LastConsuming_throughput_1" title "Consuming" with yerrorbars ls 2

# Negation Test Case

set auto y
set auto x
set output "Graphs/NegationThroughput.ps"
plot "Avg/TRex_Rain_throughput_1" notitle with lines ls 1, "Avg/TRex_Rain_throughput_1" title "1%" with yerrorbars ls 1, "Avg/TRex_Rain_throughput_2" notitle with lines ls 2, "Avg/TRex_Rain_throughput_2" title "3%" with yerrorbars ls 2, "Avg/TRex_Rain_throughput_3" notitle with lines ls 3, "Avg/TRex_Rain_throughput_3" title "5%" with yerrorbars ls 3

# Selection Test Case (with Esper)

set auto y
set auto x
set output "Graphs/SelectionThroughput.ps"
plot "Avg/TRex_Selection_throughput_1" notitle with lines ls 1, "Avg/TRex_Selection_throughput_1" title "T-Rex" with yerrorbars ls 1, "Avg/Esper_Selection_throughput_1" notitle with lines ls 2, "Avg/Esper_Selection_throughput_1" title "Esper" with yerrorbars ls 2

# Aggregate Test Case (with Esper) - Smoke = 10

set auto y
set auto x
set output "Graphs/AggregateThroughput10.ps"
plot "Avg/TRex_Aggregate_throughput_1" notitle with lines ls 1, "Avg/TRex_Aggregate_throughput_1" title "T-Rex" with yerrorbars ls 1, "Avg/Esper_Aggregate_throughput_1" notitle with lines ls 2, "Avg/Esper_Aggregate_throughput_1" title "Esper" with yerrorbars ls 2

# Aggregate Test Case (with Esper) - Smoke = 50

set auto y
set auto x
set output "Graphs/AggregateThroughput50.ps"
plot "Avg/TRex_Aggregate_throughput_2" notitle with lines ls 1, "Avg/TRex_Aggregate_throughput_2" title "T-Rex" with yerrorbars ls 1, "Avg/Esper_Aggregate_throughput_2" notitle with lines ls 2, "Avg/Esper_Aggregate_throughput_2" title "Esper" with yerrorbars ls 2

# Aggregate Test Case (with Esper) - Smoke = 90

set yr[0:350000]
set auto x
set output "Graphs/AggregateThroughput90.ps"
plot "Avg/TRex_Aggregate_throughput_3" notitle with lines ls 1, "Avg/TRex_Aggregate_throughput_3" title "T-Rex" with yerrorbars ls 1, "Avg/Esper_Aggregate_throughput_3" notitle with lines ls 2, "Avg/Esper_Aggregate_throughput_3" title "Esper" with yerrorbars ls 2

############################################
#                 TIME BARS                #
############################################

# Length Time (Each)

set auto x
set auto y
set output "Graphs/LengthEachTime.ps"
set line style 1 linewidth 20
set xlabel "Number of States"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:5]
set yr[0:900]
set xtics ("2 States" 1, "3 States" 2, "4 States" 3, "5 States" 4)
plot "Avg/TRex_LengthEach_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthEach_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthEach_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthEach_time_4" u 1:2 with imp notitle ls 1

# Length Time (Last)

set auto x
set auto y
set output "Graphs/LengthLastTime.ps"
set line style 1 linewidth 20
set xlabel "Number of States"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:5]
set yr[0:90]
set xtics ("2" 1, "3" 2, "4" 3, "5" 4)
plot "Avg/TRex_LengthLast_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthLast_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthLast_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthLast_time_4" u 1:2 with imp notitle ls 1

# Length Time (First)

set auto x
set auto y
set output "Graphs/LengthFirstTime.ps"
set line style 1 linewidth 20
set xlabel "Number of States"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:5]
set yr[0:350]
set xtics ("2" 1, "3" 2, "4" 3, "5" 4)
plot "Avg/TRex_LengthFirst_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthFirst_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthFirst_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_LengthFirst_time_4" u 1:2 with imp notitle ls 1

# Width Time (Each)

set auto x
set auto y
set output "Graphs/WidthEachTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Sequences"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:5]
set yr[0:]
set xtics ("2" 1, "3" 2, "4" 3, "5" 4)
plot "Avg/TRex_WidthEach_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthEach_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthEach_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthEach_time_4" u 1:2 with imp notitle ls 1

# Width Time (Last)

set auto x
set auto y
set output "Graphs/WidthLastTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Sequences"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:5]
set yr[0:]
set xtics ("2" 1, "3" 2, "4" 3, "5" 4)
plot "Avg/TRex_WidthLast_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthLast_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthLast_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_WidthLast_time_4" u 1:2 with imp notitle ls 1

# NumRules Time (Each)

set auto x
set auto y
set output "Graphs/NumRulesEachTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Rules"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("1000" 1, "3000" 2, "5000" 3, "7000" 4, "9000" 5)
plot "Avg/TRex_NumRulesEach_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesEach_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesEach_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesEach_time_4" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesEach_time_5" u 1:2 with imp notitle ls 1

# NumRules Time (Last)

set auto x
set auto y
set output "Graphs/NumRulesLastTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Rules"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("1000" 1, "3000" 2, "5000" 3, "7000" 4, "9000" 5)
plot "Avg/TRex_NumRulesLast_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesLast_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesLast_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesLast_time_4" u 1:2 with imp notitle ls 1, "Avg/TRex_NumRulesLast_time_5" u 1:2 with imp notitle ls 1

# Win Time (Each)

set auto x
set auto y
set output "Graphs/WinEachTime.ps"
set line style 1 linewidth 20
set xlabel "Average Window Size"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:4]
set yr[0:]
set xtics ("2 s" 1, "6 s" 2, "10 s" 3)
plot "Avg/TRex_WinEach_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_WinEach_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_WinEach_time_3" u 1:2 with imp notitle ls 1

# Win Time (Last)

set auto x
set auto y
set output "Graphs/WinLastTime.ps"
set line style 1 linewidth 20
set xlabel "Avg Window Size"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:4]
set yr[0:]
set xtics ("2 s" 1, "6 s" 2, "10 s" 3)
plot "Avg/TRex_WinLast_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_WinLast_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_WinLast_time_3" u 1:2 with imp notitle ls 1

# Selectivity Time (Each)

set auto x
set auto y
set output "Graphs/SelectivityEachTime.ps"
set line style 1 linewidth 20
set xlabel "Avg Number of Rules Activated by an Event"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("10" 1, "12.5" 2, "16.5" 3, "25" 4, "50" 5)
plot "Avg/TRex_SelectivityEach_time_5" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityEach_time_4" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityEach_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityEach_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityEach_time_1" u 1:2 with imp notitle ls 1

# Selectivity Time (Last)

set auto x
set auto y
set output "Graphs/SelectivityLastTime.ps"
set line style 1 linewidth 20
set xlabel "Avg Number of Rules Activated by an Event"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("10" 1, "12.5" 2, "16.5" 3, "25" 4, "50" 5)
plot "Avg/TRex_SelectivityLast_time_5" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityLast_time_4" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityLast_time_3" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityLast_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_SelectivityLast_time_1" u 1:2 with imp notitle ls 1

# NumCores Time (Each)

set auto x
set auto y
set output "Graphs/CoreEachTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Processing Cores"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("1" 1, "2" 2, "3" 3, "4" 4, "5" 5)
plot "Avg/TRex_CoreEach1_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreEach2_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreEach3_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreEach4_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreEach5_time_1" u 1:2 with imp notitle ls 1

# NumCores Time (Last)

set auto x
set auto y
set output "Graphs/CoreLastTime.ps"
set line style 1 linewidth 20
set xlabel "Number of Processing Cores"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:6]
set yr[0:]
set xtics ("1" 1, "2" 2, "3" 3, "4" 4, "5" 5)
plot "Avg/TRex_CoreLast1_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreLast2_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreLast3_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreLast4_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_CoreLast5_time_1" u 1:2 with imp notitle ls 1

# Consuming (Each)

set auto x
set auto y
set output "Graphs/ConsumingEachTime.ps"
set line style 1 linewidth 20
set xlabel "Consuming Clause"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:3]
set yr[0:]
set xtics ("No" 1, "Yes" 2)
plot "Avg/TRex_LengthEach_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_EachConsuming_time_1" u 1:2 with imp notitle ls 1

# Consuming (Last)

set auto x
set auto y
set output "Graphs/ConsumingLastTime.ps"
set line style 1 linewidth 20
set xlabel "Consuming Clause"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:3]
set yr[0:]
set xtics ("No" 1, "Yes" 2)
plot "Avg/TRex_LengthLast_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_LastConsuming_time_1" u 1:2 with imp notitle ls 1

# Negation Time

set auto x
set auto y
set output "Graphs/NegationTime.ps"
set line style 1 linewidth 20
set xlabel "Rain Events (%)"
set ylabel "Avg Processing Time (microseconds)"
set xr[0:4]
set yr[0:]
set xtics ("1" 1, "3" 2, "5" 3)
plot "Avg/TRex_Rain_time_1" u 1:2 with imp notitle ls 1, "Avg/TRex_Rain_time_2" u 1:2 with imp notitle ls 1, "Avg/TRex_Rain_time_3" u 1:2 with imp notitle ls 1