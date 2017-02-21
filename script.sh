#!/bin/bash

for((i=0;i<100;i++))
do
    ./BI -b 5 --grid -m test.mac
    hadd -f gamma$i.root result_t*
done
