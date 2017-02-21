#ifndef BIPrimaryGeneratorAction_h
#define BIPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


enum BeamType{ // I wanna use enum class.  But impossible on Grid (Scientific Linux 6).
   kFirstBeam,
   kSecondBeam,
   kThirdBeam,

   kElectronTest,

   kXTest
};

class G4Event;
class G4ParticleGun;
class G4GenericMessenger;
class TF1;
class TH2D;
class TFile;

class BIPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   BIPrimaryGeneratorAction(BeamType beamType, G4bool gridFlag, G4bool quarterFlag);
   virtual ~BIPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   BeamType fBeamType;
   G4bool fForGrid;
   G4bool fUseQuarter;
   
   void GetParVec(G4double limit);
   G4ParticleGun *fParticleGun;
   
   void SetSourceZ(G4double);
   G4double fZPosition;

//For random generator
   void FirstBeamGun();
   void SecondBeamGun();
   void ThirdBeamGun();
   void XBeamGun();
   void (BIPrimaryGeneratorAction::*GunFuncPointer)();
   
   G4double fDx;
   G4double fDy;

   G4GenericMessenger *fMessenger;
   void DefineCommands();

   TH2D *fHisSource;
   TF1 *fEneFnc;
   TF1 *fAngFnc;
   G4double fEnergy;
   G4ThreeVector fParVec;

   G4double fPhiLimit;
};

#endif
