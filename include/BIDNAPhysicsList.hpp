#ifndef BIDNAPhysicsList_h
#define BIDNAPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class BIDNAPhysicsList: public G4VModularPhysicsList
{
  public:
    BIDNAPhysicsList();
    virtual ~BIDNAPhysicsList();
};

#endif

