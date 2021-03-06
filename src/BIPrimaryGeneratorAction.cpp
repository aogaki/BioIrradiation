#include <fstream>
#include <math.h>
#include <time.h>

#include <TFile.h>
#include <TH2.h>
#include <TF1.h>
#include <TRandom3.h>

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "g4root.hh"
#include "G4GenericMessenger.hh"
#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"

#include "BIPrimaryGeneratorAction.hpp"


G4int nEveInPGA = 0; // Global variable change to local? 
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;

// For using ROOT classes.
// When using ROOT classes, it have to be thread local or mutex locked.
// I don't know _G4MT_TLS_ is truely needed or not.
// https://indico.cern.ch/event/226961/material-old/0/0?contribId=0
// In case without, this code looks like working well...
//G4ThreadLocal TF1 *fEneFnc_G4MT_TLS_ = nullptr;

BIPrimaryGeneratorAction::BIPrimaryGeneratorAction(BeamType beamType, G4bool gridFlag, G4bool quarterFlag)
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr),
     fHisSource(nullptr),
     fEneFnc(nullptr),
     fAngFnc(nullptr)
{
   fBeamType = beamType;

   fForGrid = gridFlag;

   fUseQuarter = quarterFlag;
   if(fUseQuarter) fPhiLimit = 0.5 * CLHEP::pi;
   else fPhiLimit = 2. * CLHEP::pi;
   
   fDx = (458. - 78.) / 15.;
   fDy = (332 - 152) / (log10(60.) - log10(20.));

   G4AutoLock lock(&mutexInPGA);
   
   Int_t seed = G4UniformRand() * 1000000;
   G4cout << "Seed of PGA = " << seed << G4endl;
   gRandom->SetSeed(seed);
   
   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   fZPosition = -300.*mm;
   //fZPosition = -160.*mm; // Minimum distance for new beam

   fParVec = G4ThreeVector(0., 0., 1.);
   
   G4ParticleTable *parTable = G4ParticleTable::GetParticleTable();
   if(fBeamType == kXTest){
      G4ParticleDefinition *gamma = parTable->FindParticle("gamma");
      fParticleGun->SetParticleDefinition(gamma);
   }
   else if(fBeamType == kIonTest){ // Still we can not use IonTable?
      G4ParticleDefinition *particle
         //= parTable->FindParticle("chargedgeantino");
         = parTable->FindParticle("GenericIon");
      fParticleGun->SetParticleDefinition(particle);
      fFirstShot = true;
   }
   else {
      G4ParticleDefinition *proton = parTable->FindParticle("proton");
      fParticleGun->SetParticleDefinition(proton);
   }
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fParticleGun->SetParticleMomentumDirection(fParVec);
   fParticleGun->SetParticleEnergy(fEnergy);

   TFile *file = new TFile("randomSource.root", "OPEN");
   fHisSource = (TH2D*)file->Get("HisMap");
   fHisSource->SetName("fHisSource");
   fHisSource->SetDirectory(0);
   file->Close();
   delete file;
   
   DefineCommands();

// Pointer of Function is not good for readable code?
// And also, use if statement does not make program slow.
   if(fBeamType == kFirstBeam){
      //if(!fEneFnc_G4MT_TLS_) fEneFnc_G4MT_TLS_ = new TF1("fEneFnc", "expo(0)+expo(2)+gaus(4)", 0., 30.);
      //fEneFnc = fEneFnc_G4MT_TLS_;
      fEneFnc = new TF1("fEneFnc", "expo(0)+expo(2)+gaus(4)", 0., 30.);
      fEneFnc->SetParameter(0, 2.67164e+01);
      fEneFnc->SetParameter(1, -8.35969e-01);
      fEneFnc->SetParameter(2, 2.15801e+01);
      fEneFnc->SetParameter(3, -7.66820e-02);
      fEneFnc->SetParameter(4, 3.79820e+09);
      fEneFnc->SetParameter(5, 8.94835e+00);
      fEneFnc->SetParameter(6, 3.04310e+00);

      GunFuncPointer = &BIPrimaryGeneratorAction::FirstBeamGun;
   }
   else if(fBeamType == kSecondBeam){
      GunFuncPointer = &BIPrimaryGeneratorAction::SecondBeamGun;
   }
   else if(fBeamType == kThirdBeam){
      fEneFnc = new TF1("EneFnc", "exp([0]*x)", 0., 100.);
      fEneFnc->SetParameter(0, -4.77205e-02);
      
      fAngFnc = new TF1("fAngFnc", "exp([0]*x)", 0., 20.);
      fAngFnc->SetParameter(0, -8.98131e-02);

      GunFuncPointer = &BIPrimaryGeneratorAction::ThirdBeamGun;
   }
   else if(fBeamType == kXTest){
      GunFuncPointer = &BIPrimaryGeneratorAction::XBeamGun;
      fEnergy = 1. * MeV;
   }
   else if(fBeamType == kIonTest){
      GunFuncPointer = &BIPrimaryGeneratorAction::IonBeamGun;
      fEnergy = 1. * MeV;
   }
   else{
      G4cout << "Beam type is wrong.  Please check it." << G4endl;
      exit(0);
   }
}

