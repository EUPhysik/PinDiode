#ifndef Stepping_hh
#define Stepping_hh

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

#include "Construction.hh"
#include "Event.hh"
#include "Run.hh"


class SteppingAction : public G4UserSteppingAction
{
public:
	SteppingAction(EventAction* eventAction); 
	~SteppingAction();

	virtual void UserSteppingAction(const G4Step*);

private:
	
	EventAction* fEventAction;
};

#endif