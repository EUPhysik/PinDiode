#ifndef Event_hh
#define Event_hh

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include <fstream>

#include "Run.hh"
#include "Generator.hh"

class EventAction : public G4UserEventAction
{
public:
	EventAction(RunAction*);
	~EventAction();

	virtual void BeginOfEventAction(const G4Event* );

	virtual void EndOfEventAction(const G4Event* );

	void AddEdep(G4double edep);

	void IncrementPhotonEntered();
	void IncrementPhotonDeposited();
	void PrintStats();

private:
	G4double fEdep;
	G4int fEventID;
	G4double fPhotonEnergy;

	G4int photonsEntered = 0;
	G4int photonsDeposited = 0;
};

#endif