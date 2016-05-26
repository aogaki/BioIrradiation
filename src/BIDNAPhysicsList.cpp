#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "Shielding.hh"

#include "BIDNAPhysicsList.hpp"


BIDNAPhysicsList::BIDNAPhysicsList() 
: G4VModularPhysicsList()
{
   SetDefaultCutValue(1.0*nm);
   SetVerboseLevel(1);

   // Now I need only for proton inside cell
   RegisterPhysics(new G4EmDNAPhysics());
   
   
//RegisterPhysics(new G4EmStandardPhysics_option4());
//G4EmParameters::Instance()->AddDNA("World", "Opt0");
//G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
}

BIDNAPhysicsList::~BIDNAPhysicsList()
{}
