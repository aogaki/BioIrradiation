#include <fstream>
#include <math.h>
#include <time.h>

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

BIPrimaryGeneratorAction::BIPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fProtonGun(0),
     fEneFnc(nullptr)
{
   G4AutoLock lock(&mutexInPGA);

   Int_t seed = time(NULL) + G4Threading::G4GetThreadId() * 10000;
   gRandom->SetSeed(seed);
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
}

BIPrimaryGeneratorAction::~BIPrimaryGeneratorAction()
{
   delete fProtonGun;
}

void BIPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   G4double coneTheta = 15.*deg;
   G4ThreeVector particleVec = GetParVec(coneTheta);
   fProtonGun->SetParticleMomentumDirection(particleVec);

   fProtonGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));

   fProtonGun->SetParticleEnergy(fEnergy);
   fProtonGun->GeneratePrimaryVertex(event);
/*
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->FillNtupleIColumn(1, 0, 11);
   anaMan->FillNtupleDColumn(1, 1, ene);
   anaMan->FillNtupleDColumn(1, 2, particleVec.x());
   anaMan->FillNtupleDColumn(1, 3, particleVec.y());
   anaMan->FillNtupleDColumn(1, 4, particleVec.z());
   anaMan->AddNtupleRow(1);
*/
   G4AutoLock lock(&mutexInPGA);
   fEnergy = fEneFnc->GetRandom() * MeV;
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
