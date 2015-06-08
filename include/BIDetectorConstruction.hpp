#ifndef BIDetectorConstruction_h
#define BIDetectorConstruction_h 1

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"


class BIDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   BIDetectorConstruction();
   virtual ~BIDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

private:
   G4LogicalVolume *fWorldLV;

   G4LogicalVolume *ConstructPlate();
   
   void DefineMaterial();
   G4Material *fVacuum;
   G4Material *fAir;
   G4Material *fWindowMat;
   G4Material *fCassetteMat;
   G4Material *fPlateMat;
   G4Material *fHolderMat;
   G4Material *fSealingMat;
   G4Material *fCellMat;
   
   std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
