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
#include "G4Cons.hh"
#include "G4CutTubs.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UImanager.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

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
     fCellMat(0),
     fFilmMat(0),
     fStuffMat(0)
{
   fCheckOverlap = true;
   
   DefineMaterial();

   // Geometry parameters
   // Should be read from geometry file?
   // It's user friendly?
   fWellPitch = 9.*mm;
   fWellOpening = 6.96*mm; // inner size
   fWellBottom = 6.39*mm; // inner size
   fWellH = 10.9*mm; // inner size

   fCellT = 150.*nm;
      
   fPlateT = 1.*mm; // temporary
   fPlateL = 125.*mm; // temporary
   fPlateW = 82.4*mm; // temporary
   //fPlateH = 14.6*mm; // temporary
   fPlateH = fWellH + fPlateT; // temporary

   const G4double mil = 25.3995*um;
   fFilmT = 4.4*mil; // temporary
   fFilmL = 125.*mm; // temporary
   fFilmW = 82.4*mm; // temporary

   fCassetteL = 163.6*mm; // temporary
   fCassetteW = 118.4*mm; // temporary
   fCassetteH = 22.5*mm; // temporary
   fCassetteBottomT = 3.*mm; // temporary

   fSealingL = fCassetteL;
   fSealingW = fCassetteW;
   fSealingT = 5.3*mm;

   fWindowL = fCassetteL;
   fWindowW = fCassetteW;
   fWindowT = 0.5*mm;

   fHolderL = fCassetteL;
   fHolderW = fCassetteW;
   fHolderT = 30.*mm - fCassetteH - fSealingT - fWindowT;

   fOpeningL = 122.*mm;
   fOpeningW = 80.*mm;
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
   fFilmMat =  manager->FindOrBuildMaterial("G4_POLYETHYLENE");
   fHolderMat = manager->FindOrBuildMaterial("G4_Al");
   fSealingMat = manager->FindOrBuildMaterial("G4_Al");
   fStuffMat = manager->FindOrBuildMaterial("G4_WATER");
   fCellMat = manager->BuildMaterialWithNewDensity("G4_WATER_MODIFIED","G4_WATER",
                                                   1.1*g/cm/cm/cm);
}

