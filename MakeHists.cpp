#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TVector3.h"


TH3D *HisPosition;
TH2D *HisDose;
TH2D *HisWindow;
TH2D *HisGap;
TH2D *HisPlate;

void MakeHists()
{
   HisPosition = new TH3D("HisPosition", "spatial distribution",
                          101, -50.5, 50.5, 101, -50.5, 50.5,
                          130, 0., 13.0);

   HisDose = new TH2D("HisDose", "energy distribution",
                      101, -50.5, 50.5, 101, -50.5, 50.5);

   HisWindow = new TH2D("HisWindow", "spatial distribution",
                        101, -50.5, 50.5, 101, -50.5, 50.5);

   HisGap = new TH2D("HisGap", "spatial distribution",
                     101, -50.5, 50.5, 101, -50.5, 50.5);

   HisPlate = new TH2D("HisPlate", "spatial distribution",
                       101, -50.5, 50.5, 101, -50.5, 50.5);

   
   TFile *inputFile = new TFile("sum.root", "READ");
   TTree *inputTree = (TTree*)inputFile->Get("BI");

   Int_t PDG;
   inputTree->SetBranchAddress("PDGCode", &PDG);

   Double_t depEne; // deposit energy
   inputTree->SetBranchAddress("DepositEnergy", &depEne);

   Double_t time;
   inputTree->SetBranchAddress("Time", &time);

   Char_t volumeName[256]; // Array size is enough?
   inputTree->SetBranchAddress("VolumeName", volumeName);

   TVector3 position;
   inputTree->SetBranchAddress("x", &position[0]);
   inputTree->SetBranchAddress("y", &position[1]);
   inputTree->SetBranchAddress("z", &position[2]);

   TVector3 momentum;
   inputTree->SetBranchAddress("vx", &momentum[0]);
   inputTree->SetBranchAddress("vy", &momentum[1]);
   inputTree->SetBranchAddress("vz", &momentum[2]);

   const Int_t kEve = inputTree->GetEntries();
   for(Int_t iEve = 0; iEve < kEve; iEve++){
      inputTree->GetEntry(iEve);

      HisPosition->Fill(position.x(), position.y(), position.z());
      if(TString(volumeName) == "Window")
         HisWindow->Fill(position.x(), position.y(), position.z());
      else if(TString(volumeName) == "AirGap")
         HisGap->Fill(position.x(), position.y());
      else if(TString(volumeName) == "Plate")
         HisPlate->Fill(position.x(), position.y());
      else if(TString(volumeName) == "Cell")
         HisDose->Fill(position.x(), position.y(), depEne);
   }

   HisPlate->Draw("COLZ");
}
