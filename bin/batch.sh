#!/bin/bash

#for setNumber in 15 
#do
#    for i in {2..180..2}
#    do
#        for seq in {0..15}
#        do
#            ./Process.exe ${setNumber} 1 FullDataSet.root $seq $i 0
#        done
#    done
#    ./Process.exe ${setNumber} 2
#done

for setNumber in 9 10 12 13 14 15
do
    ./Process.exe ${setNumber} 0
done

