#ifndef Construction_hh
#define Construction_hh

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "Detector.hh"

/// Detector construction class for materials and geometry.
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

   virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume* logicPinD;

    virtual void ConstructSDandField();

    G4LogicalVolume *fScoringVolume;

    G4bool isWater;
};

#endif