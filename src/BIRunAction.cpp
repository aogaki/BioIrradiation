#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "g4root.hh"

#include "BIRunAction.hpp"


BIRunAction::BIRunAction()
   : G4UserRunAction()
{}

BIRunAction::~BIRunAction()
{
   delete G4AnalysisManager::Instance();
}

void BIRunAction::BeginOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->SetVerboseLevel(1);
    G4cout << "Using " << anaMan->GetType()
           << " analysis manager" << G4endl;
    G4String fileName = "result";
    anaMan->OpenFile(fileName);

    // Ntuple
    anaMan->CreateNtuple("BI", "Laser and Gamma");

    anaMan->CreateNtupleIColumn(0, "EventID");
    anaMan->CreateNtupleIColumn(0, "PDGCode");    
    anaMan->CreateNtupleDColumn(0, "DepositEnergy");
    anaMan->CreateNtupleDColumn(0, "Time");
    anaMan->CreateNtupleSColumn(0, "VolumeName");

    anaMan->CreateNtupleDColumn(0, "x");
    anaMan->CreateNtupleDColumn(0, "y");
    anaMan->CreateNtupleDColumn(0, "z");

    anaMan->CreateNtupleDColumn(0, "preX");
    anaMan->CreateNtupleDColumn(0, "preY");
    anaMan->CreateNtupleDColumn(0, "preZ");

    anaMan->CreateNtupleDColumn(0, "vx");
    anaMan->CreateNtupleDColumn(0, "vy");
    anaMan->CreateNtupleDColumn(0, "vz");

    anaMan->CreateNtupleIColumn(0, "IsLast");

    anaMan->CreateNtupleIColumn(0, "TrackID");
    
    anaMan->CreateNtuple("InitPar", "Initial Parameters");
    anaMan->CreateNtupleIColumn(1, "PDGCode");
    anaMan->CreateNtupleDColumn(1, "TotalEnergy");
    anaMan->CreateNtupleDColumn(1, "vx");
    anaMan->CreateNtupleDColumn(1, "vy");
    anaMan->CreateNtupleDColumn(1, "vz");

    anaMan->FinishNtuple();
}

void BIRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
    anaMan->Write();
    anaMan->CloseFile();
}

