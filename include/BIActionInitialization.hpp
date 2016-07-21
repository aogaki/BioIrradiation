#ifndef BIActionInitialization_h
#define BIActionInitialization_h 1

#include "globals.hh"
#include "G4VUserActionInitialization.hh"

#include "BIPrimaryGeneratorAction.hpp"

class BIActionInitialization : public G4VUserActionInitialization
{
public:
   BIActionInitialization(BeamType beamType, G4bool gridFlag, G4bool quarterFlag);
   virtual ~BIActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   BeamType fBeamType;
   G4bool fForGrid;
   G4bool fUseQuarter;
};

#endif
