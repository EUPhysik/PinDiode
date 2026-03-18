#include "Action.hh"

ActionInitialization::ActionInitialization()
{

}
ActionInitialization::~ActionInitialization()
{

}

void ActionInitialization::BuildForMaster() const
{
	RunAction* runAct = new RunAction();
	SetUserAction(runAct);
}

void ActionInitialization::Build() const
{
	PrimaryGenerator* generator = new PrimaryGenerator();
	SetUserAction(generator);

	RunAction* runAct = new RunAction();
	SetUserAction(runAct);

	EventAction* eventAct = new EventAction(runAct);
	SetUserAction(eventAct);

	SteppingAction* steppingAct = new SteppingAction(eventAct);
	SetUserAction(steppingAct);
}