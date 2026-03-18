#include "Generator.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

PrimaryGenerator::PrimaryGenerator()
{
	fParticleGun = nullptr;

	isHp10 = false; // Set to true to simulate Hp(10)

	simulatedDose_mSv = 0.;
	simulatedDoseRate_uSvPerH = 0.;

	G4int n_particle = 1; // Number of Photons per Event!! Can be Changed

	fParticleGun = new G4ParticleGun(n_particle);

	G4ParticleTable* ParticleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* Particle = ParticleTable->FindParticle("gamma");
	fParticleGun->SetParticleDefinition(Particle);
}
PrimaryGenerator::~PrimaryGenerator()
{
	delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
	

	G4double E_min, E_max;
	G4double energy;

	// Photon energy range (change depending on Hp(0.07) or Hp(10))
	if (isHp10)
	{
		G4String ParticleName = "Hp(10)";
		E_min = 80. * keV; // Lower bound for Hp(10)
		E_max = 1250. * keV; // Upper bound for Hp(10)

		simulatedDose_mSv = (G4UniformRand() * (1000. - 0.1)) + 0.1; // 0.1 mSv to 1000 mSv
		simulatedDoseRate_uSvPerH = (G4UniformRand() * (1e6 - 100)) + 100; // 0.1 µSv/h to 1 Sv/h
	}
	else
	{
		G4String ParticleName = "Hp(0,07)";
		E_min = 30. * keV;   // Lower bound for Hp(0.07)
		E_max = 250. * keV;  // Upper bound for Hp(0.07)

		simulatedDose_mSv = (G4UniformRand() * (10000. - 1.0)) + 1.0; // 1 mSv to 10000 mSv
		simulatedDoseRate_uSvPerH = (G4UniformRand() * (1e6 - 0.1e3)) + 0.1e3; // 0.1 µSv/h to 1 Sv/h
	}

	//energy = G4UniformRand() * (E_max - E_min) + E_min;
	energy = 250 * keV;

	//----------------------------------------
	// Geometry parameters (customize as needed)
	//----------------------------------------

	// Diode center position
	G4ThreeVector diodePos(0., 0.425 * mm, 0.);

	// Photon Start Point
	G4double y_start = 8. * mm; // Make sure this is above all 5 layers

	//----------------------------------------
	// Generate randomized direction within ±60° cone
	//----------------------------------------

	G4double maxAngleRad = 60. * deg;

	// Random direction in spherical coordinates
	G4double theta = G4UniformRand() * maxAngleRad;      // polar angle from vertical
	G4double phi = G4UniformRand() * 2.0 * pi;           // azimuthal angle

	// Base direction (vertical downward)
	G4ThreeVector baseDir(0., -1., 0.);

	// Build orthonormal basis
	G4ThreeVector u = baseDir.orthogonal().unit();       // any perpendicular vector
	G4ThreeVector v = baseDir.cross(u).unit();           // complete the basis

	// Apply deviation in the ±60° cone
	G4ThreeVector dir = std::cos(theta) * baseDir
		+ std::sin(theta) * std::cos(phi) * u
		+ std::sin(theta) * std::sin(phi) * v;
	dir = dir.unit();

	// -------- Guard: avoid upward directions --------
	if (dir.y() >= -0.01) {
		dir = G4ThreeVector(0., -1., 0.); // fallback to vertical
	}

	//----------------------------------------
	// Compute starting point so that the ray hits the diode
	//----------------------------------------

	G4double t = (y_start - diodePos.y()) / dir.y();  // how far up to place the photon
	G4ThreeVector pos = diodePos + t * dir;

	//G4ThreeVector target = diodePos;
	//G4ThreeVector pos = target - (y_start - diodePos.y()) / dir.y() * dir;


	// Set photon parameters
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(dir);
	fParticleGun->SetParticleEnergy(energy);


	//G4cout << "Simulated Dose: " << simulatedDose_mSv << " mSv" << G4endl;  
	//G4cout << "Simulated Dose Rate: " << simulatedDoseRate_uSvPerH << " µSv/h" << G4endl; 

	// Generate primary
	fParticleGun->GeneratePrimaryVertex(anEvent);
}