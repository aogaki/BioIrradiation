#ifndef BIPhysicsList_h
#define BIPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class BIPhysicsList: public G4VModularPhysicsList
{
  public:
    BIPhysicsList();
    virtual ~BIPhysicsList();
};

#endif

