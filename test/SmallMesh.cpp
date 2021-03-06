#include <iostream>

#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>
#include <TAxis.h>


TH2D *HisEachWell[96];
TH2D *HisPlate;
TH2D *HisEneDis;

TH1D *HisEnergy[96];
TF1 *FitEnergy[96];
TH1D *HisTest;

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
   const Double_t wellPitch = 9.;
   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "His20" + XYtoWell(x, y);
         HisEachWell[hisIt] = (TH2D*)file->Get(name);
         HisEachWell[hisIt]->SetXTitle("X-Coordinate on the Plate (mm)");
         HisEachWell[hisIt]->SetYTitle("Y-Coordinate on the Plate (mm)");
         HisEachWell[hisIt]->SetZTitle("Deposited Energy (MeV)");
         hisIt++;
      }
   }
}

void DefineHists()
{
   HisPlate = new TH2D("HisPlate", "Average Deposited Energy in Each Well (MeV)",
                      12, 0, 12, 8, 0, 8);
   HisPlate->SetStats(0);
   HisPlate->SetCanExtend(TH1::kAllAxes);

   HisEneDis = new TH2D("HisEneDis", "Energy Distrbution (MeV) vs Distance from Center of Plate (mm)",
                        50, 0, 50, 70, 0, 70);
   
   HisTest = new TH1D("HisTest", "test", 500, 0, 50);
   HisTest->SetXTitle("Deposited Energy (MeV)");

   UInt_t hisIt = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "HisEnergy" + XYtoWell(x, y);
         TString title = "Deposited Energy at " + XYtoWell(x, y);
         HisEnergy[hisIt] = new TH1D(name, title, 2000, 0, 20);
         HisEnergy[hisIt++]->SetXTitle("Deposited Energy (MeV)");
      }
   }
}

Bool_t CheckInsideWell(TH2D *hist, Int_t xBin, Int_t yBin)
{
   TAxis *xAxis = hist->GetXaxis();
   TAxis *yAxis = hist->GetYaxis();

   Double_t xCenter = (xAxis->GetXmax() + xAxis->GetXmin()) / 2.;
   Double_t yCenter = (yAxis->GetXmax() + yAxis->GetXmin()) / 2.;

   Double_t x = xAxis->GetBinCenter(xBin);
   if(x < xCenter) x -= xAxis->GetBinWidth(xBin) / 2.;
   else x += xAxis->GetBinWidth(xBin) / 2.;
   x -= xCenter;
   
   Double_t y = yAxis->GetBinCenter(yBin);
   if(y < yCenter) y -= yAxis->GetBinWidth(yBin) / 2.;
   else y += yAxis->GetBinWidth(yBin) / 2.;
   y -= yCenter;
   
   Double_t distance = sqrt(x*x + y*y);

   if(distance <= 6.39 / 2.) // well size
      return kTRUE;
   else
      return kFALSE;
}

Double_t GetDistance(TH2D *hist, Int_t xBin, Int_t yBin)
{
   TAxis *xAxis = hist->GetXaxis();
   TAxis *yAxis = hist->GetYaxis();
   Double_t x = xAxis->GetBinCenter(xBin);
   Double_t y = yAxis->GetBinCenter(yBin);
   return sqrt(x*x + y*y);   
}

void GetCellInfo(Int_t i)
{
   TH2D *hist = HisEachWell[i];
   const Int_t kXbin = hist->GetXaxis()->GetNbins();
   const Int_t kYbin = hist->GetYaxis()->GetNbins();
   
   for(Int_t y = 1; y <= kYbin; y++){
      for(Int_t x = 1; x <= kXbin; x++){
         if(CheckInsideWell(hist, x, y)){
            Double_t ene = hist->GetBinContent(x, y);
            if(ene > 0.){
               HisTest->Fill(ene);
               HisEnergy[i]->Fill(ene);
               Double_t dis = GetDistance(hist, x, y);
               HisEneDis->Fill(ene, dis);
            }
         }
      }
   }
}

void FitHists()
{
   UInt_t i = 0;
   for(Double_t y = -36.; y < 32.; y += 9.){
      for(Double_t x = -54.; x < 50.; x += 9.){
         TString name = "FitEnergy" + XYtoWell(x, y);
         FitEnergy[i] = new TF1(name, "gaus");
         HisEnergy[i]->Fit(FitEnergy[i]);

         Double_t mean = FitEnergy[i]->GetParameter(1);
         Double_t sigma = FitEnergy[i]->GetParameter(2);
         FitEnergy[i]->SetRange(mean - 0.5 * sigma, mean + 0.5 * sigma);
         HisEnergy[i]->Fit(FitEnergy[i], "R");

         mean = FitEnergy[i]->GetParameter(1);
         sigma = FitEnergy[i]->GetParameter(2);
         FitEnergy[i]->SetRange(mean - 0.5 * sigma, mean + 0.5 * sigma);
         HisEnergy[i]->Fit(FitEnergy[i], "R");
         i++;
      }
   }

}

Int_t CountMesh(TH2D *hist)
{
   Int_t meshCounter = 0;
   const Int_t kXbin = hist->GetXaxis()->GetNbins();
   const Int_t kYbin = hist->GetYaxis()->GetNbins();
   for(Int_t x = 1; x <= kXbin; x++){
      for(Int_t y = 1; y <= kXbin; y++){
         if(CheckInsideWell(hist, x, y)) meshCounter++;
      }
   }

   return meshCounter;
}

void SmallMesh()
{
   //TFile *inputFile = new TFile("999.root", "READ");
   TFile *inputFile = new TFile("1499.root", "READ");
   GetHists(inputFile);

   DefineHists();
   
   for(Int_t i = 0; i < 96; i++){
      GetCellInfo(i);
   }

   FitHists();
   
   TString xLabel[12];
   for(Int_t i = 0; i < 12; i++) xLabel[i] = Form("%02d", i + 1);
   TString yLabel[8] = {"H", "G", "F", "E", "D", "C", "B", "A"};

   for(Int_t y = 0; y < 8; y++){
      for(Int_t x = 0; x < 12; x++){
         //Double_t ene = HisEachWell[x + y * 12]->Integral();
         Double_t ene = HisEnergy[x + y * 12]->GetMean();
         //Double_t ene = FitEnergy[x + y * 12]->GetParameter(1);
         HisPlate->Fill(xLabel[x], yLabel[y], ene);
      }
   }

   HisPlate->SetZTitle("Deposited Energy (MeV)");
   HisPlate->Draw("COLZ");
   //HisEnergy[55]->Draw();

   // Count effective mesh

   cout << CountMesh(HisEachWell[55]) << endl;
}
