//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 11 14:27:04 2015 by ROOT version 6.04/06
// from TTree BI/Laser and Gamma
// found on file: /dev/shm/result_t0.root
//////////////////////////////////////////////////////////

#ifndef EventAnalyzer_h
#define EventAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TString.h>

// Header file for the classes stored in the TTree if any.

class EventAnalyzer : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           EventID;
   Int_t           PDGCode;
   Double_t        DepositEnergy;
   Double_t        Time;
   Char_t          VolumeName[9];
   Double_t        x;
   Double_t        y;
   Double_t        z;
   Double_t        preX;
   Double_t        preY;
   Double_t        preZ;
   Double_t        vx;
   Double_t        vy;
   Double_t        vz;
   Int_t           IsLast;
   Int_t           TrackID;

   // List of branches
   TBranch        *b_EventID;   //!
   TBranch        *b_PDGCode;   //!
   TBranch        *b_DepositEnergy;   //!
   TBranch        *b_Time;   //!
   TBranch        *b_VolumeName;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_z;   //!
   TBranch        *b_preX;   //!
   TBranch        *b_preY;   //!
   TBranch        *b_preZ;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_IsLast;   //!
   TBranch        *b_TrackID;   //!

   EventAnalyzer(TTree */*tree*/ =0);
   virtual ~EventAnalyzer();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   // User functions
   TString XYtoWell(Double_t x, Double_t y);
   Int_t XYtoIndex(Double_t x, Double_t y);
   
   // Histograms
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
   TH2D *fHisCell;
   TH2D *fHisEachCell[96];
      
   ClassDef(EventAnalyzer,0);

};

#endif

#ifdef EventAnalyzer_cxx
void EventAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

  
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventID", &EventID, &b_EventID);
   fChain->SetBranchAddress("PDGCode", &PDGCode, &b_PDGCode);
   fChain->SetBranchAddress("DepositEnergy", &DepositEnergy, &b_DepositEnergy);
   fChain->SetBranchAddress("Time", &Time, &b_Time);
   fChain->SetBranchAddress("VolumeName", VolumeName, &b_VolumeName);
   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("z", &z, &b_z);
   fChain->SetBranchAddress("preX", &preX, &b_preX);
   fChain->SetBranchAddress("preY", &preY, &b_preY);
   fChain->SetBranchAddress("preZ", &preZ, &b_preZ);
   fChain->SetBranchAddress("vx", &vx, &b_vx);
   fChain->SetBranchAddress("vy", &vy, &b_vy);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("IsLast", &IsLast, &b_IsLast);
   fChain->SetBranchAddress("TrackID", &TrackID, &b_TrackID);
}

Bool_t EventAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EventAnalyzer_cxx
