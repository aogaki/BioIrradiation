#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "g4root.hh"

#include "BIEventAction.hpp"
#include "BICommonHit.hpp"


BIEventAction::BIEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{
   // set printing per each event
   //G4RunManager::GetRunManager()->SetPrintProgress(1);
}

BIEventAction::~BIEventAction()
{}

void BIEventAction::BeginOfEventAction(const G4Event *)
{
    if (fHitsCollectionID == -1) {
       G4SDManager *manager = G4SDManager::GetSDMpointer();
       fHitsCollectionID = manager->GetCollectionID("Common/CommonHitsCollection");
       if (fHitsCollectionID == -1) {
          G4cout << "Common/CommonHitsCollection not found" << G4endl;
          exit(0);
       }
    }
}

void BIEventAction::EndOfEventAction(const G4Event *event)
{
   G4HCofThisEvent *hce = event->GetHCofThisEvent();
   if (!hce) {
      G4cout << "HCE not found: EndOfEventAction@BIEventAction" << G4endl;
      exit(0);
   }
   BICommonHitsCollection *hitsCollection =
      static_cast<BICommonHitsCollection *>(hce->GetHC(fHitsCollectionID));
   if (!hitsCollection) {
      G4cout << "HC not found: EndOfEventAction@BIEventAction" << G4endl;
      exit(0);
   }

   G4int eventID = event->GetEventID();

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();

   const G4int kHit = hitsCollection->entries();
   G4bool CellHit = false;
   /*
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      BICommonHit *newHit = (*hitsCollection)[iHit];
      G4String name = newHit->GetVolumeName();
      G4double ene = newHit->GetDepositEnergy();
      if(name.contains("Cell") && ene > 0.){
         CellHit = true;
         break;
      }
   }
*/
   CellHit = true;
   if(CellHit){
      for (G4int iHit = 0; iHit < kHit; iHit++) {
         BICommonHit *newHit = (*hitsCollection)[iHit];

         anaMan->FillNtupleIColumn(0, 0, eventID); // EventID

         G4int pdgCode = newHit->GetPDGCode();
         anaMan->FillNtupleIColumn(0, 1, pdgCode);

         G4double depositEnergy = newHit->GetDepositEnergy();
         anaMan->FillNtupleDColumn(0, 2, depositEnergy);

         G4double time = newHit->GetTime();
         anaMan->FillNtupleDColumn(0, 3, time);

         G4String volumeName = newHit->GetVolumeName();
         anaMan->FillNtupleSColumn(0, 4, volumeName);

         G4ThreeVector position = newHit->GetPosition();
         anaMan->FillNtupleDColumn(0, 5, position.x());
         anaMan->FillNtupleDColumn(0, 6, position.y());
         anaMan->FillNtupleDColumn(0, 7, position.z());

         G4ThreeVector prePosition = newHit->GetPrePosition();
         anaMan->FillNtupleDColumn(0, 8, prePosition.x());
         anaMan->FillNtupleDColumn(0, 9, prePosition.y());
         anaMan->FillNtupleDColumn(0, 10, prePosition.z());

         G4ThreeVector momentum = newHit->GetMomentum();
         anaMan->FillNtupleDColumn(0, 11, momentum.x());
         anaMan->FillNtupleDColumn(0, 12, momentum.y());
         anaMan->FillNtupleDColumn(0, 13, momentum.z());

         G4int isLast = newHit->GetIsLast();
         anaMan->FillNtupleIColumn(0, 14, isLast);

         G4int trackID = newHit->GetTrackID();
         anaMan->FillNtupleIColumn(0, 15, trackID);
         
         anaMan->AddNtupleRow(0);
      }
   }
}

