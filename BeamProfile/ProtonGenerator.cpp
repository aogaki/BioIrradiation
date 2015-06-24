#include <time.h>
#include <iostream>
#include <fstream>

#include <TF1.h>
#include <TRandom3.h>


void ProtonGenerator(const Int_t kEvent = 1000000)
{
   Int_t seed = time(NULL);
   gRandom->SetSeed(seed);
   
   TF1 *eneFnc = new TF1("eneFnc", "expo(0)+expo(2)+gaus(4)", 0., 30.);
   eneFnc->SetParameter(0, 2.67164e+01);
   eneFnc->SetParameter(1, -8.35969e-01);
   eneFnc->SetParameter(2, 2.15801e+01);
   eneFnc->SetParameter(3, -7.66820e-02);
   eneFnc->SetParameter(4, 3.79820e+09);
   eneFnc->SetParameter(5, 8.94835e+00);
   eneFnc->SetParameter(6, 3.04310e+00);
   
   ofstream fout("ProtonEnergy.dat");
   for(Int_t i = 0; i < kEvent; i++){
      fout << eneFnc->GetRandom() << endl;
   }
   fout.close();
}
