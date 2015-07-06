#include <time.h>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#include "G4Threading.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "Shielding.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4TrajectoryParticleFilter.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// Remove?
#include "Randomize.hh"

#include "BIPhysicsList.hpp"
#include "BIDetectorConstruction.hpp"
#include "BIActionInitialization.hpp"


namespace
{
   void PrintUsage()
   {
      G4cerr << " Usage: " << G4endl;
      G4cerr << " ./LG [-m macro filename]\n"
             << " -a Show all trajectory (default show only ploton)\n"
             << G4endl;
   }
}

int main(int argc, char **argv)
{
   G4String macro = "";
   G4bool showAll = false;
   for (G4int i = 1; i < argc; i++) {
      if (G4String(argv[i]) == "-m") macro = argv[++i];
      else if (G4String(argv[i]) == "-a") showAll = true;
      else {
         PrintUsage();
         return 1;
      }
   }

   // Remove?
   // Choose the Random engine
   CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
   G4int seed = time(0);
   G4cout << "\nseed = " << seed << G4endl;
   CLHEP::HepRandom::setTheSeed(seed);
   G4Random::setTheSeed(seed);

   // Construct the default run manager
   //

#ifdef G4MULTITHREADED
   G4MTRunManager *runManager = new G4MTRunManager();
   runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
   //runManager->SetNumberOfThreads(1);
#else
   G4RunManager *runManager = new G4RunManager();
#endif

   // Set mandatory initialization classes
   //
   // Detector construction
   runManager->SetUserInitialization(new BIDetectorConstruction());
   G4cout << "Here" << G4endl;
   // Physics list
   //G4VModularPhysicsList *physicsList = new FTFP_BERT;
   //G4VModularPhysicsList *physicsList = new QGSP_BERT_HP;
   G4VModularPhysicsList *physicsList = new Shielding;
   //G4VModularPhysicsList *physicsList = new BIPhysicsList;
   physicsList->SetVerboseLevel(0);
   //physicsList->SetCutValue(1.*um, "proton");
   //physicsList->SetCuts();
   //physicsList->SetDefaultCutValue(100.*um);
   runManager->SetUserInitialization(physicsList);

   // Primary generator action and User action intialization
   runManager->SetUserInitialization(new BIActionInitialization());

   // Initialize G4 kernel
   //
   runManager->Initialize();

#ifdef G4VIS_USE
   // Initialize visualization
   G4VisManager *visManager = new G4VisExecutive;
   visManager->Initialize();
   
   if (!showAll) { //Show only proton
      G4TrajectoryParticleFilter *filterp = new G4TrajectoryParticleFilter;
      filterp->Add("proton");
      visManager->RegisterModel(filterp);
   }
#endif

   // Get the pointer to the User Interface manager
   G4UImanager *UImanager = G4UImanager::GetUIpointer();

   if (macro != "") {
      // batch mode
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command + macro);
   } else {
      // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
      UImanager->ApplyCommand("/control/execute init.mac");
#endif
      if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute icons.mac");
      }
      ui->SessionStart();
      delete ui;
#endif
   }

   // Job termination
   // Free the store: user actions, physics_list and detector_description are
   // owned and deleted by the run manager, so they should not be deleted
   // in the main() program !

#ifdef G4VIS_USE
   delete visManager;
#endif

   delete runManager;

   return 0;
}
