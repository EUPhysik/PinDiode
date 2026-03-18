#ifndef Action_hh
#define Action_hh

#include "G4VUserActionInitialization.hh"

#include "Generator.hh"
#include "Run.hh"
#include "Event.hh"
#include "Stepping.hh"

class ActionInitialization : public G4VUserActionInitialization
{
	public:
		ActionInitialization();
		~ActionInitialization();

		virtual void Build() const;
		virtual void BuildForMaster() const;
};

#endif