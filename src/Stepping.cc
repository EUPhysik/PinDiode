#include "Stepping.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
{
	fEventAction = eventAction;
}

SteppingAction::~SteppingAction()
{

}

void SteppingAction::UserSteppingAction(const G4Step* step)
{

	G4LogicalVolume * volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4double edep = step->GetTotalEnergyDeposit();

	if (edep > 0) {
		G4cout << "Energy deposit: " << edep / keV << " keV in volume: " << volume->GetName() << G4endl; 
	}

	const MyDetectorConstruction* DtctrConstruction = static_cast <const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4LogicalVolume* fScoringVolume = DtctrConstruction->GetScoringVolume();
	
	if (volume == fScoringVolume) {

		if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
			auto track = step->GetTrack();
			if (track->GetParentID() == 0 && track->GetDefinition()->GetParticleName() == "gamma") {
				fEventAction->IncrementPhotonEntered();

			}
		}
		fEventAction->AddEdep(edep);
	}

	if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && volume == fScoringVolume) {
		G4cout << "Photon entered sensitive volume." << G4endl; 
	}
}