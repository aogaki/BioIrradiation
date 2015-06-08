#ifndef BICommonSD_h
#define BICommonSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

#include "BICommonHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class BICommonSD: public G4VSensitiveDetector
{
public:
   BICommonSD(const G4String &name,
              const G4String &hitsCollectionName);
   virtual ~BICommonSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   BICommonHitsCollection *fHitsCollection;
};

#endif
