#!/bin/bash

#kEvents=100000000
kEvents=10000000
#kEvents=10000
#outDir="/media/aogaki/Data/BI"

outDir="Ni200"
rm -f *.root
rm -f tmp.mac
echo "/BI/Geometry/WindowMaterial G4_Ni" > tmp.mac
echo "/BI/Geometry/WindowThickness 200" >> tmp.mac
echo "/run/beamOn $kEvents" >> tmp.mac
./BI -m tmp.mac
root -l -q MakeHists.cpp+O
mv out.root tmp.root

for ((i=1;i<1500;i++)); do
    ./BI -m tmp.mac
    root -l -q FillHists.cpp+O
    mv out.root $outDir/$i.root
    rm tmp.root
    ln -sf $outDir/$i.root tmp.root
done
