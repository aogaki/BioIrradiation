#include <TChain.h>
#include <TProof.h>
#include <TProofLite.h>


void ActivatePROOF(TChain *chain, Int_t nThreads = 0)
{
   TProof *proof = TProof::Open("");
   proof->SetProgressDialog(kFALSE);
   if(nThreads > 0) proof->SetParallel(nThreads);

   chain->SetProof();
}

void run()
{
   TChain *chain = new TChain("BI");
   chain->Add("/dev/shm/result*.root");

   const Int_t kNThreads = chain->GetNtrees();
   //ActivatePROOF(chain, kNThreads);
   ActivatePROOF(chain);
   
   chain->Process("EventAnalyzer.C+O");
}
