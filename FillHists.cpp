#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TVector3.h>
#include <TFile.h>
#include <TH2.h>
#include <TH3.h>
#include <TCanvas.h>
#include <TString.h>


TH3D *HisAll;
TH3D *HisSealing;
TH3D *HisWindow;
TH3D *HisHolder;
TH3D *HisCassette;
TH3D *HisAir;
TH3D *HisPlate;
TH3D *HisFilm;
TH3D *HisWell;
TH2D *HisEachCell20[96];
TH2D *HisEachCell50[96];

TString XYtoWell(Double_t x, Double_t y)
{
   if(x < -54. || x >= 54. || y < -36. || y >= 36.){
      cout << "The coordinate is out of range.  x = " << x
           << ", y = " << y << endl;
      exit(0);
   }
   
   TString alphabet[8] = {"H", "G", "F", "E", "D", "C", "B", "A"};
   x += 9. * 6.;
   y += 9. * 4.;

   Int_t wellX = Int_t(x / 9.);
   Int_t wellY = Int_t(y / 9.);

   // readable?
   return Form("%s%02d", alphabet[wellY].Data(), wellX + 1);
}

Int_t XYtoIndex(Double_t x, Double_t y)
{
      if(x < -54. || x >= 54. || y < -36. || y >= 36.){
      cout << "The coordinate is out of range.  x = " << x
           << ", y = " << y << endl;
      exit(0);
   }
   
   TString alphabet[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};
   x += 9. * 6.;
   y += 9. * 4.;

   Int_t wellX = Int_t(x / 9.);
   Int_t wellY = Int_t(y / 9.);

   return (wellX + wellY * 12);
}

void GetHists(TFile *file)
{
   HisAll = (TH3D*)file->Get("HisAll");
   HisSealing = (TH3D*)file->Get("HisSealing");
   HisWindow = (TH3D*)file->Get("HisWindow");
   HisHolder = (TH3D*)file->Get("HisHolder");
   HisCassette = (TH3D*)file->Get("HisCassette");
   HisAir = (TH3D*)file->Get("HisAir");
   HisPlate = (TH3D*)file->Get("HisPlate");
   HisFilm = (TH3D*)file->Get("HisFilm");
   HisWell = (TH3D*)file->Get("HisWell");

   const Double_t wellPitch = 9.;
   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His20" + XYtoWell(x, y);
         HisEachCell20[hisIt++] = (TH2D*)file->Get(name);
      }
   }

   hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His50" + XYtoWell(x, y);
         HisEachCell50[hisIt++] = (TH2D*)file->Get(name);
      }
   }
   
}

void FillHists()
{
   TFile *inputFile = new TFile("tmp.root");

   GetHists(inputFile);

   TChain *chain = new TChain("BI");
   //chain->Add("/media/aogaki/Data/BI/*.root");
   chain->Add("result*.root");

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

   const Int_t kEntry = chain->GetEntries();
   cout << kEntry << endl;
   for(Int_t iEntry = 0; iEntry < kEntry; iEntry++){
      //if(iEntry%100000 == 0) cout << iEntry <<" / "<< kEntry << endl;
      chain->GetEntry(iEntry);
      if(!(ene > 0.)) continue;
      
      HisAll->Fill(position.X(), position.Y(), position.Z(), ene);

      if(TString(volumeName) == "Sealing")
         HisSealing->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Window")
         HisWindow->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Holder")
         HisHolder->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Cassette")
         HisCassette->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Air")
         HisAir->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Plate" ||
              TString(volumeName) == "Well" ||
              TString(volumeName) == "Outer")
         HisPlate->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Film")
         HisFilm->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Stuff")
         HisWell->Fill(position.X(), position.Y(), position.Z(), ene);

      else if(TString(volumeName) == "Cell"){
         Int_t wellID = XYtoIndex(position.X(), position.Y());
         HisEachCell20[wellID]->Fill(position.X(), position.Y(), ene);
         HisEachCell50[wellID]->Fill(position.X(), position.Y(), ene);
      }

   }

   TFile *outputFile = new TFile("out.root", "RECREATE");
   
   HisAll->Write();
   HisSealing->Write();
   HisWindow->Write();
   HisHolder->Write();
   HisCassette->Write();
   HisAir->Write();
   HisPlate->Write();
   HisFilm->Write();
   HisWell->Write();
   for(Int_t i = 0; i < 96; i++)
      HisEachCell20[i]->Write();
   for(Int_t i = 0; i < 96; i++)
      HisEachCell50[i]->Write();

   outputFile->Close();
   
   inputFile->Close();

}
