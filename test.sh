#!/bin/bash

kEvents=10000000
#kEvents=100000
kMat="Al"
#kT="5000"
kT="1000"


kD="300"

outDir="$kMat""T$kT""D$kD""Att"
mkdir $outDir
rm -f tmp.mac
echo "/BI/Geometry/WindowMaterial G4_$kMat" > tmp.mac
echo "/BI/Geometry/WindowThickness $kT" >> tmp.mac
echo "/BI/Primary/Z -$kD" >> tmp.mac
echo "/run/beamOn $kEvents" >> tmp.mac

./BI -m tmp.mac
root -l -q MakeHists.cpp+O
rm -f tmp.root
mv out.root tmp.root

for ((i=1;i<1000;i++)); do
    ./BI -m tmp.mac
    root -l -q FillHists.cpp+O
    mv out.root $outDir/$i.root
    rm -f tmp.root
    ln -sf $outDir/$i.root tmp.root
done
