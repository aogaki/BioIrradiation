#ifndef BIPrimaryGeneratorAction_h
#define BIPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"


class G4Event;
class G4ParticleGun;
class G4GenericMessenger;

class BIPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   BIPrimaryGeneratorAction();
   virtual ~BIPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ThreeVector GetParVec(G4double limit);
   G4ParticleGun *fProtonGun;

   G4double fZPosition;

   void ReadTable();
   G4double fEnergyTable[100000000];
};

#endif
