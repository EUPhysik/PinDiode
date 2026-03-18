#include "Detector.hh"
#include "Construction.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{

}
SensitiveDetector::~SensitiveDetector()
{

}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
   // G4Track* track = aStep->GetTrack();

    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false; 
 
   // track->SetTrackStatus(fStopAndKill);
    return true;
}