#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VProcess.hh"

#include "BICommonSD.hpp"
#include "BICommonHit.hpp"


BICommonSD::BICommonSD(const G4String &name,
                       const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

BICommonSD::~BICommonSD()
{}

void BICommonSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new BICommonHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool BICommonSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   // Recording all steps.  Huge file size!
   BICommonHit *newHit = new BICommonHit();

   G4StepPoint *preStepPoint = step->GetPreStepPoint();
   G4String volumeName = preStepPoint->GetPhysicalVolume()->GetName();
   newHit->SetVolumeName(volumeName);

   //G4Track *track = step->GetTrack();   
   //G4ParticleDefinition *particle = track->GetDefinition();
   //G4int pdgCode = particle->GetPDGEncoding();
   //newHit->SetPDGCode(pdgCode);

   //G4int trackID = track->GetTrackID();
   //newHit->SetTrackID(trackID);
   
   G4double depositEnergy = step->GetTotalEnergyDeposit();
   newHit->SetDepositEnergy(depositEnergy);

   //G4double kineticEnergy = track->GetKineticEnergy();
   //newHit->SetKineticEnergy(kineticEnergy);

   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   //G4double time = postStepPoint->GetGlobalTime();
   //newHit->SetTime(time);

   G4ThreeVector position =  postStepPoint->GetPosition();
   newHit->SetPosition(position);
   
   //G4ThreeVector prePosition =  preStepPoint->GetPosition();
   //newHit->SetPrePosition(prePosition);
   
   //G4ThreeVector momentum =  postStepPoint->GetMomentum();
   //newHit->SetMomentum(momentum);

   //G4int isLast = (G4int)step->IsLastStepInVolume();
   //newHit->SetIsLast(isLast);

   fHitsCollection->insert(newHit);
   return true;
}
