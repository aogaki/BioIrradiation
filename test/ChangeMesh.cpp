/*
  I think ROOT have more smart way.  
*/

TH2D *HisD8;
TH2D *His200;
TH2D *His300;
TH2D *His500;

TH1D *HisEnergy;
TH1D *HisEnergy200;
TH1D *HisEnergy300;
TH1D *HisEnergy500;


void DefineHists()
{
   His200 = new TH2D("His200", "Deposited Energy at D8",
                     45, 9., 18., 45, 0., 9.);
   His200->SetXTitle("X-Coordinate on the Plate (mm)");
   His200->SetYTitle("Y-Coordinate on the Plate (mm)");
   His200->SetZTitle("Deposited Energy (MeV)");

   His300 = new TH2D("His300", "Deposited Energy at D8",
                     30, 9., 18., 30, 0., 9.);
   His300->SetXTitle("X-Coordinate on the Plate (mm)");
   His300->SetYTitle("Y-Coordinate on the Plate (mm)");
   His300->SetZTitle("Deposited Energy (MeV)");

   His500 = new TH2D("His500", "Deposited Energy at D8",
                     18, 9., 18., 18, 0., 9.);
   His500->SetXTitle("X-Coordinate on the Plate (mm)");
   His500->SetYTitle("Y-Coordinate on the Plate (mm)");
   His500->SetZTitle("Deposited Energy (MeV)");

   HisEnergy200 = new TH1D("HisEnergy200", "Deposited Energy at D8",
                           100, 0., 100.);
   HisEnergy200->SetXTitle("Deposited Energy (MeV)");
   
   HisEnergy300 = new TH1D("HisEnergy300", "Deposited Energy at D8",
                           100, 0., 300.);
   HisEnergy300->SetXTitle("Deposited Energy (MeV)");
   
   HisEnergy500 = new TH1D("HisEnergy500", "Deposited Energy at D8",
                           100, 0., 500.);
   HisEnergy500->SetXTitle("Deposited Energy (MeV)");
   
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

void ChangeMesh()
{
   TFile *inputFile = new TFile("999.root", "READ");
   HisD8 = (TH2D*)inputFile->Get("HisD08");

   DefineHists();

   for(Int_t yBin = 1; yBin <= 90; yBin++){
      for(Int_t xBin = 1; xBin <= 90; xBin++){
         Double_t x = HisD8->GetXaxis()->GetBinCenter(xBin);
         Double_t y = HisD8->GetYaxis()->GetBinCenter(yBin);
         Double_t ene = HisD8->GetBinContent(xBin, yBin);

         His200->Fill(x, y, ene);
         His300->Fill(x, y, ene);
         His500->Fill(x, y, ene);
      }
   }
   
   for(Int_t yBin = 1; yBin <= 45; yBin++){
      for(Int_t xBin = 1; xBin <= 45; xBin++){
         if(CheckInsideWell(His200, xBin, yBin)){
            Double_t ene = His200->GetBinContent(xBin, yBin);
            if(ene > 0.) HisEnergy200->Fill(ene);
         }
      }
   }
   
   for(Int_t yBin = 1; yBin <= 30; yBin++){
      for(Int_t xBin = 1; xBin <= 30; xBin++){
         if(CheckInsideWell(His300, xBin, yBin)){
            Double_t ene = His300->GetBinContent(xBin, yBin);
            if(ene > 0.) HisEnergy300->Fill(ene);
         }
      }
   }
   
   for(Int_t yBin = 1; yBin <= 18; yBin++){
      for(Int_t xBin = 1; xBin <= 18; xBin++){
         if(CheckInsideWell(His500, xBin, yBin)){
            Double_t ene = His500->GetBinContent(xBin, yBin);
            if(ene > 0.) HisEnergy500->Fill(ene);
         }
      }
   }
   
   HisD8->Draw("COLZ");
}
