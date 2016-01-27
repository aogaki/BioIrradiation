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
   BIPrimaryGeneratorAction(G4bool oldBeamFlag, G4bool gridFlag);
   virtual ~BIPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4bool fUseOldGun;
   G4bool fForGrid;
   
   void GetParVec(G4double limit);
   G4ParticleGun *fProtonGun;
   
   void SetSourceZ(G4double);
   G4double fZPosition;

//For random generator
   void NewGun();
   void OldGun();
   void (BIPrimaryGeneratorAction::*GunPointer)();
   
   static const G4double kDx = (458. - 78.) / 15.;
   static const G4double kDy = (332 - 152) / (log10(60.) - log10(20.));

   G4GenericMessenger *fMessenger;
   void DefineCommands();

   TH2D *fHisSource;
   TF1 *fEneFnc;
   G4double fEnergy;
   G4ThreeVector fParVec;
};

#endif
