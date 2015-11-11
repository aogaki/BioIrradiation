#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TVector3.h>
#include <TFile.h>
#include <TH2.h>
#include <TH3.h>
#include <TCanvas.h>
#include <TString.h>
#include <TTree.h>


TH3D *HisAll;
TH3D *HisSealing;
TH3D *HisWindow;
TH3D *HisFoil;
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

void DefineHists()
{
   const Double_t areaL = 200.;
   const Double_t areaW = 140.;
   
   const Double_t plateL = 125.;
   const Double_t plateW = 82.4;
   //Double_t binW = 0.02;

   HisAll = new TH3D("HisAll", "Deposited Energy",
                     Int_t(areaL), -areaL / 2., areaL / 2.,
                     Int_t(areaW), -areaW / 2., areaW / 2.,
                     450, -10., 35.);
   HisAll->SetXTitle("[mm]");
   HisAll->SetYTitle("[mm]");
   HisAll->SetZTitle("[mm]");

   HisSealing = new TH3D("HisSealing", "Deposited Energy at Sealing",
                         Int_t(areaL), -areaL / 2., areaL / 2.,
                         Int_t(areaW), -areaW / 2., areaW / 2.,
                         60, -6., 0.);
   HisSealing->SetXTitle("[mm]");
   HisSealing->SetYTitle("[mm]");
   HisSealing->SetZTitle("[mm]");

   HisWindow = new TH3D("HisWindow", "Deposited Energy at Window",
                        Int_t(areaL), -areaL / 2., areaL / 2.,
                        Int_t(areaW), -areaW / 2., areaW / 2.,
                        20, -1., 1.);
   HisWindow->SetXTitle("[mm]");
   HisWindow->SetYTitle("[mm]");
   HisWindow->SetZTitle("[mm]");

   HisFoil = new TH3D("HisFoil", "Deposited Energy at Foil",
                      Int_t(areaL), -areaL / 2., areaL / 2.,
                      Int_t(areaW), -areaW / 2., areaW / 2.,
                      15, 0., 1.5);
   HisFoil->SetXTitle("[mm]");
   HisFoil->SetYTitle("[mm]");
   HisFoil->SetZTitle("[mm]");

   HisHolder = new TH3D("HisHolder", "Deposited Energy at Holder",
                        Int_t(areaL), -areaL / 2., areaL / 2.,
                        Int_t(areaW), -areaW / 2., areaW / 2.,
                        30, 0., 3.);
   HisHolder->SetXTitle("[mm]");
   HisHolder->SetYTitle("[mm]");
   HisHolder->SetZTitle("[mm]");

   HisCassette = new TH3D("HisCassette", "Deposited Energy at Cassette",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          300, 0., 30.);
   HisCassette->SetXTitle("[mm]");
   HisCassette->SetYTitle("[mm]");
   HisCassette->SetZTitle("[mm]");

   HisAir = new TH3D("HisAir", "Deposited Energy at Air",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          300, 0., 30.);
   HisAir->SetXTitle("[mm]");
   HisAir->SetYTitle("[mm]");
   HisAir->SetZTitle("[mm]");

   HisPlate = new TH3D("HisPlate", "Deposited Energy at Plate",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          300, 0., 30.);
   HisPlate->SetXTitle("[mm]");
   HisPlate->SetYTitle("[mm]");
   HisPlate->SetZTitle("[mm]");

   HisFilm = new TH3D("HisFilm", "Deposited Energy at Film",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          300, 0., 30.);
   HisFilm->SetXTitle("[mm]");
   HisFilm->SetYTitle("[mm]");
   HisFilm->SetZTitle("[mm]");

   HisWell = new TH3D("HisWell", "Deposited Energy at Well",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          300, 0., 30.);
   HisWell->SetXTitle("[mm]");
   HisWell->SetYTitle("[mm]");
   HisWell->SetZTitle("[mm]");


   const Double_t wellPitch = 9.;
   const Double_t bin20W = 0.02;
   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His20" + XYtoWell(x, y);
         TString title = "Deposited Energy at " + XYtoWell(x, y);
         HisEachCell20[hisIt++] = new TH2D(name, title,
                                           Int_t(wellPitch / bin20W), x, x + wellPitch,
                                           Int_t(wellPitch / bin20W), y, y + wellPitch);
      }
   }
   
   const Double_t bin50W = 0.05;
   hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His50" + XYtoWell(x, y);
         TString title = "Deposited Energy at " + XYtoWell(x, y);
         HisEachCell50[hisIt++] = new TH2D(name, title,
                                           Int_t(wellPitch / bin50W), x, x + wellPitch,
                                           Int_t(wellPitch / bin50W), y, y + wellPitch);
      }
   }
   
}

void MakeHists()
{
   DefineHists();

   TChain *chain = new TChain("BI");
   //chain->Add("/media/aogaki/Data/BI/*.root");
   chain->Add("/dev/shm/result*.root");

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
      else if(TString(volumeName) == "Foil")
         HisFoil->Fill(position.X(), position.Y(), position.Z(), ene);
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
/*
   TCanvas *canvas = new TCanvas();
   HisCell->Draw("COLZ");
   canvas->Print("tmp.pdf", "pdf"); 
*/

   TFile *outputFile = new TFile("tmp.root", "RECREATE");

   HisAll->Write();
   HisSealing->Write();
   HisWindow->Write();
   HisFoil->Write();
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
}
