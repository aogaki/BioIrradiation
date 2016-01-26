#ifndef BIEventAction_h
#define BIEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"


class BIEventAction : public G4UserEventAction
{
public:
   BIEventAction(G4bool gridFlag);
   virtual ~BIEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   G4bool fForGrid;
   G4int fHitsCollectionID;
};

#endif
