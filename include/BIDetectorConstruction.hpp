#ifndef BIDetectorConstruction_h
#define BIDetectorConstruction_h 1

#ifdef NOTCPP11
#define nullptr NULL
#endif

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"


class BIDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   BIDetectorConstruction(G4bool forGrid);
   virtual ~BIDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

   // UI Commands
   void SetWindowT(G4double t);
   void SetWindowMat(G4String matName);
   void SetFoilT(G4double t);
   void SetFoilMat(G4String matName);
   void SetCassetteMat(G4String matName);
   void SetAirGapT(G4double t);
   
private:
   G4bool fCut;
   G4bool fForGrid;
   
   G4LogicalVolume *fWorldLV;
   G4bool fCheckOverlap;

   // Commands
   void DefineCommands();
   G4GenericMessenger *fMessenger;
   G4VPhysicalVolume *fWindowPV;
   G4VPhysicalVolume *fFoilPV;
   G4VPhysicalVolume *fAirPV;
   G4VPhysicalVolume *fSealingPV;
   G4VPhysicalVolume *fHolderPV;
   G4VPhysicalVolume *fPlatePV;
   G4VPhysicalVolume *fFilmPV;
   G4VPhysicalVolume *fCassettePV;
   
   // Materials
   void DefineMaterial();
   G4Material *fVacuum;
   G4Material *fAir;
   G4Material *fWindowMat;
   G4Material *fFoilMat;
   G4Material *fCassetteMat;
   G4Material *fPlateMat;
   G4Material *fHolderMat;
   G4Material *fSealingMat;
   G4Material *fCellMat;
   G4Material *fAttMat;
   G4Material *fFilmMat;
   G4Material *fStuffMat;
   
   // Geometries
   // Do I really need all members? too much?
   void DefineGeoPar();
   
   G4LogicalVolume *ConstructPlate();
   G4double fPlateT;
   G4double fPlateL;
   G4double fPlateW;
   G4double fPlateH;
   G4double fAirGap;
   
   G4LogicalVolume *ConstructFilm();
   G4double fFilmT;
   G4double fFilmL;
   G4double fFilmW;

   G4LogicalVolume *ConstructWell();
   G4double fWellPitch;
   G4double fWellOpening;
   G4double fWellBottom;
   G4double fWellH;

   G4LogicalVolume *ConstructStuff();
   G4double fCellT;
   
   G4LogicalVolume *ConstructCassette();
   G4double fCassetteL;
   G4double fCassetteW;
   G4double fCassetteH;
   G4double fCassetteBottomT;

   G4LogicalVolume *ConstructHolder();
   G4LogicalVolume *ConstructSimpleHolder();
   G4double fHolderL;
   G4double fHolderW;
   G4double fHolderT;

   G4double fBumpT;
   G4double fBumpW;

   // Holder and Cassette are at air layer
   G4double fAirL;
   G4double fAirW;
   G4double fAirT;
   
   G4LogicalVolume *ConstructWindow();
   G4double fWindowL;
   G4double fWindowW;
   G4double fWindowT;

   G4LogicalVolume *ConstructFoil();
   G4double fFoilL;
   G4double fFoilW;
   G4double fFoilT;

   G4LogicalVolume *ConstructSealing();
   G4double fSealingL;
   G4double fSealingW;
   G4double fSealingT;

   G4double fOpeningL;
   G4double fOpeningW;

   void ReadAttData();
   G4LogicalVolume *ConstructAtt(G4String, G4double, G4double);
   static const G4int kAtt = 75;
   G4double fAttT[kAtt];
   G4double fAttH; // sum of fAttT
   G4VPhysicalVolume *fAttPV[kAtt];
   
   std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
