#ifndef BISmallSD_h
#define BISmallSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

#include "BISmallHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class BISmallSD: public G4VSensitiveDetector
{
public:
   BISmallSD(const G4String &name,
              const G4String &hitsCollectionName);
   virtual ~BISmallSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   BISmallHitsCollection *fHitsCollection;
};

#endif
