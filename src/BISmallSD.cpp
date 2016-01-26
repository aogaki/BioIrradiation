#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VProcess.hh"

#include "BISmallSD.hpp"
#include "BISmallHit.hpp"


BISmallSD::BISmallSD(const G4String &name,
                       const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

BISmallSD::~BISmallSD()
{}

void BISmallSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new BISmallHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool BISmallSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   // Recording all steps.  Huge file size!
   BISmallHit *newHit = new BISmallHit();

   G4double depositEnergy = step->GetTotalEnergyDeposit();
   newHit->SetDepositEnergy(depositEnergy);

   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   G4ThreeVector position =  postStepPoint->GetPosition();
   newHit->SetPosition(position);

   fHitsCollection->insert(newHit);
   return true;
}
