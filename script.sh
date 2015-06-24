#!/bin/bash

rm -f *.root
#kEvents=100000000
kEvents=10000
outDir="/media/aogaki/Data/BI"

for ((i=0;i<100;i++)) do
    root -l -q "BeamProfile/ProtonGenerator.cpp+O($kEvents)"
    echo "/run/beamOn $kEvent" > tmp.mac
    ./BI -m tmp.mac
    hadd $outDir/`printf %.4d $i`".root" result*.root
    mv ProtonEnergy.dat $outDir/`printf %.4d $i`".dat"
done
