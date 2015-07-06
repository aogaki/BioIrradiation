#!/bin/bash

#rm -f *.root
#kEvents=100000000
kEvents=10000000
#kEvents=1000000
#outDir="/media/aogaki/Data/BI"

#root -l -q "BeamProfile/ProtonGenerator.cpp+O($kEvents)"
#echo "/run/beamOn $kEvents" > tmp.mac
#./BI -m tmp.mac
#root -l -q MakeHists.cpp+O

for ((i=303;i<10000;i++)) do
    root -l -q "BeamProfile/ProtonGenerator.cpp+O($kEvents)"
    ./BI -m tmp.mac
    root -l -q FillHists.cpp+O
    cp out.root tmp.root
    #rm -f Out/*
    cp out.root Out/$i.root
done
