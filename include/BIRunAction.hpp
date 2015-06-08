#ifndef BIRunAction_h
#define BIRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;

class BIRunAction: public G4UserRunAction
{
public:
   BIRunAction();
   virtual ~BIRunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);
};

#endif
