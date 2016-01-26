#include "BIActionInitialization.hpp"
#include "BIPrimaryGeneratorAction.hpp"
#include "BIRunAction.hpp"
#include "BIEventAction.hpp"


BIActionInitialization::BIActionInitialization(G4bool beamFlag)
   : G4VUserActionInitialization()
{
   fUseOldBeam = beamFlag;
}

BIActionInitialization::~BIActionInitialization()
{}

void BIActionInitialization::BuildForMaster() const
{
   SetUserAction(new BIRunAction());
}

void BIActionInitialization::Build() const
{
   SetUserAction(new BIPrimaryGeneratorAction(fUseOldBeam));
   SetUserAction(new BIRunAction());
   SetUserAction(new BIEventAction());
}
