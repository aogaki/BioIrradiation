#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TVector3.h"
#include "TMath.h"


TH1D *HistEnergy;
TH1D *HistTheta;
TH1D *HistPhi;

void InitialDistributions()
{
   TString fileName = "test.root";
   TFile *inputFile = new TFile(fileName, "OPEN");
   TTree *inputTree = (TTree*)inputFile->Get("InitPar");

   inputTree->Print();

   inputTree->SetBranchStatus("PDGCode", 0);

   Double_t ene;
   inputTree->SetBranchAddress("TotalEnergy", &ene);

   Double_t vx;
   inputTree->SetBranchAddress("vx", &vx);
   Double_t vy;
   inputTree->SetBranchAddress("vy", &vy);
   Double_t vz;
   inputTree->SetBranchAddress("vz", &vz);

   HistEnergy = new TH1D("HistEnergy", "Initial Energy Distribution", 
                         700, 0, 70);
   HistEnergy->SetXTitle("[MeV]");

   HistTheta = new TH1D("HistTheta", "Initial Polar Anglular Distribution", 
                           200, 0, 20);
   HistTheta->SetXTitle("[deg]");

   HistPhi = new TH1D("HistPhi", "Initial Azimuth Anglular Distribution", 
                      400, -200, 200);
   HistPhi->SetXTitle("[deg]");

   const Int_t nEve = inputTree->GetEntries();
   for(Int_t iEve = 0; iEve < nEve; iEve++){
      inputTree->GetEntry(iEve);

      TVector3 vec(vx, vy, vz);
      Double_t theta = vec.Theta();
      Double_t phi = vec.Phi();

      HistEnergy->Fill(ene); 
      HistTheta->Fill(theta * 180. / TMath::Pi());
      HistPhi->Fill(phi * 180. / TMath::Pi());

   }

   //delete inputTree;
   //inputFile->Close();
}
