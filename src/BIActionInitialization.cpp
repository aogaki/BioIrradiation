#include "BIActionInitialization.hpp"
#include "BIPrimaryGeneratorAction.hpp"
#include "BIRunAction.hpp"
#include "BIEventAction.hpp"


BIActionInitialization::BIActionInitialization()
   : G4VUserActionInitialization()
{}

BIActionInitialization::~BIActionInitialization()
{}

void BIActionInitialization::BuildForMaster() const
{
   SetUserAction(new BIRunAction);
}

void BIActionInitialization::Build() const
{
   SetUserAction(new BIPrimaryGeneratorAction());
   SetUserAction(new BIRunAction());
   SetUserAction(new BIEventAction());
}
