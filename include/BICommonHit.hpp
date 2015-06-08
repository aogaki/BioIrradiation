#ifndef BICommonHit_h
#define BICommonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class BICommonHit : public G4VHit
{
public:
   BICommonHit();
   virtual ~BICommonHit();
   BICommonHit(const BICommonHit &right);
   const BICommonHit &operator=(const BICommonHit &right);
   int operator==(const BICommonHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetPDGCode(G4int code) {fPDGCode = code;};
   G4int GetPDGCode() {return fPDGCode;};

   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};

   void SetTime(G4double time) {fTime = time;};
   G4double GetTime() {return fTime;};
   
   void SetVolumeName(G4String name) {fVolumeName = name;};
   G4String GetVolumeName() {return fVolumeName;};
   
   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};
   
   void SetMomentum(G4ThreeVector p) {fMomentum = p;};
   G4ThreeVector GetMomentum() {return fMomentum;};
   
   void SetIsLast(G4int flag) {fIsLastStep = flag;};
   G4int GetIsLast() {return fIsLastStep;};

private:
   G4int fPDGCode;
   G4double fDepositEnergy;
   G4double fTime;
   G4String fVolumeName;
   G4ThreeVector fPosition;
   G4ThreeVector fMomentum;
   G4int fIsLastStep;
      
};

typedef G4THitsCollection<BICommonHit> BICommonHitsCollection;

extern G4ThreadLocal G4Allocator<BICommonHit> *BICommonHitAllocator;

inline void *BICommonHit::operator new(size_t)
{
   if (!BICommonHitAllocator)
      BICommonHitAllocator = new G4Allocator<BICommonHit>;
   return (void *)BICommonHitAllocator->MallocSingle();
}

inline void BICommonHit::operator delete(void *hit)
{
   BICommonHitAllocator->FreeSingle((BICommonHit *) hit);
}

#endif
