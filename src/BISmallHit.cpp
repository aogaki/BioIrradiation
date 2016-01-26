#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "BISmallHit.hpp"


G4ThreadLocal G4Allocator<BISmallHit> *BISmallHitAllocator = 0;


BISmallHit::BISmallHit()
   : G4VHit(),
     fDepositEnergy(0),
     fPosition(0)
{}

BISmallHit::~BISmallHit()
{}

BISmallHit::BISmallHit(const BISmallHit & /*right*/)
   : G4VHit()
{}

const BISmallHit &
BISmallHit::operator=(const BISmallHit & /*right*/)
{
   return *this;
}

int BISmallHit::operator==(const BISmallHit & /*right*/) const
{
   return 0;
}
