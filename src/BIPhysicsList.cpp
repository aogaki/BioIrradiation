#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "Shielding.hh"

#include "BIPhysicsList.hpp"


BIPhysicsList::BIPhysicsList() 
: G4VModularPhysicsList()
{
  SetDefaultCutValue(1.0*mm);
  SetVerboseLevel(1);
  RegisterPhysics(new G4EmDNAPhysics());
  //G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
}

BIPhysicsList::~BIPhysicsList()
{}
