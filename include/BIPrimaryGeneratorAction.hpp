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
class TH2D;

class BIPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   BIPrimaryGeneratorAction(G4bool oldBeamFlag);
   virtual ~BIPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   void GetParVec(G4double limit);
   G4ParticleGun *fProtonGun;
   
   void SetSourceZ(G4double);
   G4double fZPosition;

//For random generator
   void NewGun();
   void OldGun();
   void (BIPrimaryGeneratorAction::*GunPointer)();
   
   const G4double fDx = (458. - 78.) / 15.;
   const G4double fDy = (332 - 152) / (log10(60.) - log10(20.));

   G4GenericMessenger *fMessenger;
   void DefineCommands();

   TH2D *fHisSource;
   TF1 *fEneFnc;
   G4double fEnergy;
   G4ThreeVector fParVec;
};

#endif
