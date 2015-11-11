#!/bin/bash

for ((i=61;i<100;i++)); do
    ./BI -m test.mac
    hadd tmp/test$i.root /dev/shm/result*
done
