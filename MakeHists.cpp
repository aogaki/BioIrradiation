#include <iostream>

#include <TChain.h>
#include <TVector3.h>
#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>


TH2D *HisCell;

void DefineHists()
{
   Double_t plateL = 125.;
   Double_t plateW = 82.4;
   //Double_t binW = 0.02;
   Double_t binW = 0.1;
   HisCell = new TH2D("HisCell", "Deposited Energy at Cell Layer",
                      (plateL / binW), -plateL / 2, plateL / 2,
                      (plateW / binW), -plateW / 2, plateW / 2);
   HisCell->SetXTitle("[mm]");
   HisCell->SetYTitle("[mm]");
   HisCell->SetZTitle("Deposited Energy [MeV]");
}

void MakeHists()
{
   DefineHists();
   
   TChain *chain = new TChain("BI");;
   chain->Add("/media/aogaki/Data/BI/*.root");

   chain->SetBranchStatus("*", 0);

   TVector3 position;
   chain->SetBranchStatus("x", 1);
   chain->SetBranchStatus("y", 1);
   chain->SetBranchStatus("z", 1);
   chain->SetBranchAddress("x", &position[0]);
   chain->SetBranchAddress("y", &position[1]);
   chain->SetBranchAddress("z", &position[2]);
   
   Char_t volumeName[256]; // Array size is enough?
   chain->SetBranchStatus("VolumeName", 1);
   chain->SetBranchAddress("VolumeName", volumeName);

   Double_t ene;
   chain->SetBranchStatus("DepositEnergy", 1);
   chain->SetBranchAddress("DepositEnergy", &ene);

   const Int_t kTree = chain->GetNtrees();
   Int_t reachTree[kTree];
   for(auto &flag: reachTree) flag = 0;
   Int_t reachEvent[1000000];
   for(auto &flag: reachEvent) flag = 0;
   
   const Int_t kEntry = chain->GetEntries();
   cout << kEntry << endl;
   for(Int_t iEntry = 0; iEntry < kEntry; iEntry++){
      if(iEntry%10000 == 0) cout << iEntry <<" / "<< kEntry << endl;
      chain->GetEntry(iEntry);
      if(TString(volumeName) == "Cell"){
         //cout << position.X() <<"\t" << position.Y() << endl;
         HisCell->Fill(position.X(), position.Y(), ene);
      }
   }

   TCanvas *canvas = new TCanvas();
   HisCell->Draw("COLZ");
   canvas->Print("tmp.pdf", "pdf");
   
   TFile *outputFile = new TFile("tmp.root", "RECREATE");
   HisCell->Write();
   outputFile->Close();
}
