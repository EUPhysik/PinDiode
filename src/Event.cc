#include "Event.hh"

EventAction::EventAction(RunAction*) 
{

}

EventAction::~EventAction() 
{

}

void EventAction::BeginOfEventAction(const G4Event* event)
{
	fEdep = 0.;
	fPhotonEnergy = 0.;
	fEventID = event->GetEventID();
}

void EventAction::AddEdep(G4double edep)
{
	fEdep += edep;

	if (edep > 0) {
		photonsDeposited++;
	}
}

void EventAction::IncrementPhotonEntered()
{
	photonsEntered++;
}

void EventAction::PrintStats()
{
	G4cout << "Photons Entered: " << photonsEntered
		<< ", Deposited Energy: " << photonsDeposited
		<< ", Fraction: " << (G4double)photonsDeposited / photonsEntered << G4endl;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
	//G4cout << "EventID: " << fEventID << G4endl;

	//PrimaryGenerator* generator = static_cast<PrimaryGenerator*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

	auto evt = G4RunManager::GetRunManager()->GetCurrentEvent();
	G4PrimaryParticle* primary = evt->GetPrimaryVertex(0)->GetPrimary(0);
	G4double photonEnergy = primary->GetTotalEnergy() / keV;

	//G4AnalysisManager* man = G4AnalysisManager::Instance();

	// Get photon energy
	G4PrimaryVertex* vertex = event->GetPrimaryVertex(0);
	if (vertex) {
		G4PrimaryParticle* particle = vertex->GetPrimary(0);
		if (particle) {
			fPhotonEnergy = particle->GetTotalEnergy() / keV; // in keV
		}
	}
	//man->FillNtupleIColumn(0, 0, fEventID);
	//man->FillNtupleFColumn(0, 1, fPhotonEnergy);
	//man->FillNtupleDColumn(0, 2, fEdep / keV);
	//man->AddNtupleRow(0);
	
	static int count = 0;
	count++;

	if (count % 100 == 0) {
		PrintStats();
	}

	// CSV output
	G4int threadID = G4Threading::G4GetThreadId();
	std::stringstream ss;
	ss << "Xoutput" << threadID << ".csv";

	
	std::ofstream outFile(ss.str(), std::ios::app);
	if (fEventID == 0) {
		// Write header only once
		outFile << "EventID,PhotonEnergy_keV,EnergyDeposition_keV\n";
	}
	
	/*
	// check if the file exists already
	std::ifstream check(ss.str());
	bool exists = check.good();
	check.close();

	std::ofstream outFile;
	if (!exists) {
		outFile.open(ss.str(), std::ios::out);
		outFile << "EventID,PhotonEnergy_keV,EnergyDeposition_keV\n";
	}
	else {
		outFile.open(ss.str(), std::ios::app);
	}
	*/
	outFile << fEventID << "," << fPhotonEnergy << "," << fEdep / keV << "\n";
	outFile.close();
}