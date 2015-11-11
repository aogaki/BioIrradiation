#include <iostream>

#include <TThread.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TString.h>
#include <TVector3.h>


const Int_t kThreads = 8;

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

class THisHandler
{
public:
   THisHandler(TFile *);
   virtual ~THisHandler();

   void DefineHists();
   void FillHists();
   void AddHists();
   
private:
   TFile *fFile;
   TTree *fTree;
   
   TH3D *fHisAll;
   TH3D *fHisSealing;
   TH3D *fHisWindow;
   TH3D *fHisFoil;
   TH3D *fHisHolder;
   TH3D *fHisCassette;
   TH3D *fHisAir;
   TH3D *fHisPlate;
   TH3D *fHisFilm;
   TH3D *fHisWell;
   TH2D *fHisEachCell20[96];
   TH2D *fHisEachCell50[96];
  
};

THisHandler::THisHandler(TFile *inputFile)
   : fFile(0),
     fTree(0)
{
   fFile = inputFile;
   fFile->Print();
   fTree = (TTree*)fFile->Get("BI");
   cout << fTree->GetEntries() << endl;

   DefineHists();
}

THisHandler::~THisHandler()
{}

void THisHandler::DefineHists()
{
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

   fHisSealing = new TH3D("HisSealing", "Deposited Energy at Sealing",
                          Int_t(areaL), -areaL / 2., areaL / 2.,
                          Int_t(areaW), -areaW / 2., areaW / 2.,
                          60, -6., 0.);
   fHisSealing->SetXTitle("[mm]");
   fHisSealing->SetYTitle("[mm]");
   fHisSealing->SetZTitle("[mm]");

   fHisWindow = new TH3D("HisWindow", "Deposited Energy at Window",
                         Int_t(areaL), -areaL / 2., areaL / 2.,
                         Int_t(areaW), -areaW / 2., areaW / 2.,
                         20, -1., 1.);
   fHisWindow->SetXTitle("[mm]");
   fHisWindow->SetYTitle("[mm]");
   fHisWindow->SetZTitle("[mm]");

   fHisFoil = new TH3D("HisFoil", "Deposited Energy at Foil",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       15, 0., 1.5);
   fHisFoil->SetXTitle("[mm]");
   fHisFoil->SetYTitle("[mm]");
   fHisFoil->SetZTitle("[mm]");

   fHisHolder = new TH3D("HisHolder", "Deposited Energy at Holder",
                         Int_t(areaL), -areaL / 2., areaL / 2.,
                         Int_t(areaW), -areaW / 2., areaW / 2.,
                         30, 0., 3.);
   fHisHolder->SetXTitle("[mm]");
   fHisHolder->SetYTitle("[mm]");
   fHisHolder->SetZTitle("[mm]");

   fHisCassette = new TH3D("HisCassette", "Deposited Energy at Cassette",
                           Int_t(areaL), -areaL / 2., areaL / 2.,
                           Int_t(areaW), -areaW / 2., areaW / 2.,
                           300, 0., 30.);
   fHisCassette->SetXTitle("[mm]");
   fHisCassette->SetYTitle("[mm]");
   fHisCassette->SetZTitle("[mm]");

   fHisAir = new TH3D("HisAir", "Deposited Energy at Air",
                      Int_t(areaL), -areaL / 2., areaL / 2.,
                      Int_t(areaW), -areaW / 2., areaW / 2.,
                      300, 0., 30.);
   fHisAir->SetXTitle("[mm]");
   fHisAir->SetYTitle("[mm]");
   fHisAir->SetZTitle("[mm]");

   fHisPlate = new TH3D("HisPlate", "Deposited Energy at Plate",
                        Int_t(areaL), -areaL / 2., areaL / 2.,
                        Int_t(areaW), -areaW / 2., areaW / 2.,
                        300, 0., 30.);
   fHisPlate->SetXTitle("[mm]");
   fHisPlate->SetYTitle("[mm]");
   fHisPlate->SetZTitle("[mm]");

   fHisFilm = new TH3D("HisFilm", "Deposited Energy at Film",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       300, 0., 30.);
   fHisFilm->SetXTitle("[mm]");
   fHisFilm->SetYTitle("[mm]");
   fHisFilm->SetZTitle("[mm]");

   fHisWell = new TH3D("HisWell", "Deposited Energy at Well",
                       Int_t(areaL), -areaL / 2., areaL / 2.,
                       Int_t(areaW), -areaW / 2., areaW / 2.,
                       300, 0., 30.);
   fHisWell->SetXTitle("[mm]");
   fHisWell->SetYTitle("[mm]");
   fHisWell->SetZTitle("[mm]");


   const Double_t wellPitch = 9.;
   const Double_t bin20W = 0.02;
   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His20" + XYtoWell(x, y);
         TString title = "Deposited Energy at " + XYtoWell(x, y);
         fHisEachCell20[hisIt++] = new TH2D(name, title,
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
         fHisEachCell50[hisIt++] = new TH2D(name, title,
                                            Int_t(wellPitch / bin50W), x, x + wellPitch,
                                            Int_t(wellPitch / bin50W), y, y + wellPitch);
      }
   }
      
}

