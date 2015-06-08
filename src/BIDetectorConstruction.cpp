#include <string>

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4CutTubs.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UImanager.hh"
#include "G4UnionSolid.hh"

#include "BIDetectorConstruction.hpp"
#include "BICommonSD.hpp"


BIDetectorConstruction::BIDetectorConstruction()
   : G4VUserDetectorConstruction(),
     fWorldLV(0),
     fVacuum(0),
     fAir(0),
     fWindowMat(0),
     fCassetteMat(0),
     fPlateMat(0),
     fHolderMat(0),
     fSealingMat(0),
     fCellMat(0)
{
   DefineMaterial();
}

BIDetectorConstruction::~BIDetectorConstruction()
{}

void BIDetectorConstruction::DefineMaterial()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuum = manager->FindOrBuildMaterial("G4_Galactic");
   fAir = manager->FindOrBuildMaterial("G4_AIR");
   fWindowMat = manager->FindOrBuildMaterial("G4_Al");
   fCassetteMat = manager->FindOrBuildMaterial("G4_Al"); // Just testing
   fPlateMat = manager->FindOrBuildMaterial("G4_POLYSTYRENE");
   fHolderMat = manager->FindOrBuildMaterial("G4_Al");
   fSealingMat = manager->FindOrBuildMaterial("G4_Al");
   fCellMat = manager->FindOrBuildMaterial("G4_WATER");
}

G4VPhysicalVolume *BIDetectorConstruction::Construct()
{
   G4bool checkOverlaps = true;

   // world volume
   G4double worldDx = 1.*m;
   G4double worldDy = 1.*m;
   G4double worldDz = 1.*m;

   G4Box *worldS = new G4Box("World", worldDx / 2., worldDy / 2., worldDz / 2.);
   fWorldLV = new G4LogicalVolume(worldS, fVacuum, "World");
   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(0, G4ThreeVector(), fWorldLV, "World", 0,
                          false, 0, checkOverlaps);

   // plate
   G4LogicalVolume *plateLV = ConstructPlate();
   new G4PVPlacement(0, G4ThreeVector(), plateLV, "Plate", fWorldLV,
                     false, 0, checkOverlaps);
   
// visualization options
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   fWorldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   plateLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return worldPV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructPlate()
{
   G4LogicalVolume *plateLV;
   G4double plateT = 1.*mm;
   G4double plateL = 127.76*mm;
   G4double plateW = 85.48*mm;
   G4double plateH = 14.6*mm;

   G4Box *topS = new G4Box("Top", plateL / 2., plateW / 2., plateT / 2.);

   G4double wellL = 10.9*mm;
   G4double wellD1 = 6.96*mm; // inner diameter of well opening
   G4double wellD2 = 6.39*mm; // inner diameter of well bottom

   G4Tubs *wellS = new G4Tubs("Well", 0, plateT + wellD1 / 2.,
                              (plateT + wellL) / 2., 0, CLHEP::twopi);

   //G4UnionSolid *plateS = new G4UnionSolid("Plate", nullptr, topS);
   //G4UnionSolid *plateS = new G4UnionSolid("Plate", topS, wellS, wellRot, wellPos);
   G4UnionSolid *plateS = (G4UnionSolid*)topS;

   // adding wells
   for(G4int iRow = 0; iRow < 8; iRow++){
      for(G4int iColumn = 0; iColumn < 12; iColumn++){
         G4cout << iRow <<"\t"<< iColumn << G4endl;
         G4RotationMatrix *wellRot = new G4RotationMatrix(); // don't need?

         G4double offsetRow = 11.24*mm;
         G4double offsetColumn = 14.38*mm;
         G4double wellPitch = 9.*mm;
         G4double xStart = offsetColumn - (plateL / 2.);
         G4double yStart = offsetRow - (plateW / 2.);
         G4double xPos = xStart + iColumn * wellPitch;
         G4double yPos = yStart + iRow * wellPitch;
         G4double zPos = plateT / 2. + (plateT + wellL) / 2.;
         G4ThreeVector wellPos = G4ThreeVector(xPos, yPos, zPos);
         
         plateS = new G4UnionSolid("Plate", plateS, wellS, wellRot, wellPos);
      }
   }
   
   plateLV = new G4LogicalVolume(plateS, fPlateMat, "Plate");
   
   return plateLV;
}

void BIDetectorConstruction::ConstructSDandField()
{
   G4VSensitiveDetector *CommonSD = new BICommonSD("Common",
                                                   "CommonHitsCollection");

   // Set SD for all LV.  Using G4LogicalVolume::SetSensitiveDetector() ?
   // Which is better?
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   std::vector<G4LogicalVolume*>::const_iterator it;
   for(it = lvStore->begin(); it != lvStore->end(); it++){
      SetSensitiveDetector((*it)->GetName(), CommonSD);
   }

   fWorldLV->SetSensitiveDetector(nullptr);
}

