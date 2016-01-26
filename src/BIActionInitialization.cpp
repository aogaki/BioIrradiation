#include "BIActionInitialization.hpp"
#include "BIPrimaryGeneratorAction.hpp"
#include "BIRunAction.hpp"
#include "BIEventAction.hpp"


BIActionInitialization::BIActionInitialization(G4bool beamFlag, G4bool gridFlag)
   : G4VUserActionInitialization()
{
   fUseOldBeam = beamFlag;
   fForGrid = gridFlag;
}

BIActionInitialization::~BIActionInitialization()
{}

void BIActionInitialization::BuildForMaster() const
{
   SetUserAction(new BIRunAction(fForGrid));
}

void BIActionInitialization::Build() const
{
   SetUserAction(new BIPrimaryGeneratorAction(fUseOldBeam, fForGrid));
   SetUserAction(new BIRunAction(fForGrid));
   SetUserAction(new BIEventAction(fForGrid));
}
