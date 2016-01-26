#ifndef BIDetectorConstruction_h
#define BIDetectorConstruction_h 1

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
   G4VPhysicalVolume *fAttPV[96];
   
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

   G4LogicalVolume *ConstructAtt(G4String, G4double);
   G4double fAttL;
   G4double fAttT[12][8];
   
   //G4double fAttT[12][8] = {{3.91392, 17.1604, 21.9134, 21.2907, 19.2882, 29.2778, 23.6306, 2.31686}, {14.7895, 27.4412, 43.7659, 28.0608, 33.379, 33.5245, 34.8886, 21.3795}, {44.9708, 49.8623, 56.5286, 59.669, 50.5819, 54.5409, 51.9093, 33.678}, {44.5565, 55.8554, 56.8911, 69.605, 64.1153, 60.8462, 49.9072, 41.7684}, {47.9317, 53.665, 70.0531, 68.2353, 78.9393, 69.3196, 61.6816, 60.7488}, {50.3241, 65.3353, 70.6074, 75.5942, 67.812, 67.5787, 63.4604, 56.9021}, {50.214, 61.8536, 69.0548, 74.4076, 80.1653, 64.7203, 52.6413, 59.0382}, {30.9497, 70.549, 69.956, 71.1589, 65.8088, 62.9623, 63.7693, 57.0522}, {44.0057, 55.0551, 68.9887, 70.2516, 65.7727, 63.0274, 47.543, 44.3712}, {41.5222, 45.5449, 44.0417, 56.7551, 56.4539, 49.3395, 46.895, 36.8659}, {24.6683, 29.1433, 45.4707, 50.8436, 37.9194, 33.8828, 37.1693, 27.52}, {16.4561, 7.90209, 30.3647, 26.4809, 28.0584, 25.4568, 27.4854, 0}}; // Al500

   //G4double fAttT[12][8] = {{6.80123, 14.115, 19.5693, 16.4193, 18.0594, 26.566, 16.7393, 1.37846}, {12.6811, 24.9709, 39.07, 23.0319, 35.4207, 28.7389, 32.0641, 18.712}, {40.208, 42.9113, 49.951, 54.6121, 37.0691, 48.1597, 41.866, 23.8004}, {40.5758, 46.7663, 49.6769, 62.0906, 50.2633, 54.7234, 42.2111, 36.6075}, {38.8468, 48.9406, 56.0496, 58.7492, 63.4704, 57.0603, 53.582, 46.3822}, {40.4481, 53.0062, 61.7602, 59.6169, 56.9807, 58.1104, 52.8987, 50.1282}, {42.0113, 52.5335, 53.504, 65.2705, 69.35, 58.8875, 43.2073, 50.9059}, {29.6366, 57.6518, 57.1079, 61.9524, 49.4934, 56.6617, 54.987, 42.9827}, {34.0169, 42.5904, 54.6555, 58.8497, 56.7755, 52.281, 39.9463, 36.7695}, {38.0409, 38.2438, 42.8704, 46.3559, 46.9049, 37.8294, 38.9678, 31.6336}, {15.5642, 21.7402, 39.5107, 40.437, 35.6182, 25.474, 35.3588, 22.265}, {10.2127, 6.09206, 25.1265, 23.0188, 19.717, 22.5522, 23.6551, 0}}; // Ni100

   //G4double fAttT[12][8] = {{2.65802, 10.5925, 17.7281, 18.0969, 14.0869, 24.8717, 17.96, 0.189648}, {9.83801, 30.5845, 35.0798, 24.678, 34.4237, 31.8068, 34.6226, 19.3531}, {40.6132, 51.0878, 59.832, 61.1435, 41.9508, 55.6256, 46.93, 25.2704}, {46.0696, 53.0901, 54.5069, 68.6448, 58.6201, 59.2493, 48.8194, 40.554}, {42.4179, 56.0973, 60.4348, 70.2345, 74.2924, 66.6769, 61.115, 56.2245}, {47.2729, 65.0622, 72.413, 69.9105, 68.4683, 69.5564, 59.0303, 56.8396}, {50.0246, 57.7576, 63.3969, 69.5153, 79.6195, 70.8283, 53.6694, 57.1386}, {28.4785, 67.1805, 73.9555, 71.5811, 60.7519, 66.4187, 64.0015, 49.5532}, {36.8931, 50.0239, 60.1623, 72.6956, 60.4725, 59.7033, 43.2692, 42.4832}, {36.951, 43.2536, 47.2189, 52.8112, 54.1248, 43.1472, 45.0877, 28.2944}, {20.3925, 26.8086, 42.0063, 46.1876, 42.1913, 27.1059, 39.1741, 20.3697}, {5.57987, 4.25578, 30.4629, 24.655, 23.668, 19.5156, 23.7221, 0}}; // Ni200

   
   std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
