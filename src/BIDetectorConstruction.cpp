#include <stdio.h>
#include <string>
#include <fstream>

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
#include "G4PhysicalVolumeStore.hh"
#include "G4UImanager.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SDManager.hh"

#include "BIDetectorConstruction.hpp"
#include "BICommonSD.hpp"
#include "BISmallSD.hpp"

#ifdef NOTCPP11
#include <sstream>
G4double stol(std::string buf)
{
   std::istringstream is;
   is.str(buf);
   G4double retVal;
   is >> retVal;
   return retVal;
}

G4String itos(const G4int val)
{
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

#endif

BIDetectorConstruction::BIDetectorConstruction(G4bool forGrid, G4bool useTile, G4bool cutGeo)
   : G4VUserDetectorConstruction(),
     fWorldLV(nullptr),
     fMessenger(nullptr),
     fWindowPV(nullptr),
     fFoilPV(nullptr),
     fAirPV(nullptr),
     fSealingPV(nullptr),
     fHolderPV(nullptr),
     fPlatePV(nullptr),
     fFilmPV(nullptr),
     fCassettePV(nullptr),
     fVacuum(nullptr),
     fAir(nullptr),
     fWindowMat(nullptr),
     fFoilMat(nullptr),
     fCassetteMat(nullptr),
     fPlateMat(nullptr),
     fHolderMat(nullptr),
     fSealingMat(nullptr),
     fCellMat(nullptr),
     fAttMat(nullptr),
     fFilmMat(nullptr),
     fStuffMat(nullptr)
{
   fAttFile = "tile.att";
   fCutGeo = cutGeo;
   fForGrid = forGrid;
   fUseTileAtt = useTile;
   fCheckOverlap = true;
   kAttPitch = fAttPitch * um;

   if(!fUseTileAtt){
      fAttFile = "circle.att";
      ReadAttData();
   }
   for(G4int i = 0; i < kAtt; i++) fAttPV[i] = nullptr;
   for(G4int i = 0; i < 96; i++) fTileAttPV[i] = nullptr;

   DefineMaterial();
   DefineGeoPar();
   DefineCommands();
}

BIDetectorConstruction::~BIDetectorConstruction()
{
   // delete PV pointers?
}

void BIDetectorConstruction::DefineGeoPar()
{
   // Geometry parameters
   // Should be read from geometry file?
   // It's user friendly?
   fWellPitch = 9.*mm;
   fWellOpening = 6.96*mm; // inner size
   fWellBottom = 6.39*mm; // inner size
   fWellH = 10.9*mm; // inner size

   //fCellT = 150.*nm;
   //fCellT = 20.*um;
   fCellT = 10.*um;
      
   fPlateT = 1.*mm; // temporary
   //fPlateT = 0.3*mm; // temporary
   fPlateL = 125.*mm; // temporary
   fPlateW = 82.4*mm; // temporary
   //fPlateH = 14.6*mm; // temporary
   fPlateH = fWellH + fPlateT; // temporary
   fAirGap = 5.*mm;
   
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
   fHolderT = 1.7*mm;

   fAirL = fCassetteL;
   fAirW = fCassetteW;
   fAirT = fCassetteH + fHolderT;
   
   fBumpT = 2.2*mm;
   fBumpW = 2.*mm;
   
   fOpeningL = 122.*mm;
   fOpeningW = 80.*mm;

   fFoilL = fOpeningL;
   fFoilW = fOpeningW;
   fFoilT = 0.5*mm;

   fTileAttL = fWellPitch;

   if(fUseTileAtt){
      for(G4int y = 0; y < 12; y++){ // init with 0. no attenuator
         for(G4int x = 0; x < 8; x++){
            fTileAttT[y][x] = 0;
         }
      }
      std::ifstream attT(fAttFile);
      if(attT.is_open()){
         std::string buf;
         for(G4int y = 0; y < 12; y++){
            for(G4int x = 0; x < 8; x++){
               attT >> buf;
               G4cout << atof(buf.c_str()) << G4endl;
               fTileAttT[y][x] = atof(buf.c_str());
            }
         }
         attT.close();
      }
   }

}

void BIDetectorConstruction::DefineMaterial()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuum = manager->FindOrBuildMaterial("G4_Galactic");
   fAir = manager->FindOrBuildMaterial("G4_AIR");
   fWindowMat = manager->FindOrBuildMaterial("G4_Ni");
   fAttMat = fWindowMat;
   fFoilMat = manager->FindOrBuildMaterial("G4_Au");
   fCassetteMat = manager->FindOrBuildMaterial("G4_Al"); // Just testing
   fPlateMat = manager->FindOrBuildMaterial("G4_POLYSTYRENE");
   fFilmMat =  manager->FindOrBuildMaterial("G4_POLYETHYLENE");
   fHolderMat = manager->FindOrBuildMaterial("G4_Al");
   fSealingMat = manager->FindOrBuildMaterial("G4_Al");
   fStuffMat = manager->FindOrBuildMaterial("G4_WATER");
   fCellMat = manager->BuildMaterialWithNewDensity("G4_WATER_MODIFIED","G4_WATER",
                                                   1.1*g/cm/cm/cm);

   G4Material *Si3N4 = new G4Material("SiliconNitride", 3.44*g/cm3, 2, kStateSolid);
   G4Element *Si = manager->FindOrBuildElement("Si");
   G4Element *N = manager->FindOrBuildElement("N");
   Si3N4->AddElement(Si, 3);
   Si3N4->AddElement(N, 4);
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
   G4LogicalVolume *holderLV = ConstructStuff();
   G4ThreeVector holderPos = G4ThreeVector();
   new G4PVPlacement(nullptr, holderPos, holderLV, "Holder", fWorldLV,
                     false, 0, fCheckOverlap);
*/

   G4LogicalVolume *sealingLV = ConstructSealing();
   G4double sealingZ = (-fWindowT - fSealingT) / 2.;
   G4ThreeVector sealingPos = G4ThreeVector(0., 0., sealingZ);
   fSealingPV = new G4PVPlacement(nullptr, sealingPos, sealingLV, "Sealing", fWorldLV,
                     false, 0, fCheckOverlap);
   
   G4LogicalVolume *windowLV = ConstructWindow();
   G4double windowZ = 0.*mm;
   G4ThreeVector windowPos = G4ThreeVector(0., 0., windowZ);
   fWindowPV = new G4PVPlacement(nullptr, windowPos, windowLV, "Window", fWorldLV,
                     false, 0, fCheckOverlap);

   G4Box *airS = new G4Box("Air", fAirL / 2., fAirW / 2., fAirT / 2.);
   G4LogicalVolume *airLV = new G4LogicalVolume(airS, fAir, "Air");
   visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   airLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   G4ThreeVector airPos = G4ThreeVector(0., 0., (fAirT + fWindowT) / 2.);
   fAirPV = new G4PVPlacement(nullptr, airPos, airLV, "Air", fWorldLV,
                              false, 0, fCheckOverlap);
   
   //G4LogicalVolume *holderLV = ConstructHolder();
   G4LogicalVolume *holderLV = ConstructSimpleHolder();
   G4double holderZ = -fCassetteH / 2.;
   G4ThreeVector holderPos = G4ThreeVector(0., 0., holderZ);
   fHolderPV = new G4PVPlacement(nullptr, holderPos, holderLV, "Holder", airLV,
                                 false, 0, fCheckOverlap);
/*
   G4LogicalVolume *foilLV = ConstructFoil();
   G4double foilZ = (fFoilT - fAirT) / 2.;
   G4ThreeVector foilPos = G4ThreeVector(0., 0., foilZ);
   fFoilPV = new G4PVPlacement(nullptr, foilPos, foilLV, "Foil", airLV,
                     false, 0, fCheckOverlap);
*/
   G4LogicalVolume *plateLV = ConstructPlate();
   G4double plateZ = -fAirT / 2. + fAirGap + (fWellH + fPlateT) / 2.;
   G4ThreeVector platePos = G4ThreeVector(0., 0., plateZ);
   fPlatePV = new G4PVPlacement(nullptr, platePos, plateLV, "Plate", airLV,
                                false, 0, fCheckOverlap);

   if(fUseTileAtt){
      G4String rowMap[8] = {"H", "G", "F", "E", "D", "C", "B", "A"};
      for(G4int iColumn = 0; iColumn < 12; iColumn++){
         for(G4int iRow = 0; iRow < 8; iRow++){
            if(fTileAttT[iColumn][iRow] != 0.){
#ifdef NOTCPP11
               G4String name = "Att" + rowMap[iRow] + itos(iColumn + 1);
#else
               G4String name = "Att" + rowMap[iRow] + std::to_string(iColumn + 1);
#endif
               G4cout << name <<"\t"<< fTileAttT[iColumn][iRow]  << G4endl;
               G4double t = fTileAttT[iColumn][iRow]*um;
               G4LogicalVolume *attLV = ConstructTileAtt(name, t);
               G4double xStart = -(fWellPitch * 11) / 2.;
               G4double yStart = -(fWellPitch * 7) / 2.;
               G4double xPos = xStart + iColumn * fWellPitch;
               G4double yPos = yStart + iRow * fWellPitch;
               //G4double zPos = -fAirT / 2. + fAirGap - t / 2;
               //G4double zPos = -fAirT / 2. + t / 2;
               G4double zPos = -fWindowT / 2. - t / 2;
               G4ThreeVector attPos = G4ThreeVector(xPos, yPos, zPos);
               fTileAttPV[iColumn * 8 + iRow] =
                  new G4PVPlacement(nullptr, attPos, attLV, name, fWorldLV,
                                    false, 0, fCheckOverlap);
            }
         }
      }
   }
   else {
      G4LogicalVolume *attLV[kAtt];
      G4double attR = fAttArea*um;
      //G4double attOffset = -fAirT / 2.;
      G4double attOffset = -fWindowT / 2.;
      G4Color col[1000]; // Shoud be larger than
      G4int colIt = 0;
      G4double colPitch = 0.1;
      for(G4double rCol = colPitch; rCol <= 1.0; rCol += colPitch){
         for(G4double gCol = colPitch; gCol <= 1.0; gCol += colPitch){
            for(G4double bCol = colPitch; bCol <= 1.0; bCol += colPitch){
               col[colIt++] = G4Color(rCol, gCol, bCol, 1.0);
            }
         }
      }
      for(G4int i = 0; i < kAtt; i++){
         if(fAttT[i] > 0.){
#ifdef NOTCPP11
            G4String name = "Att" + itos(kAtt - i);
#else
            G4String name = "Att" + std::to_string(kAtt - i);
#endif
            attLV[i] = ConstructAtt(name, attR, fAttT[i], col[i * (1000 / kAtt)]);
            G4double zPos = attOffset - fAttT[i] / 2.;
            G4ThreeVector attPos = G4ThreeVector(0., 0., zPos);
            fAttPV[i] = new G4PVPlacement(nullptr, attPos, attLV[i], name, fWorldLV,
                                          false, 0, fCheckOverlap);
         }
         attOffset -= fAttT[i];
         attR -= kAttPitch;
      }
   }
   
   G4LogicalVolume *filmLV = ConstructFilm();
   G4double filmZ = plateZ + fPlateH / 2. + fFilmT / 2.;
   G4ThreeVector filmPos = G4ThreeVector(0., 0., filmZ);
   fFilmPV = new G4PVPlacement(nullptr, filmPos, filmLV, "Film", airLV,
                               false, 0, fCheckOverlap);

   G4LogicalVolume *cassetteLV = ConstructCassette();
   G4double cassetteZ = fHolderT / 2.;
   G4ThreeVector cassettePos = G4ThreeVector(0., 0., cassetteZ);
   fCassettePV = new G4PVPlacement(nullptr, cassettePos, cassetteLV, "Cassette", airLV,
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
{
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

   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Sealing", outerS, maskS, nullptr, maskPos);

   G4LogicalVolume *outerLV;
   if(fCutGeo) outerLV = new G4LogicalVolume(halfS, fPlateMat, "Outer");
   else{
      outerLV = new G4LogicalVolume(outerS, fPlateMat, "Outer");
      delete halfS;
   }
   
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

   G4int kColumn = 0;
   if(fCutGeo) kColumn = 6;
   
   for(G4int iRow = 0; iRow < 8; iRow++){
      for(G4int iColumn = kColumn; iColumn < 12; iColumn++){
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

   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Film", filmS, maskS, nullptr, maskPos);

   G4LogicalVolume *filmLV;
   if(fCutGeo) filmLV = new G4LogicalVolume(halfS, fFilmMat, "Film");
   else{
      filmLV = new G4LogicalVolume(filmS, fFilmMat, "Film");
      delete halfS;
   }
   
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

   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Casette", unionS, maskS, nullptr, maskPos);

   G4LogicalVolume *cassetteLV;
   if(fCutGeo) cassetteLV = new G4LogicalVolume(halfS, fCassetteMat, "Cassette");
   else{
      cassetteLV = new G4LogicalVolume(unionS, fCassetteMat, "Cassette");
      delete halfS;
   }
      
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
   G4SubtractionSolid *topS = new G4SubtractionSolid("Holder", boardS, openingS);

   G4Box *bumpS = new G4Box("Holder", (fOpeningL + fBumpW) / 2., (fOpeningW + fBumpW) / 2.,
                            fBumpT / 2.);
   G4SubtractionSolid *bottomS = new G4SubtractionSolid("Holder", bumpS, openingS);

   G4ThreeVector bottomPos(0., 0., (fHolderT + fBumpT) / 2.);
   G4UnionSolid *holderS = new G4UnionSolid("Holder", topS, bottomS, nullptr, bottomPos);
   
   G4LogicalVolume *holderLV = new G4LogicalVolume(holderS, fHolderMat, "Holder");
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Green());
   visAttributes->SetVisibility(true);
   holderLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return holderLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructSimpleHolder()
{
   G4Box *boardS = new G4Box("Holder", fHolderL / 2., fHolderW / 2., fHolderT / 2.);
   G4Box *openingS = new G4Box("Opening", fBumpW + fOpeningL / 2., fBumpW + fOpeningW / 2.,
                               fHolderT);
   G4SubtractionSolid *holderS = new G4SubtractionSolid("Holder", boardS, openingS);
   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Holder", holderS, maskS, nullptr, maskPos);
   
   G4LogicalVolume *holderLV;
   if(fCutGeo) holderLV = new G4LogicalVolume(halfS, fHolderMat, "Holder");
   else{
      holderLV = new G4LogicalVolume(holderS, fHolderMat, "Holder");
      delete halfS;
   }
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Green());
   visAttributes->SetVisibility(true);
   holderLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return holderLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructWindow()
{
   G4Box *windowS = new G4Box("Window", fWindowL / 2., fWindowW / 2., fWindowT / 2.);
   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Window", windowS, maskS, nullptr, maskPos);

   
   G4LogicalVolume *windowLV;
   if(fCutGeo) windowLV = new G4LogicalVolume(halfS, fWindowMat, "Window");
   else{
      windowLV = new G4LogicalVolume(windowS, fWindowMat, "Window");
      delete halfS;
   }
      
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   windowLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return windowLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructFoil()
{
   G4Box *foilS = new G4Box("Foil", fFoilL / 2., fFoilW / 2., fFoilT / 2.);
   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Foil", foilS, maskS, nullptr, maskPos);

   
   G4LogicalVolume *foilLV;
   if(fCutGeo) foilLV = new G4LogicalVolume(halfS, fFoilMat, "Foil");
   else{
      foilLV = new G4LogicalVolume(foilS, fFoilMat, "Foil");
      delete halfS;
   }
      
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   foilLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return foilLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructSealing()
{
   G4Box *boardS = new G4Box("Sealing", fSealingL / 2., fSealingW / 2., fSealingT / 2.);
   G4Box *openingS = new G4Box("Opening", fOpeningL / 2., fOpeningW / 2., fSealingT);
   G4SubtractionSolid *sealingS = new G4SubtractionSolid("Sealing", boardS, openingS);
   G4Box *maskS = new G4Box("Mask", 1.*m / 2., 1.*m / 2., 1.*m / 2.);
   G4ThreeVector maskPos(-1.*m / 2., 0., 0.);
   G4SubtractionSolid *halfS = new G4SubtractionSolid("Sealing", sealingS, maskS, nullptr, maskPos);

   G4LogicalVolume *sealingLV;
   if(fCutGeo) sealingLV = new G4LogicalVolume(halfS, fSealingMat, "Sealing");
   else{
      sealingLV = new G4LogicalVolume(sealingS, fSealingMat, "Sealing");
      delete halfS;
   }
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   sealingLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return sealingLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructTileAtt(G4String name, G4double t)
{
   G4Box *attS = new G4Box(name, fTileAttL / 2., fTileAttL / 2., t / 2.);
   G4LogicalVolume *attLV = new G4LogicalVolume(attS, fAttMat, name);

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Brown());
   visAttributes->SetVisibility(true);
   attLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return attLV;
}

G4LogicalVolume *BIDetectorConstruction::ConstructAtt(G4String name, G4double R, G4double T,
                                                     G4Color col)
{
   G4LogicalVolume *attLV;
   G4ThreeVector maskPos = G4ThreeVector(0., 0., 0.);
   G4Tubs *attS = new G4Tubs("layer", 0., R, T / 2., 0., 360.*deg);
   G4Box *maskS;
   
   if(fCutGeo){
      maskS = new G4Box("Mask", fOpeningL / 4., fOpeningW / 2., 1.*m);
      maskPos = G4ThreeVector(fOpeningL / 4., 0., 0.);
   }
   else maskS = new G4Box("Mask", fOpeningL / 2., fOpeningW / 2., 1.*m);
   
   G4IntersectionSolid *intS = new G4IntersectionSolid(name, attS, maskS, nullptr, maskPos);
   attLV = new G4LogicalVolume(intS, fAttMat, name);
   //attLV = new G4LogicalVolume(attS, fAttMat, name);

   G4VisAttributes *visAttributes = new G4VisAttributes(col);
   visAttributes->SetVisibility(true);
   attLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return attLV;
}

void BIDetectorConstruction::ConstructSDandField()
{
   if(fForGrid){
      G4VSensitiveDetector *smallSD = new BISmallSD("SmallSD", "CommonHC");
      G4SDManager::GetSDMpointer()->AddNewDetector(smallSD);
      SetSensitiveDetector("Cell", smallSD);
   }
   else{
      G4VSensitiveDetector *commonSD = new BICommonSD("CommonSD", "CommonHC");
      G4SDManager::GetSDMpointer()->AddNewDetector(commonSD); 
      G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
      std::vector<G4LogicalVolume*>::const_iterator it;
      for(it = lvStore->begin(); it != lvStore->end(); it++){
         if((*it)->GetName() != "World")
            SetSensitiveDetector((*it)->GetName(), commonSD);
      }
   }

}

void BIDetectorConstruction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/BI/Geometry/", 
                                       "For geometries");

   
   G4GenericMessenger::Command &windowTCmd
      = fMessenger->DeclareMethodWithUnit("WindowThickness", "um",
                                          &BIDetectorConstruction::SetWindowT, 
                                          "Set the thickness of the window.");
   windowTCmd.SetParameterName("thickness", true);
   windowTCmd.SetRange("thickness>=0. && thickness<=10000.");
   windowTCmd.SetDefaultValue("500.0");

   G4GenericMessenger::Command &windowMatCmd
      = fMessenger->DeclareMethod("WindowMaterial",
                                  &BIDetectorConstruction::SetWindowMat, 
                                  "Set the material of the window.");
   windowMatCmd.SetParameterName("matName", true);

   G4GenericMessenger::Command &foilTCmd
      = fMessenger->DeclareMethodWithUnit("FoilThickness", "um",
                                          &BIDetectorConstruction::SetFoilT, 
                                          "Set the thickness of the foil.");
   foilTCmd.SetParameterName("thickness", true);
   foilTCmd.SetRange("thickness>=0. && thickness<=10000.");
   foilTCmd.SetDefaultValue("500.0");

   G4GenericMessenger::Command &foilMatCmd
      = fMessenger->DeclareMethod("FoilMaterial",
                                  &BIDetectorConstruction::SetFoilMat, 
                                  "Set the material of the foil.");
   foilMatCmd.SetParameterName("matName", true);

   G4GenericMessenger::Command &cassetteMatCmd
      = fMessenger->DeclareMethod("CassetteMaterial",
                                  &BIDetectorConstruction::SetCassetteMat, 
                                  "Set the material of the cassette.");
   cassetteMatCmd.SetParameterName("matName", true);

   G4GenericMessenger::Command &airGapTCmd
      = fMessenger->DeclareMethodWithUnit("AirGapThickness", "mm",
                                          &BIDetectorConstruction::SetAirGapT, 
                                          "Set the thickness of the air gap.");
   airGapTCmd.SetParameterName("thickness", true);
   airGapTCmd.SetRange("thickness>=0. && thickness<=50.");
   airGapTCmd.SetDefaultValue("50.0");

}

void BIDetectorConstruction::SetWindowT(G4double t)
{
   G4cout << t << G4endl;
   fWindowT = t;
   G4Box *window = (G4Box*)(fWindowPV->GetLogicalVolume()->GetSolid());
   window->SetZHalfLength(fWindowT / 2.);

   G4ThreeVector sealingPos = G4ThreeVector(0., 0., -(fWindowT + fSealingT) / 2.);
   fSealingPV->SetTranslation(sealingPos);
   
   G4ThreeVector airPos = G4ThreeVector(0., 0., (fWindowT + fAirT) / 2.);
   fAirPV->SetTranslation(airPos);
   
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void BIDetectorConstruction::SetWindowMat(G4String matName)
{
   G4NistManager *manager = G4NistManager::Instance();
   G4Material *mat;
   mat = manager->FindOrBuildMaterial(matName);
   if(mat == nullptr){
      G4cout << matName << " is not a defined material.\n"
             << "Window material is not changed." << G4endl;
      //exit(0);
   }
   else{
      G4LogicalVolume *windowLV = fWindowPV->GetLogicalVolume();
      G4cout << "The material of window is changed from "
             << windowLV->GetMaterial()->GetName()
             << " to " << mat->GetName() <<". "<< G4endl;
      windowLV->SetMaterial(mat);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
}

void BIDetectorConstruction::SetFoilT(G4double t)
{
   G4cout << t << G4endl;
   fFoilT = t;
   G4Box *foil = (G4Box*)(fFoilPV->GetLogicalVolume()->GetSolid());
   foil->SetZHalfLength(fFoilT / 2.);

   G4ThreeVector sealingPos = G4ThreeVector(0., 0., -(fFoilT + fSealingT) / 2.);
   fSealingPV->SetTranslation(sealingPos);
   
   G4ThreeVector airPos = G4ThreeVector(0., 0., (fFoilT + fAirT) / 2.);
   fAirPV->SetTranslation(airPos);
   
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void BIDetectorConstruction::SetFoilMat(G4String matName)
{
   G4NistManager *manager = G4NistManager::Instance();
   G4Material *mat;
   mat = manager->FindOrBuildMaterial(matName);
   if(mat == nullptr){
      G4cout << matName << " is not a defined material.\n"
             << "Foil material is not changed." << G4endl;
      //exit(0);
   }
   else{
      G4LogicalVolume *foilLV = fFoilPV->GetLogicalVolume();
      G4cout << "The material of foil is changed from "
             << foilLV->GetMaterial()->GetName()
             << " to " << mat->GetName() <<". "<< G4endl;
      foilLV->SetMaterial(mat);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
}

void BIDetectorConstruction::SetCassetteMat(G4String matName)
{
   G4NistManager *manager = G4NistManager::Instance();
   G4Material *mat;
   mat = manager->FindOrBuildMaterial(matName);
   if(mat == nullptr){
      G4cout << matName << " is not a defined material.\n"
             << "Cassette material is not changed." << G4endl;
      //exit(0);
   }
   else{
      G4LogicalVolume *cassetteLV = fCassettePV->GetLogicalVolume();
      G4cout << "The material of cassette is changed from "
             << cassetteLV->GetMaterial()->GetName()
             << " to " << mat->GetName() <<". "<< G4endl;
      cassetteLV->SetMaterial(mat);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
}

void BIDetectorConstruction::SetAirGapT(G4double t)
{
   G4cout << t << G4endl;
   fAirGap = t;
   G4double plateZ = -fAirT / 2. + fAirGap + (fWellH + fPlateT) / 2.;
   G4ThreeVector platePos = G4ThreeVector(0., 0., plateZ);
   fPlatePV->SetTranslation(platePos);
   
   G4double filmZ = plateZ + fPlateH / 2. + fFilmT / 2.;
   G4ThreeVector filmPos = G4ThreeVector(0., 0., filmZ);
   fFilmPV->SetTranslation(filmPos);

   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void BIDetectorConstruction::ReadAttData()
{
   G4double thickness[kAtt];

   std::ifstream fin(fAttFile);
   if(!fin.is_open()){
      G4cout << "Attenuator data file not found. Use all zero" << G4endl;
      for(G4int i = 0; i < kAtt; i++)
         fAttT[i] = 0;
   }
   else{
      fAttH = 0.;
      G4int it = 0;
      std::string buf;
      while(1){
         fin >> buf;
         if(it >= kAtt || fin.eof()) break;
         thickness[it] = stol(buf)*um;
         //G4cout << buf <<"\t"<< thickness[it] << G4endl;
         if(thickness[it] > fAttH) fAttH = thickness[it];
         it++;
      }
   
      fin.close();

      for(G4int i = 0; i < kAtt; i++){
         if(i <9 ) thickness[i] = thickness[9];
         else if (i > 66) thickness[i] = thickness[66];
         G4cout << thickness[i] << G4endl;
      }
      
      for(G4int i = 0; i < kAtt; i++){
         if(i == 0) fAttT[i] = thickness[i];
         else if(thickness[i] > thickness[i - 1]) fAttT[i] = thickness[i] - thickness[i - 1];
         else {
            thickness[i] = thickness[i - 1];
            fAttT[i] = 0;
         }
      }
   }

}
