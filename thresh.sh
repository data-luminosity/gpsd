#!/bin/bash
cd logs

../cgps > 1.txt 2>&1 &
../cgps > 2.txt 2>&1 &
../cgps > 3.txt 2>&1 &
../cgps > 4.txt 2>&1 &
../cgps > 5.txt 2>&1 &
../cgps > 6.txt 2>&1 &
../cgps > 7.txt 2>&1 &



#i=0
#while [ $i -lt 10 ]; do
    #echo $i
    #name=$i
    #touch name
    #../cgps &> $name &

    #i=$[$i+1]
#done
