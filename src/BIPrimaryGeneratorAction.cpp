#include <math.h>

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
     fProtonGun(0)
{
   G4AutoLock lock(&mutexInPGA);

   G4int nPar = 1;
   fProtonGun = new G4ParticleGun(nPar);

   fZPosition = -20.*mm;
   G4ParticleTable *parTable = G4ParticleTable::GetParticleTable();

   G4ParticleDefinition *proton = parTable->FindParticle("proton");
   fProtonGun->SetParticleDefinition(proton);
   fProtonGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fProtonGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
   fProtonGun->SetParticleEnergy(50.*MeV);
}

BIPrimaryGeneratorAction::~BIPrimaryGeneratorAction()
{
   delete fProtonGun;
}

void BIPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   
   G4ThreeVector particleVec = GetParVec();
   particleVec = G4ThreeVector(0, 0, 1);
   fProtonGun->SetParticleMomentumDirection(particleVec);

   fProtonGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));

   G4double ene = 20.*MeV;
   fProtonGun->SetParticleEnergy(ene);
   fProtonGun->GeneratePrimaryVertex(event);

   anaMan->FillNtupleIColumn(1, 0, 11);
   anaMan->FillNtupleDColumn(1, 1, ene);
   anaMan->FillNtupleDColumn(1, 2, particleVec.x());
   anaMan->FillNtupleDColumn(1, 3, particleVec.y());
   anaMan->FillNtupleDColumn(1, 4, particleVec.z());
   anaMan->AddNtupleRow(1);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;

}

G4ThreeVector BIPrimaryGeneratorAction::GetParVec()
{
   G4double angleLimit = CLHEP::pi;
   G4double theta = acos(1. - G4UniformRand() * (1. - cos(angleLimit)));
   G4double phi = G4UniformRand() * 2. * CLHEP::pi;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   G4ThreeVector particleVec = G4ThreeVector(vx, vy, vz);

   return particleVec;
}
