#define EventAnalyzer_cxx
// The class definition in EventAnalyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("EventAnalyzer.C")
// root> T->Process("EventAnalyzer.C","some options")
// root> T->Process("EventAnalyzer.C+")
//

#include <iostream>

#include "EventAnalyzer.h"
#include <TStyle.h>


EventAnalyzer::EventAnalyzer(TTree * /*tree*/)
   :fChain(nullptr),
    fHisAll(nullptr),
    fHisSealing(nullptr),
    fHisWindow(nullptr),
    fHisFoil(nullptr),
    fHisHolder(nullptr),
    fHisCassette(nullptr),
    fHisAir(nullptr),
    fHisPlate(nullptr),
    fHisFilm(nullptr),
    fHisWell(nullptr),
    fHisCell(nullptr)
{
   for(Int_t i = 0; i < 96; i++)
      fHisEachCell[i] = nullptr;
}

EventAnalyzer::~EventAnalyzer()
{
}

void EventAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void EventAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // Defining histograms
   const Double_t areaL = 200.;
   const Double_t areaW = 140.;
   
   const Double_t plateL = 125.;
   const Double_t plateW = 82.4;
   //Double_t binW = 0.02;

   fHisAll = new TH3D("HisAll", "Deposited Energy",
                      Int_t(areaL), -areaL / 2., areaL / 2.,
                      Int_t(areaW), -areaW / 2., areaW / 2.,
                      450, -10., 35.);
   fHisAll->SetXTitle("[mm]");
   fHisAll->SetYTitle("[mm]");
   fHisAll->SetZTitle("[mm]");
   GetOutputList()->Add(fHisAll);

   fHisSealing = new TH3D("HisSealing", "Deposited Energy at Sealing",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          60, -6., 0.);
   fHisSealing->SetXTitle("[mm]");
   fHisSealing->SetYTitle("[mm]");
   fHisSealing->SetZTitle("[mm]");
   GetOutputList()->Add(fHisSealing);

   fHisWindow = new TH3D("HisWindow", "Deposited Energy at Window",
                         Int_t(areaL), -areaL / 2., areaL / 2.,
                         Int_t(areaW), -areaW / 2., areaW / 2.,
                         20, -1., 1.);
   fHisWindow->SetXTitle("[mm]");
   fHisWindow->SetYTitle("[mm]");
   fHisWindow->SetZTitle("[mm]");
   GetOutputList()->Add(fHisWindow);
   
   fHisFoil = new TH3D("HisFoil", "Deposited Energy at Foil",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       15, 0., 1.5);
   fHisFoil->SetXTitle("[mm]");
   fHisFoil->SetYTitle("[mm]");
   fHisFoil->SetZTitle("[mm]");
   GetOutputList()->Add(fHisFoil);

   fHisHolder = new TH3D("HisHolder", "Deposited Energy at Holder",
                         Int_t(areaL), -areaL / 2., areaL / 2.,
                         Int_t(areaW), -areaW / 2., areaW / 2.,
                         30, 0., 3.);
   fHisHolder->SetXTitle("[mm]");
   fHisHolder->SetYTitle("[mm]");
   fHisHolder->SetZTitle("[mm]");
   GetOutputList()->Add(fHisHolder);
   
   fHisCassette = new TH3D("HisCassette", "Deposited Energy at Cassette",
                           Int_t(areaL), -areaL / 2., areaL / 2.,
                           Int_t(areaW), -areaW / 2., areaW / 2.,
                           300, 0., 30.);
   fHisCassette->SetXTitle("[mm]");
   fHisCassette->SetYTitle("[mm]");
   fHisCassette->SetZTitle("[mm]");
   GetOutputList()->Add(fHisCassette);
   
   fHisAir = new TH3D("HisAir", "Deposited Energy at Air",
                      Int_t(areaL), -areaL / 2., areaL / 2.,
                      Int_t(areaW), -areaW / 2., areaW / 2.,
                      300, 0., 30.);
   fHisAir->SetXTitle("[mm]");
   fHisAir->SetYTitle("[mm]");
   fHisAir->SetZTitle("[mm]");
   GetOutputList()->Add(fHisAir);
   
   fHisPlate = new TH3D("HisPlate", "Deposited Energy at Plate",
                        Int_t(areaL), -areaL / 2., areaL / 2.,
                        Int_t(areaW), -areaW / 2., areaW / 2.,
                        300, 0., 30.);
   fHisPlate->SetXTitle("[mm]");
   fHisPlate->SetYTitle("[mm]");
   fHisPlate->SetZTitle("[mm]");
   GetOutputList()->Add(fHisPlate);
   
   fHisFilm = new TH3D("HisFilm", "Deposited Energy at Film",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       300, 0., 30.);
   fHisFilm->SetXTitle("[mm]");
   fHisFilm->SetYTitle("[mm]");
   fHisFilm->SetZTitle("[mm]");
   GetOutputList()->Add(fHisFilm);
   
   fHisWell = new TH3D("HisWell", "Deposited Energy at Well",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       300, 0., 30.);
   fHisWell->SetXTitle("[mm]");
   fHisWell->SetYTitle("[mm]");
   fHisWell->SetZTitle("[mm]");
   GetOutputList()->Add(fHisWell);

   fHisCell = new TH2D("HisCell", "Deposited Energy at Cell",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.);
   fHisCell->SetXTitle("[mm]");
   fHisCell->SetYTitle("[mm]");
   GetOutputList()->Add(fHisCell);

   const Double_t wellPitch = 9.;
   const Double_t bin20W = 0.02;
   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His" + XYtoWell(x, y);
         TString title = "Deposited Energy at " + XYtoWell(x, y);
         fHisEachCell[hisIt] = new TH2D(name, title,
                                          Int_t(wellPitch / bin20W), x, x + wellPitch,
                                          Int_t(wellPitch / bin20W), y, y + wellPitch);
         GetOutputList()->Add(fHisEachCell[hisIt++]);
      }
   }

}

