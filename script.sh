#!/bin/bash

for((i=0;i<=20;i++))
do
    rm -f circle.att
    
    for((j=0;j<70;j++))
    do
	echo $((i * 500)) >> circle.att
    done

    tar zcvf bi$i.tgz *
    mv bi*.tgz ../
    
done
