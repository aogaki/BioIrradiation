#!/bin/bash
for((i=$1;i<=$2;i++))
do
    suffix=`printf %04d $i`
    ./BI -m test.mac --grid --oldbeam --tile
    hadd -f resultT160.$suffix.root result_t*.root
done