void THisHandler::FillHists()
{
   fTree->SetBranchStatus("*", 0);

   TVector3 position;
   fTree->SetBranchStatus("x", 1);
   fTree->SetBranchStatus("y", 1);
   fTree->SetBranchStatus("z", 1);
   fTree->SetBranchAddress("x", &position[0]);
   fTree->SetBranchAddress("y", &position[1]);
   fTree->SetBranchAddress("z", &position[2]);
   
   Char_t volumeName[256]; // Array size is enough?
   fTree->SetBranchStatus("VolumeName", 1);
   fTree->SetBranchAddress("VolumeName", volumeName);

   Double_t ene;
   fTree->SetBranchStatus("DepositEnergy", 1);
   fTree->SetBranchAddress("DepositEnergy", &ene);

   const Int_t kEntries = fTree->GetEntries();
   for(Int_t iEntry = 0; iEntry < kEntries; iEntry++){
      //if(iEntry%100000 == 0) cout << iEntry <<" / "<< kEntries << endl;
      fTree->GetEntry(iEntry);
      if(!(ene > 0.)) continue;
      
      fHisAll->Fill(position.X(), position.Y(), position.Z(), ene);

      if(TString(volumeName) == "Sealing")
         fHisSealing->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Window")
         fHisWindow->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Foil")
         fHisFoil->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Holder")
         fHisHolder->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Cassette")
         fHisCassette->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Air")
         fHisAir->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Plate" ||
              TString(volumeName) == "Well" ||
              TString(volumeName) == "Outer")
         fHisPlate->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Film")
         fHisFilm->Fill(position.X(), position.Y(), position.Z(), ene);
      else if(TString(volumeName) == "Stuff")
         fHisWell->Fill(position.X(), position.Y(), position.Z(), ene);

      else if(TString(volumeName) == "Cell"){
         Int_t wellID = XYtoIndex(position.X(), position.Y());
         fHisEachCell20[wellID]->Fill(position.X(), position.Y(), ene);
         fHisEachCell50[wellID]->Fill(position.X(), position.Y(), ene);
      }

   }

}

void THisHandler::AddHists()
{
   HisAll->Add(fHisAll);
   HisSealing->Add(fHisSealing);
   HisWindow->Add(fHisWindow);
   HisFoil->Add(fHisFoil);
   HisHolder->Add(fHisHolder);
   HisCassette->Add(fHisCassette);
   HisAir->Add(fHisAir);
   HisPlate->Add(fHisPlate);
   HisFilm->Add(fHisFilm);
   HisWell->Add(fHisWell);
   for(Int_t i = 0; i < 96; i++){
      HisEachCell20[i]->Add(fHisEachCell20[i]);
      HisEachCell50[i]->Add(fHisEachCell50[i]);
   }
}

void GetHists(TFile *file)
{
   HisAll = (TH3D*)file->Get("HisAll");
   HisSealing = (TH3D*)file->Get("HisSealing");
   HisWindow = (TH3D*)file->Get("HisWindow");
   HisFoil = (TH3D*)file->Get("HisFoil");
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

void *handler(void *ptr)
{
   TThread::Lock();
   Int_t threadID = (Long_t)ptr; // ptr never be more than Int_t size.
   TString fileName = Form("/dev/shm/result_t%d.root", threadID);
   TFile *file = new TFile(fileName);
   if(!file->IsOpen()){ // Do nothing
      TThread::UnLock();
      return 0;
   }
   THisHandler *hisHandler = new THisHandler(file);
   TThread::UnLock();

   hisHandler->FillHists();

   TThread::Lock();
   hisHandler->AddHists();
   TThread::UnLock();
   return 0; // This is neede TThread.
}

void FillHists()
{
   TFile *inputFile = new TFile("tmp.root");
   GetHists(inputFile);
   
   TThread *thread[kThreads];
   for(Int_t iThread = 0; iThread < kThreads; iThread++){
      thread[iThread] = new TThread(Form("thread%03d", iThread),
                                    handler, (void*)((Long_t)iThread));
      thread[iThread]->Run();
   }
   TThread::Ps();
   for(Int_t iThread = 0; iThread < kThreads; iThread++){
      thread[iThread]->Join();
   }
   TThread::Ps();
   
   TFile *outputFile = new TFile("out.root", "RECREATE");
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
   inputFile->Close();
}
