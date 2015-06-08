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

   // window
   G4double windowL = 163.6*mm;
   G4double windowW = 118.4*mm;
   G4double windowH = 0.5*mm;

   G4Box *windowS = new G4Box("Window", windowL / 2., windowW / 2., windowH / 2.);
   G4LogicalVolume *windowLV = new G4LogicalVolume(windowS, fWindowMat, "Window");
   new G4PVPlacement(0, G4ThreeVector(), windowLV, "Window", fWorldLV,
                     false, 0, checkOverlaps);

   // air gap
   //G4double airGapL = 163.6*mm;
   //G4double airGapW = 118.4*mm;
   //G4double airGapH = 0.5*mm;
   G4double airGapL = windowL;
   G4double airGapW = windowW;
   G4double airGapH = 10.*mm;

   G4ThreeVector airGapPos = G4ThreeVector(0., 0., windowH / 2. + airGapH / 2.);
   
   G4Box *airGapS = new G4Box("AirGap", airGapL / 2., airGapW / 2., airGapH / 2.);
   G4LogicalVolume *airGapLV = new G4LogicalVolume(airGapS, fAir, "AirGap");
   new G4PVPlacement(0, airGapPos, airGapLV, "AirGap", fWorldLV,
                     false, 0, checkOverlaps);
   
   // plate
   //G4double plateL = 163.6*mm;
   //G4double plateW = 118.4*mm;
   //G4double plateH = 0.5*mm;
   G4double plateL = windowL;
   G4double plateW = windowW;
   G4double plateH = 2.*mm;

   G4ThreeVector platePos = G4ThreeVector(0., 0., windowH / 2. + airGapH + plateH / 2.);
   
   G4Box *plateS = new G4Box("Plate", plateL / 2., plateW / 2., plateH / 2.);
   G4LogicalVolume *plateLV = new G4LogicalVolume(plateS, fPlateMat, "Plate");
   new G4PVPlacement(0, platePos, plateLV, "Plate", fWorldLV,
                     false, 0, checkOverlaps);


   // cell
   //G4double cellL = 163.6*mm;
   //G4double cellW = 118.4*mm;
   //G4double cellH = 0.5*mm;
   G4double cellL = windowL;
   G4double cellW = windowW;
   G4double cellH = 0.1*mm;

   G4ThreeVector cellPos = G4ThreeVector(0., 0., windowH / 2. + airGapH +
                                         plateH + cellH / 2.);
   
   G4Box *cellS = new G4Box("Cell", cellL / 2., cellW / 2., cellH / 2.);
   G4LogicalVolume *cellLV = new G4LogicalVolume(cellS, fCellMat, "Cell");
   new G4PVPlacement(0, cellPos, cellLV, "Cell", fWorldLV,
                     false, 0, checkOverlaps);


   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   fWorldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Red());
   windowLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   airGapLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Magenta());
   plateLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Blue());
   cellLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return worldPV;
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

