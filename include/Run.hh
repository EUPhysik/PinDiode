#ifndef Run_hh
#define Run_hh

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

class RunAction : public G4UserRunAction
{
public:
	RunAction();
	~RunAction();

	virtual void BeginOfRunAction(const G4Run*);

	virtual void EndOfRunAction(const G4Run*);
};

#endif