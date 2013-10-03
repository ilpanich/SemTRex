import subprocess
from math import sqrt
import os
import scipy.stats

def AvgThroughput(filename, column):
    output = open("Avg/" + filename + "_throughput_" + str(column), "w")
    temp = open("Results/" + filename + "_throughput1", "r")
    numLines = len(temp.readlines())
    temp.close()

    output.write("0\t0\t0\n")
    for line in range(0, numLines):
        sum = 0
        sumSquare = 0
        count = 0
        label = ""
        for i in range(1,11):    
            count = count+1
            f = open("Results/" + filename + "_throughput" + str(i), "r")
            lines = f.readlines()
            tokens = lines[line].split("\t")
            label = tokens[0]
            val =  float(tokens[column].replace(",","."))     
            sum = sum + val
            sumSquare = sumSquare + (val)**2
            f.close()

        mean = sum/count
        sampleStdDev = sqrt((sumSquare/count - mean**2)*count/(count-1))
        delta = (-scipy.stats.t.ppf(0.05,count-1).sum() * sampleStdDev)/sqrt(count)

        output.write(str(float(label)/1000) + "\t" + str(mean) + "\t" + str(delta) + "\n")

def AvgTime(filename, column, label):
    output = open("Avg/" + filename + "_time_" + str(column), "w")
    temp = open("Results/" + filename + "_time1", "r")
    numLines = len(temp.readlines())
    temp.close()

    sum = 0
    sumSquare = 0
    count = 0
    for line in range(0, numLines):    
        for i in range(1,11):    
            count = count+1
            f = open("Results/" + filename + "_time" + str(i), "r")
            lines = f.readlines()
            tokens = lines[line].split("\t")
            val =  float(tokens[column].replace(",","."))     
            sum = sum + val
            sumSquare = sumSquare + (val)**2
            f.close()

    mean = sum/count
    sampleStdDev = sqrt((sumSquare/count - mean**2)*count/(count-1))
    delta = (-scipy.stats.t.ppf(0.05,count-1).sum() * sampleStdDev)/sqrt(count)

    if (label>0):
        output.write(str(label) + "\t" + str(mean) + "\t" + str(delta) + "\n")
    else:
        output.write(str(column) + "\t" + str(mean) + "\t" + str(delta) + "\n")

def Avg(filename, columns, label=0):
    if "Selectivity" in filename:
        for column in range(1,columns+1):
            AvgThroughput(filename, column)
            AvgTime(filename, column, 6-column)
    else:
        for column in range(1,columns+1):
            AvgThroughput(filename, column)
            AvgTime(filename, column, label)

Avg("TRex_LengthEach", 4)
Avg("TRex_LengthLast", 4)
Avg("TRex_LengthFirst", 4)
Avg("TRex_WidthEach", 4)
Avg("TRex_WidthLast", 4)
Avg("TRex_NumRulesEach", 5)
Avg("TRex_NumRulesLast", 5)
Avg("TRex_SelectivityEach", 5)
Avg("TRex_SelectivityLast", 5)
Avg("TRex_WinEach", 3)
Avg("TRex_WinLast", 3)
Avg("TRex_EachConsuming", 1, 2)
Avg("TRex_LastConsuming", 1, 2)
Avg("TRex_Rain", 3)
Avg("TRex_Selection", 1)
Avg("TRex_Aggregate", 3)
Avg("TRex_NoConsumingEach", 1)

for i in range(1, 4):
    AvgThroughput("Esper_Aggregate", i)

AvgThroughput("Esper_Selection", 1)

for i in range(1, 6):
    Avg("TRex_CoreEach"+str(i), 1, i)
    Avg("TRex_CoreLast"+str(i), 1, i)
