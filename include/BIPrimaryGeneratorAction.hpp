#ifndef BIPrimaryGeneratorAction_h
#define BIPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


class G4Event;
class G4ParticleGun;
class G4GenericMessenger;
class TF1;

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
   
   TF1 *fEneFnc;
};

#endif
