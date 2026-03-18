#ifndef Detector_hh
#define Detector_hh

#include "Run.hh"

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

#include <vector>

class SensitiveDetector : public G4VSensitiveDetector 
{
public:
	SensitiveDetector(G4String);
	~SensitiveDetector();

private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};

#endif