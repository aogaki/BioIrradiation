#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TVector3.h"
#include "TMath.h"


TH1D *HisEnergy;
TH1D *HisTheta;
TH1D *HisCosTheta;
TH1D *HisPhi;

void InitialDistributions()
{
   TString fileName = "test.root";
   TFile *inputFile = new TFile(fileName, "OPEN");
   TTree *inputTree = (TTree*)inputFile->Get("InitPar");

   inputTree->Print();

   inputTree->SetBranchStatus("PDGCode", 0);

   Double_t ene;
   inputTree->SetBranchAddress("Energy", &ene);

   Double_t vx;
   inputTree->SetBranchAddress("vx", &vx);
   Double_t vy;
   inputTree->SetBranchAddress("vy", &vy);
   Double_t vz;
   inputTree->SetBranchAddress("vz", &vz);

   HisEnergy = new TH1D("HisEnergy", "Initial Energy Distribution", 
                        700, 0, 70);
   HisEnergy->SetXTitle("[MeV]");

   HisTheta = new TH1D("HisTheta", "Initial Polar Anglular Distribution", 
                       200, 0, 20);
   HisTheta->SetXTitle("[deg]");

   HisCosTheta = new TH1D("HisCosTheta", "Initial Polar Anglular Distribution", 
                          200, cos(TMath::Pi() * 20. / 180.), 1.0);
   HisCosTheta->SetXTitle("[Cos(deg)]");

   HisPhi = new TH1D("HisPhi", "Initial Azimuth Anglular Distribution", 
                     400, -200, 200);
   HisPhi->SetXTitle("[deg]");

   const Int_t nEve = inputTree->GetEntries();
   for(Int_t iEve = 0; iEve < nEve; iEve++){
      inputTree->GetEntry(iEve);

      TVector3 vec(vx, vy, vz);
      Double_t theta = vec.Theta();
      Double_t phi = vec.Phi();

      HisEnergy->Fill(ene); 
      if(!(ene > 30.)){
         HisTheta->Fill(theta * 180. / TMath::Pi());
         HisCosTheta->Fill(vec.CosTheta());
      }
      HisPhi->Fill(phi * 180. / TMath::Pi());

   }

   //delete inputTree;
   //inputFile->Close();
}
