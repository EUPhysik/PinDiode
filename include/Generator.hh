#ifndef Generator_hh
#define Generator_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGenerator();
	~PrimaryGenerator();

	G4double GetSimulatedDose() const { return simulatedDose_mSv; }
	G4double GetSimulatedDoseRate() const { return simulatedDoseRate_uSvPerH; }

	virtual void GeneratePrimaries(G4Event*);

private:
	G4ParticleGun* fParticleGun; //G4 gun class
	
	//G4GeneralParticleSource* fParticleGun;
	
	G4bool isHp10;

	G4double simulatedDose_mSv;
	G4double simulatedDoseRate_uSvPerH;
};

#endif