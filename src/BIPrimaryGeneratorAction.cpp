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

#include "BIPrimaryGeneratorAction.hpp"


G4int nEveInPGA = 0; // Global variable change to local? 
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;
/*
// For using ROOT classes.
// When using ROOT classes, it have to be thread local or mutex locked.
// I don't know _G4MT_TLS_ is truely needed or not.
// https://indico.cern.ch/event/226961/material-old/0/0?contribId=0
// In case without, this code looks like working well...
G4ThreadLocal TF1 *fEneFnc_G4MT_TLS_ = 0;
*/
BIPrimaryGeneratorAction::BIPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fProtonGun(0),
     fEneFnc(nullptr)
{
   G4AutoLock lock(&mutexInPGA);

   Int_t seed = time(NULL) + G4Threading::G4GetThreadId() * 100000;
   gRandom->SetSeed(seed);
/*
   if(!fEneFnc_G4MT_TLS_) fEneFnc_G4MT_TLS_ = new TF1("fEneFnc", "expo(0)+expo(2)+gaus(4)", 0., 30.);
   fEneFnc = fEneFnc_G4MT_TLS_;
*/
   fEneFnc = new TF1("fEneFnc", "expo(0)+expo(2)+gaus(4)", 0., 30.);
   fEneFnc->SetParameter(0, 2.67164e+01);
   fEneFnc->SetParameter(1, -8.35969e-01);
   fEneFnc->SetParameter(2, 2.15801e+01);
   fEneFnc->SetParameter(3, -7.66820e-02);
   fEneFnc->SetParameter(4, 3.79820e+09);
   fEneFnc->SetParameter(5, 8.94835e+00);
   fEneFnc->SetParameter(6, 3.04310e+00);
   fEnergy = fEneFnc->GetRandom() * MeV;   
   
   G4int nPar = 1;
   fProtonGun = new G4ParticleGun(nPar);

   fZPosition = -300.*mm;
   G4ParticleTable *parTable = G4ParticleTable::GetParticleTable();

   G4ParticleDefinition *proton = parTable->FindParticle("proton");
   fProtonGun->SetParticleDefinition(proton);
   fProtonGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fProtonGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
   fProtonGun->SetParticleEnergy(fEnergy);

   TFile *file = new TFile("randomSource.root", "OPEN");
   fHisSource = (TH2D*)file->Get("fHisMap");
   fHisSource->SetName("fHisSource");

   DefineCommands();
}

BIPrimaryGeneratorAction::~BIPrimaryGeneratorAction()
{
   delete fProtonGun;
}

void BIPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   //G4double coneTheta = 15.*deg;
   //G4ThreeVector particleVec = GetParVec(coneTheta);
   //fEnergy = fEneFnc->GetRandom() * MeV;

   G4ThreeVector particleVec = GetParVecEne();
   fProtonGun->SetParticleMomentumDirection(particleVec);

   fProtonGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fProtonGun->SetParticleEnergy(fEnergy);
   fProtonGun->GeneratePrimaryVertex(event);

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->FillNtupleIColumn(1, 0, 11);
   anaMan->FillNtupleDColumn(1, 1, fEnergy);
   anaMan->FillNtupleDColumn(1, 2, particleVec.x());
   anaMan->FillNtupleDColumn(1, 3, particleVec.y());
   anaMan->FillNtupleDColumn(1, 4, particleVec.z());
   anaMan->AddNtupleRow(1);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;

}

G4ThreeVector BIPrimaryGeneratorAction::GetParVec(G4double limit)
{
   G4double theta = acos(1. - G4UniformRand() * (1. - cos(limit)));
   G4double phi = G4UniformRand() * 2. * CLHEP::pi;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   G4ThreeVector particleVec = G4ThreeVector(vx, vy, vz);

   return particleVec;
}

G4ThreeVector BIPrimaryGeneratorAction::GetParVecEne()
{
   G4double x, y;
   while(1){// HisSource should be changed start with 0
      fHisSource->GetRandom2(x, y);
      if(x >= 78) break;
   }

   fEnergy = pow(10., (y - 152.) / fDy) * 20;

   G4double theta = CLHEP::pi * ((x - 78.) / fDx) / 180.;
   G4double phi = G4UniformRand() * 2. * CLHEP::pi;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   G4ThreeVector particleVec = G4ThreeVector(vx, vy, vz);

   return particleVec;
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
