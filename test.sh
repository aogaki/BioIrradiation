#!/bin/bash
for((i=$2;i>=$1;i--))
do
    suffix=`printf %04d $i`
    ./BI -m test.mac --grid --oldbeam --tile
    hadd -f result.$suffix.root result_t*.root
done
