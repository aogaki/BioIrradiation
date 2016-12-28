#ifndef BIEventAction_h
#define BIEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "BICommonHit.hpp"


class BIEventAction : public G4UserEventAction
{
public:
   BIEventAction(G4bool gridFlag);
   virtual ~BIEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   G4bool fForGrid;

   BICommonHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;

   G4int fHitsCollectionID;
};

#endif
