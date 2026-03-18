// Defines the entry point for the Project.
//
//#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4Vismanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4String.hh"

#include "Construction.hh"
#include "PhysicsList.hh"
#include "Action.hh"


int main(int argc, char** argv)
{
	// ----------------------UI/RunManager----------------------

	G4UIExecutive* ui = nullptr;

#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager();
#else
	G4RunManager* runManager = new G4RunManager();
#endif // G4MULTITHREADED

	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new ActionInitialization());

	//runManager->Initialize();

	// get pointer to UI manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	

	

	if (argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);

		G4VisManager* visManager = new G4VisExecutive();
		visManager->Initialize();

		UImanager->ApplyCommand("/control/execute run.mac");
		UImanager->ApplyCommand("/control/execute vis.mac"); //On for Vis Debug
		ui->SessionStart();

		delete visManager;
		delete ui;
	}

	else
	{
		G4String command = "/control/execute ";
		G4String filename = argv[1];

		UImanager->ApplyCommand(command + filename);
	}

	delete runManager;
	return 0;
}