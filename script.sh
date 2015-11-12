#!/bin/bash

#kEvents="10000000"
kEvents="10000"
kMat="Al"
kT="1000"
kD="300"

outDir="Data/$kMat$kT""D$kD"
mkdir -p $outDir
rm -f $outDir/*
rm -f tmp.root
rm -f tmp.mac

echo "/BI/Geometry/WindowMaterial G4_$kMat" > tmp.mac
echo "/BI/Geometry/WindowThickness $kT" >> tmp.mac
echo "/BI/Primary/Z -$kD" >> tmp.mac
echo "/run/beamOn $kEvents" >> tmp.mac

rm -f attT.dat
ln -sf AttData/att"$kMat$kT".dat attT.dat

for ((i=1;i<=1;i++)); do
    ./BI -m tmp.mac
    root -l -q run.cpp+O
    mv tmp.root $outDir/$i.root
done

hadd -f "$kMat$kT""D$kD.root" $outDir/*.root