G4VPhysicalVolume *BIDetectorConstruction::Construct()
{
   // world volume
   G4double worldDx = 2.*m;
   G4double worldDy = 2.*m;
   G4double worldDz = 2.*m;

   G4Box *worldS = new G4Box("World", worldDx / 2., worldDy / 2., worldDz / 2.);
   fWorldLV = new G4LogicalVolume(worldS, fVacuum, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   fWorldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLV, "World", 0,
                          false, 0, fCheckOverlap);

/*
   G4LogicalVolume *plateLV = ConstructPlate();
   G4ThreeVector platePos = G4ThreeVector();
   new G4PVPlacement(nullptr, platePos, plateLV, "Plate", fWorldLV,
                     false, 0, fCheckOverlap);
*/
   
   G4LogicalVolume *sealingLV = ConstructSealing();
   G4double sealingZ = (-fWindowT - fSealingT) / 2.;
   G4ThreeVector sealingPos = G4ThreeVector(0., 0., sealingZ);
   new G4PVPlacement(nullptr, sealingPos, sealingLV, "Sealing", fWorldLV,
                     false, 0, fCheckOverlap);
   
   G4LogicalVolume *windowLV = ConstructWindow();
   G4double windowZ = 0.*mm;
   G4ThreeVector windowPos = G4ThreeVector(0., 0., windowZ);
   new G4PVPlacement(nullptr, windowPos, windowLV, "Window", fWorldLV,
                     false, 0, fCheckOverlap);

   G4double airL = fCassetteL;
   G4double airW = fCassetteW;
   G4double airH = fCassetteH + fHolderT;
   G4Box *airS = new G4Box("Air", airL / 2., airW / 2., airH / 2.);
   G4LogicalVolume *airLV = new G4LogicalVolume(airS, fAir, "Air");
   visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   airLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   G4double airZ = (fWindowT + airH) / 2.;
   G4ThreeVector airPos = G4ThreeVector(0., 0., airZ);
   new G4PVPlacement(nullptr, airPos, airLV, "Air", fWorldLV,
                     false, 0, fCheckOverlap);
   
   
   G4LogicalVolume *holderLV = ConstructHolder();
   G4double holderZ = -fCassetteH / 2.;
   G4ThreeVector holderPos = G4ThreeVector(0., 0., holderZ);
   new G4PVPlacement(nullptr, holderPos, holderLV, "Holder", airLV,
                     false, 0, fCheckOverlap);

   G4LogicalVolume *plateLV = ConstructPlate();
   G4double plateZ = -airH / 2. + 5.*mm + (fWellH + fPlateT) / 2.;
   G4ThreeVector platePos = G4ThreeVector(0., 0., plateZ);
   new G4PVPlacement(nullptr, platePos, plateLV, "Plate", airLV,
                     false, 0, fCheckOverlap);

   G4LogicalVolume *filmLV = ConstructFilm();
   G4double filmZ = plateZ + fPlateH / 2. + fFilmT / 2.;
   G4ThreeVector filmPos = G4ThreeVector(0., 0., filmZ);
   new G4PVPlacement(nullptr, filmPos, filmLV, "Film", airLV,
                     false, 0, fCheckOverlap);

   G4LogicalVolume *cassetteLV = ConstructCassette();
   G4double cassetteZ = fHolderT / 2.;
   G4ThreeVector cassettePos = G4ThreeVector(0., 0., cassetteZ);
   new G4PVPlacement(nullptr, cassettePos, cassetteLV, "Cassette", airLV,
                     false, 0, fCheckOverlap);

   return worldPV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructWell()
{
   G4Box *top = new G4Box("Top", fWellPitch / 2., fWellPitch / 2., fPlateT / 2.);
   G4double coneSize = fPlateT + fWellBottom - ((fWellOpening - fWellBottom) / fWellH) * fPlateT;
   G4Cons *cone = new G4Cons("Cone", 0., fPlateT + fWellOpening / 2., 0., coneSize / 2., 
                             (fPlateT + fWellH) / 2., 0., 2 * CLHEP::pi);

   G4ThreeVector conePos(0., 0., fWellH / 2.);
   G4UnionSolid *unionS = new G4UnionSolid("Well", top, cone, nullptr, conePos); 


   G4Cons *openingS = new G4Cons("Stuff", 0., fWellOpening / 2., 0., fWellBottom / 2.,
                                 fWellH / 2., 0., 2 * CLHEP::pi);
   G4ThreeVector openingPos(0., 0., fWellH / 2. - fPlateT / 2.);
   G4SubtractionSolid *wellS = new G4SubtractionSolid("Well", unionS, openingS, nullptr, openingPos);
   
   G4LogicalVolume *wellLV = new G4LogicalVolume(wellS, fPlateMat, "Well");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   wellLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   return wellLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructStuff()
{
   G4Cons *stuffS = new G4Cons("Stuff", 0., fWellOpening / 2., 0., fWellBottom / 2.,
                                 fWellH / 2., 0., 2 * CLHEP::pi);
   G4LogicalVolume *stuffLV = new G4LogicalVolume(stuffS, fStuffMat, "Stuff");   

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Blue());
   visAttributes->SetVisibility(true);
   stuffLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4Tubs *cellS = new G4Tubs("Cell", 0., fWellBottom / 2.,
                              fCellT / 2., 0., 2 * CLHEP::pi);
   G4LogicalVolume *cellLV = new G4LogicalVolume(cellS, fCellMat, "Cell");

   visAttributes = new G4VisAttributes(G4Colour::Magenta());
   visAttributes->SetVisibility(true);
   cellLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   G4ThreeVector cellPos(0., 0., fWellH / 2. - fCellT / 2.);
   new G4PVPlacement(0, cellPos, cellLV, "Cell", stuffLV,
                     false, 0, fCheckOverlap);
   
   return stuffLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructPlate()
{ // Think when inside of wells are filled
   G4Box *plateS = new G4Box("Plate", fPlateL / 2., fPlateW / 2., fPlateH / 2.);
   G4LogicalVolume *plateLV = new G4LogicalVolume(plateS, fAir, "Plate");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   plateLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4Box *boxS = new G4Box("Outer", fPlateL / 2., fPlateW / 2., fPlateH / 2.);
   G4Box *airS = new G4Box("Outer", -fPlateT + fPlateL / 2., -fPlateT + fPlateW / 2.,
                           (fPlateH - fPlateT) / 2.);
   G4Box *windowS = new G4Box("Outer", (fWellPitch * 12) / 2., (fWellPitch * 8) / 2.,
                              fPlateT);

   G4ThreeVector airPos(0., 0., fPlateT / 2.); 
   G4SubtractionSolid *subS = new G4SubtractionSolid("Outer", boxS, airS, nullptr, airPos);
   G4ThreeVector windowPos(0., 0., (fPlateT - fPlateH) / 2.); 
   G4SubtractionSolid *outerS = new G4SubtractionSolid("Outer", subS, windowS, nullptr, windowPos);
   G4LogicalVolume *outerLV = new G4LogicalVolume(outerS, fPlateMat, "Outer");

   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   outerLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4RotationMatrix *plateRot = new G4RotationMatrix();
   plateRot->rotateX(CLHEP::pi);
   new G4PVPlacement(plateRot, G4ThreeVector(), outerLV, "Outer", plateLV,
                     false, 0, fCheckOverlap);
   
   // adding wells
   G4LogicalVolume *wellLV = ConstructWell();
   G4LogicalVolume *stuffLV = ConstructStuff();

   for(G4int iRow = 0; iRow < 8; iRow++){
      for(G4int iColumn = 0; iColumn < 12; iColumn++){
         G4double xStart = -(fWellPitch * 11) / 2.;
         G4double yStart = -(fWellPitch * 7) / 2.;
         G4double xPos = xStart + iColumn * fWellPitch;
         G4double yPos = yStart + iRow * fWellPitch;
         G4double zPos = fPlateT / 2. - fPlateH / 2.;
         G4ThreeVector wellPos = G4ThreeVector(xPos, yPos, -zPos);
         G4ThreeVector stuffPos = G4ThreeVector(xPos, yPos, -zPos - fWellH / 2. + fPlateT / 2.);
         
         new G4PVPlacement(plateRot, wellPos, wellLV, "Well", plateLV,
                           false, iRow * 12 + iColumn, fCheckOverlap);
         new G4PVPlacement(plateRot, stuffPos, stuffLV, "Stuff", plateLV,
                           false, iRow * 12 + iColumn, fCheckOverlap);
      }
   }

   return plateLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructFilm()
{
   G4Box *filmS = new G4Box("Film", fFilmL / 2., fFilmW / 2., fFilmT / 2.);
   G4LogicalVolume *filmLV = new G4LogicalVolume(filmS, fFilmMat, "Film");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   filmLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return filmLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructCassette()
{
   G4Box *cassetteS = new G4Box("Cassette", fCassetteL / 2., fCassetteW / 2., fCassetteH / 2.);
   G4Box *plateS = new G4Box("Plate", fPlateL / 2., fPlateW / 2., fCassetteH);
   G4ThreeVector platePos(0., 0., 0.);
   G4SubtractionSolid *subS = new G4SubtractionSolid("Cassette", cassetteS, plateS, nullptr, platePos);
   G4Box *bottomS = new G4Box("Cassette", fCassetteL / 2., fCassetteW / 2., fCassetteBottomT / 2.);
   G4ThreeVector bottomPos(0., 0., (fCassetteH - fCassetteBottomT) / 2.);
   G4UnionSolid *unionS = new G4UnionSolid("Cassette", subS, bottomS, nullptr, bottomPos);
   
   G4LogicalVolume *cassetteLV = new G4LogicalVolume(unionS, fCassetteMat, "Cassette");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Yellow());
   visAttributes->SetVisibility(true);
   cassetteLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return cassetteLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructHolder()
{
   G4Box *boardS = new G4Box("Holder", fHolderL / 2., fHolderW / 2., fHolderT / 2.);
   G4Box *openingS = new G4Box("Opening", fOpeningL / 2., fOpeningW / 2., fHolderT);
   G4SubtractionSolid *holderS = new G4SubtractionSolid("Holder", boardS, openingS);
   G4LogicalVolume *holderLV = new G4LogicalVolume(holderS, fHolderMat, "Holder");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Green());
   visAttributes->SetVisibility(true);
   holderLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return holderLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructWindow()
{
   G4Box *windowS = new G4Box("Window", fWindowL / 2., fWindowW / 2., fWindowT / 2.);
   G4LogicalVolume *windowLV = new G4LogicalVolume(windowS, fWindowMat, "Window");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   windowLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return windowLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructSealing()
{
   G4Box *boardS = new G4Box("Sealing", fSealingL / 2., fSealingW / 2., fSealingT / 2.);
   G4Box *openingS = new G4Box("Opening", fOpeningL / 2., fOpeningW / 2., fSealingT);
   G4SubtractionSolid *sealingS = new G4SubtractionSolid("Sealing", boardS, openingS);
   G4LogicalVolume *sealingLV = new G4LogicalVolume(sealingS, fSealingMat, "Sealing");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   sealingLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return sealingLV;
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