BIPrimaryGeneratorAction::~BIPrimaryGeneratorAction()
{
   //G4AutoLock lock(&mutexInPGA);
   if(fEneFnc != nullptr) {delete fEneFnc; fEneFnc = nullptr;}
   if(fAngFnc != nullptr) {delete fAngFnc; fAngFnc = nullptr;}
   if(fHisSource != nullptr) {delete fHisSource; fHisSource = nullptr;}
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
}

void BIPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   (this->*GunFuncPointer)();
   
   fParticleGun->SetParticleMomentumDirection(fParVec);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fParticleGun->SetParticleEnergy(fEnergy);
   fParticleGun->GeneratePrimaryVertex(event);

   if(!fForGrid){
      G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
      anaMan->FillNtupleIColumn(1, 0, 11);
      anaMan->FillNtupleDColumn(1, 1, fEnergy);
      anaMan->FillNtupleDColumn(1, 2, fParVec.x());
      anaMan->FillNtupleDColumn(1, 3, fParVec.y());
      anaMan->FillNtupleDColumn(1, 4, fParVec.z());
      anaMan->AddNtupleRow(1);

      G4AutoLock lock(&mutexInPGA);
      if (nEveInPGA++ % 10000 == 0)
         G4cout << nEveInPGA - 1 << " events done" << G4endl;
   }
}

void BIPrimaryGeneratorAction::GetParVec(G4double limit)
{
   G4double theta = acos(1. - G4UniformRand() * (1. - cos(limit)));
   G4double phi = G4UniformRand() * fPhiLimit;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   fParVec = G4ThreeVector(vx, vy, vz);
}

void BIPrimaryGeneratorAction::FirstBeamGun()
{
   G4double coneTheta = 15.*deg;
   GetParVec(coneTheta);
   G4AutoLock lock(&mutexInPGA);
   fEnergy = fEneFnc->GetRandom() * MeV;
   // no need to unlock.
}

void BIPrimaryGeneratorAction::SecondBeamGun()
{
   G4double x, y;
   fHisSource->GetRandom2(x, y);

   fEnergy = pow(10., (y - 152.) / fDy) * 20 * MeV;

   G4double theta = CLHEP::pi * ((x - 78.) / fDx) / 180.;
   G4double phi = G4UniformRand() * fPhiLimit;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   fParVec = G4ThreeVector(vx, vy, vz);
}

void BIPrimaryGeneratorAction::ThirdBeamGun()
{
   // TF1 is not thread safe.  
   G4AutoLock lock(&mutexInPGA);
   fEnergy = fEneFnc->GetRandom() * MeV;
   lock.unlock();
   
   G4double theta;
   if(fEnergy > 30.*MeV){
      lock.lock();
      theta = fAngFnc->GetRandom() * deg;
      lock.unlock();
   }
   else
      theta = acos(1. - G4UniformRand() * (1. - cos(20.*deg)));
   G4double phi = G4UniformRand() * fPhiLimit;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   fParVec = G4ThreeVector(vx, vy, vz);
}

void BIPrimaryGeneratorAction::XBeamGun()
{
   G4double coneTheta = 15.*deg;
   GetParVec(coneTheta);
   fEnergy = CLHEP::RandExponential::shoot(10);
   // no need to unlock.
}

void BIPrimaryGeneratorAction::IonBeamGun()
{
   if(fFirstShot){
      fFirstShot = false;
      // Carbon 6+
      G4int Z = 6, A = 12;
      G4double ionCharge   = 6.*eplus;
      G4double excitEnergy = 0.*keV;
    
      G4ParticleDefinition *ion
         = G4IonTable::GetIonTable()->GetIon(Z, A, excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
   }

   G4double coneTheta = 15.*deg;
   GetParVec(coneTheta);
   fEnergy = CLHEP::RandExponential::shoot(10);
   // no need to unlock.
}

void BIPrimaryGeneratorAction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/BI/Primary/", 
                                       "Beam control");

   // z position
   G4GenericMessenger::Command &sourceZCmd
      = fMessenger->DeclareMethodWithUnit("Z", "mm",
                                          &BIPrimaryGeneratorAction::SetSourceZ, 
                                          "Set the length of source.");

   sourceZCmd.SetParameterName("z", true);
   sourceZCmd.SetRange("z>=-1000. && z<1000.");
   sourceZCmd.SetDefaultValue("-300");
}

void BIPrimaryGeneratorAction::SetSourceZ(G4double z)
{
   fZPosition = z;
}
