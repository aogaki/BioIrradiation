#include <iostream>

#include <TFile.h>
#include <TH2.h>
#include <TH3.h>
#include <TBrowser.h>
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

TH2D *SliceAll[30];
TH2D *SliceSealing[6];
TH2D *SliceWindow;
TH2D *SliceHolder[2];
TH2D *SliceCassette[23];
TH2D *SlicePlate[12];
TH2D *SliceFilm;

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
}

void GetSlices(Double_t start, Double_t end, TH3D *source, TH2D **target)
{
   TAxis *zAxis = source->GetZaxis();
   Int_t startBin = zAxis->FindBin(start);
   Int_t endBin = zAxis->FindBin(end);
   Int_t it = 0;
   for(Int_t iBin = startBin; iBin < endBin; iBin += 10){
      zAxis->SetRange(iBin, iBin + 9);
      //cout << iBin << endl;
      TString name = Form("yx%d", it);
      target[it++] = (TH2D*)source->Project3D(name);
   }

   source->GetZaxis()->UnZoom();
   //zAxis->UnZoom();
}

void MakeSlices()
{
   TFile *inputFile = new TFile("999.root");
   GetHists(inputFile);

   Double_t airGap = 5.0;
   Double_t plateT = 1.;
   Double_t wellH = 10.9;
   Double_t plateH = wellH + plateT;
   Double_t cassetteH = 22.5;
   Double_t sealingT = 5.3;
   Double_t windowT = 0.5;
   Double_t holderT = 1.7;
   Double_t total = sealingT + windowT + holderT + cassetteH;


   Double_t start, end;
   
   // All volumes
   start = -sealingT - windowT / 2.;
   end = windowT / 2. + holderT + cassetteH;
   GetSlices(start, end, HisAll, SliceAll);

   // Sealing
   start = -sealingT - windowT / 2.;
   end = -windowT / 2.;
   GetSlices(start, end, HisSealing, SliceSealing);

   SliceWindow = (TH2D*)HisWindow->Project3D("yx");
   
   // Holder
   start = windowT / 2.;
   end = windowT / 2. + holderT;
   GetSlices(start, end, HisHolder, SliceHolder);
   
   // Cassette
   start = windowT / 2. + holderT;
   end = windowT / 2. + holderT + cassetteH;
   GetSlices(start, end, HisCassette, SliceCassette);
   
   // Plate
   start = windowT / 2. + airGap;
   end = windowT / 2. + airGap + plateH;
   GetSlices(start, end, HisPlate, SlicePlate);

   SliceFilm = (TH2D*)HisFilm->Project3D("yx");
   
   new TBrowser;
   HisAll->Draw();
}
