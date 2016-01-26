#ifndef BIActionInitialization_h
#define BIActionInitialization_h 1

#include "globals.hh"
#include "G4VUserActionInitialization.hh"


class BIActionInitialization : public G4VUserActionInitialization
{
public:
   BIActionInitialization(G4bool beamFlag);
   virtual ~BIActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   G4bool fUseOldBeam;
};

#endif