Bool_t EventAnalyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventAnalyzer::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   b_VolumeName->GetEntry(entry);
   b_DepositEnergy->GetEntry(entry);
   b_x->GetEntry(entry);
   b_y->GetEntry(entry);
   b_z->GetEntry(entry);
   
   if(DepositEnergy > 0.){
      fHisAll->Fill(x, y, z, DepositEnergy);

      if(TString(VolumeName) == "Sealing")
         fHisSealing->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Window")
         fHisWindow->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Foil")
         fHisFoil->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Holder")
         fHisHolder->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Cassette")
         fHisCassette->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Air")
         fHisAir->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Plate" ||
              TString(VolumeName) == "Well" ||
              TString(VolumeName) == "Outer")
         fHisPlate->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Film")
         fHisFilm->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Stuff")
         fHisWell->Fill(x, y, z, DepositEnergy);
      else if(TString(VolumeName) == "Cell"){
         Int_t wellID = XYtoIndex(x, y);
         fHisEachCell[wellID]->Fill(x, y, DepositEnergy);
      }
   }
   return kTRUE;
}

void EventAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
}

void EventAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   TFile *outputFile = new TFile("tmp.root", "RECREATE");

   fHisAll->Write();
   fHisSealing->Write();
   fHisWindow->Write();
   fHisFoil->Write();
   fHisHolder->Write();
   fHisCassette->Write();
   fHisAir->Write();
   fHisPlate->Write();
   fHisFilm->Write();
   fHisWell->Write();
   for(Int_t i = 0; i < 96; i++)
      fHisEachCell[i]->Write();

   outputFile->Close();

   fHisEachCell[48]->Draw("COLZ");
}

TString  EventAnalyzer::XYtoWell(Double_t x, Double_t y)
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

Int_t  EventAnalyzer::XYtoIndex(Double_t x, Double_t y)
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
