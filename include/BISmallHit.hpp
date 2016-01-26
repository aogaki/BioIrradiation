#ifndef BISmallHit_h
#define BISmallHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class BISmallHit : public G4VHit
{
public:
   BISmallHit();
   virtual ~BISmallHit();
   BISmallHit(const BISmallHit &right);
   const BISmallHit &operator=(const BISmallHit &right);
   int operator==(const BISmallHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};

   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};

private:
   G4double fDepositEnergy;
   G4ThreeVector fPosition;

};

typedef G4THitsCollection<BISmallHit> BISmallHitsCollection;

extern G4ThreadLocal G4Allocator<BISmallHit> *BISmallHitAllocator;

inline void *BISmallHit::operator new(size_t)
{
   if (!BISmallHitAllocator)
      BISmallHitAllocator = new G4Allocator<BISmallHit>;
   return (void *)BISmallHitAllocator->MallocSingle();
}

inline void BISmallHit::operator delete(void *hit)
{
   BISmallHitAllocator->FreeSingle((BISmallHit *) hit);
}

#endif
