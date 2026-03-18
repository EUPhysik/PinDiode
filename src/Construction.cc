#include "Construction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	isWater = true; // Set to true to simulate water diode without Layers
}
MyDetectorConstruction::~MyDetectorConstruction()
{

}
G4VPhysicalVolume * MyDetectorConstruction::Construct()
{
	//Materials

	//Carbon
	//Name,Symbol,Zeff,Aeff
	G4Element* elC = new G4Element("Carbon", "C", 6., 12.01 * g / mole);

	//Hydrogen
	G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.01 * g / mole);

	//Silicon
	G4Element* elSi = new G4Element("Silicon", "Si", 14, 28.09 * g / mole);

	//Oxygen
	G4Element* elO = new G4Element("Oxygen", "O", 8, 16.00 * g / mole);

	//Nitrogen
	G4Element* elN = new G4Element("Nitrogen", "N", 7., 14.0 * g / mole);

	//Epoxy
	//Name,Density,nComponents
	G4Material* Epoxy = new G4Material("Epoxy", 1.2 * g / cm3, 2);
	Epoxy->AddElement(elH, 2);
	Epoxy->AddElement(elC, 2);

	//SiO2 (Quarz)
	G4Material* SiO2 = new G4Material("SiO2", 2.200 * g / cm3, 2);
	SiO2->AddElement(elSi, 1);
	SiO2->AddElement(elO, 2);

	//FR4 (Glass + Epoxy)
	G4Material* FR4 = new G4Material("FR4", 1.86 * g / cm3, 2);
	FR4->AddMaterial(Epoxy, 47.2 * perCent);
	FR4->AddMaterial(SiO2, 52.8 * perCent);


	//World material
	G4NistManager* nist = G4NistManager::Instance();
	G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

	//World volumes 10mm
	G4Box* solidWorld = new G4Box("solidWorld", 10 * mm, 10 * mm, 10 * mm);

	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

	//World Visibility
	G4VisAttributes* WolrdVisAtt = new G4VisAttributes();
	WolrdVisAtt->SetForceWireframe(true);
	logicWorld->SetVisAttributes(WolrdVisAtt);

	//rotation, Position, LogicVolume, Name, motherVolume, boolean Op, Copy Nm., Overlap Checking
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	//Full size: 1.5 mm × 0.85 mm × 1.5 mm (half-sizes used in constructor).
	//Material: Silicon or Water
	
	if (isWater) {
		//PinD material
		G4Material* PinDMat = nist->FindOrBuildMaterial("G4_WATER");
		//PinD volumes
		G4Box* solidPinD = new G4Box("solidPinD", 0.75 * mm, 0.425 * mm, 0.75 * mm);

		logicPinD = new G4LogicalVolume(solidPinD, PinDMat, "logicPinD");

		G4VisAttributes* PinDVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.)); //Blau
		PinDVisAtt->SetForceWireframe(true);
		logicPinD->SetVisAttributes(PinDVisAtt);

		G4VPhysicalVolume* physPinD = new G4PVPlacement(0, G4ThreeVector(0., 0.425 * mm, 0.), logicPinD, "physPinD", logicWorld, false, 1, true);

		//Diode as ScoringVolume
		fScoringVolume = logicPinD;
	}
	else {
		//PinD material
		G4Material* PinDMat = nist->FindOrBuildMaterial("G4_Si");
		//PinD volumes
		G4Box* solidPinD = new G4Box("solidPinD", 0.75 * mm, 0.425 * mm, 0.75 * mm);

		logicPinD = new G4LogicalVolume(solidPinD, PinDMat, "logicPinD");

		G4VisAttributes* PinDVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.)); //Blau
		PinDVisAtt->SetForceWireframe(true);
		logicPinD->SetVisAttributes(PinDVisAtt);

		G4VPhysicalVolume* physPinD = new G4PVPlacement(0, G4ThreeVector(0., 0.425 * mm, 0.), logicPinD, "physPinD", logicWorld, false, 1, true);

		//Diode as ScoringVolume
		fScoringVolume = logicPinD;
	}
	
	if (isWater)
	{
		
	}
	else {
		//First Cover material
		G4Material* layerOneMat = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");

		//First Cover volumes
		G4Box* solidLayerOneout = new G4Box("solidLayerOneoutShape", 3.5 * mm, 0.425 * mm, 3.5 * mm); //OuterBox
		G4Box* solidLayerOnein = new G4Box("solidLayerOneinShape", 2.5 * mm, 0.425 * mm, 2.5 * mm);	//innerBox

		G4SubtractionSolid* solidLayerOne = new G4SubtractionSolid("solidLayerOneShape", solidLayerOneout, solidLayerOnein);

		G4LogicalVolume* logicLayerOne = new G4LogicalVolume(solidLayerOne, layerOneMat, "logicLayerOneShape");

		G4VisAttributes* LOneVisAtt = new G4VisAttributes(G4Colour(0., 1., 0.)); //Gruen
		LOneVisAtt->SetForceWireframe(true);
		logicLayerOne->SetVisAttributes(LOneVisAtt);

		//Around the PinDiode
		G4VPhysicalVolume* physLayerOne = new G4PVPlacement(0, G4ThreeVector(0., 0.425 * mm, 0.), logicLayerOne, "LayerOne", logicWorld, false, 0, true);

		//Full thickness: 0.02 mm.
		//Material:Kapton Foil
		//Purpose: Acts as a thin entrance window, commonly used due to its flexibility and radiation tolerance.

		//Second Cover material
		G4Material* layerTwoMat = nist->FindOrBuildMaterial("G4_KAPTON");

		//Second Cover volumes
		G4Box* solidLayerTwo = new G4Box("solidLayerTwoShape", 3.5 * mm, 0.01 * mm, 3.5 * mm);

		G4LogicalVolume* logicLayerTwo = new G4LogicalVolume(solidLayerTwo, layerTwoMat, "logicLayerTwoShape");

		G4VisAttributes* LTwoVisAtt = new G4VisAttributes(G4Colour(1., 0., 1.)); //Lila
		LTwoVisAtt->SetForceWireframe(true);
		logicLayerTwo->SetVisAttributes(LTwoVisAtt);

		// From bottom: Diode: 0.85 + Half of Kapton (0.02 mm / 2 = 0.01) = Final center Y = 1.71 mm
		G4VPhysicalVolume* physLayerTwo = new G4PVPlacement(0, G4ThreeVector(0, 0.86 * mm, 0), logicLayerTwo, "LayerTwo", logicWorld, false, 0, true);

		//Full thickness: 0.35 mm.
		//Material: Aluminum
		//Purpose: Shields and structural support.
		//Note: Subtraction gives a very thin-walled shell, which can be used to simulate a slight gap inside.

		//Third Cover material
		G4Material* layerThreeMat = nist->FindOrBuildMaterial("G4_Al");

		//Third Cover volumes
		//G4Box* solidLayerThreeout = new G4Box("solidLayerThreeoutShape", 5 * mm, 0.175 * mm, 5 * mm);
		//G4Box* solidLayerThreein = new G4Box("solidLayerThreeinShape", 4.9 * mm, 0.173 * mm, 4.9 * mm);

		//G4SubtractionSolid* solidLayerThree = new G4SubtractionSolid("solidLayerThreeShape", solidLayerThreeout, solidLayerThreein);

		G4Box* solidLayerThree = new G4Box("solidLayerThreeShape", 5 * mm, 0.175 * mm, 5 * mm);

		G4LogicalVolume* logicLayerThree = new G4LogicalVolume(solidLayerThree, layerThreeMat, "logicLayerThreeShape");

		G4VisAttributes* LThreeVisAtt = new G4VisAttributes(G4Colour(1., 1., 0.)); //Gelb
		LThreeVisAtt->SetForceWireframe(true);
		LThreeVisAtt->SetForceAuxEdgeVisible(true);
		logicLayerThree->SetVisAttributes(LThreeVisAtt);

		// After: Kapton top: 0.86 + 0.01 + Air gap: 0.5 + Half of Al layer: 0.175 = Final center Y = 1.545 mm
		G4VPhysicalVolume* physLayerThree = new G4PVPlacement(0, G4ThreeVector(0, 1.545 * mm, 0), logicLayerThree, "LayerThree", logicWorld, false, 0, true);

		//Full thickness: 0.1 mm.
		//Material: Copper
		//Purpose: Additional shielding or electrode layer.

		//Fourth Cover material
		G4Material* layerFourMat = nist->FindOrBuildMaterial("G4_Cu");

		//Fourth Cover volumes
		G4Box* solidLayerFour = new G4Box("solidLayerFourShape", 5.0 * mm, 0.05 * mm, 5.0 * mm);

		G4LogicalVolume* logicLayerFour = new G4LogicalVolume(solidLayerFour, layerFourMat, "logicLayerFourShape");

		G4VisAttributes* LFourVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.)); //HellBlau
		LFourVisAtt->SetForceWireframe(true);
		logicLayerFour->SetVisAttributes(LFourVisAtt);

		// After: Al top: 0.35 / 2 = 0.175 + Air gap: 0.5 + Half of Cu layer: 0.05 = Final center Y = 2.27 mm
		G4VPhysicalVolume* physLayerFour = new G4PVPlacement(0, G4ThreeVector(0, 2.27 * mm, 0), logicLayerFour, "LayerFour", logicWorld, false, 0, true);

		//Full thickness: 1.5 mm
		//Material: ABS (custom defined elsewhere).
		//Purpose: The plastic outer shell of the detector.
		//SubtractionSolid: Creates a thin-walled box structure.

		//Fifth Cover material
		
		//Acrylonitrile Butadiene Styrene (Housing)
		G4Material* ABS = new G4Material("ABS", 1.07 * g / cm3, 3);
		ABS->AddElement(elC, 15);
		ABS->AddElement(elH, 17);
		ABS->AddElement(elN, 1);

		//Fifth Cover volumes
		//G4Box* solidLayerFiveout = new G4Box("solidLayerFiveoutShape", 5 * mm, 0.75 * mm, 5 * mm); //OuterBox
		//G4Box* solidLayerFivein = new G4Box("solidLayerFiveinShape", 4.9 * mm, 0.7 * mm, 4.9 * mm);	//innerBox

		//G4SubtractionSolid* solidLayerFive = new G4SubtractionSolid("solidLayerFiveShape", solidLayerFiveout, solidLayerFivein);

		G4Box* solidLayerFive = new G4Box("solidLayerFiveShape", 5 * mm, 0.75 * mm, 5 * mm); 

		G4LogicalVolume* logicLayerFive = new G4LogicalVolume(solidLayerFive, ABS, "logicLayerFiveShape");

		G4VisAttributes* LFiveVisAtt = new G4VisAttributes(G4Colour(1., 0., 0.)); //Rot
		LFiveVisAtt->SetForceWireframe(true);
		LFiveVisAtt->SetForceAuxEdgeVisible(true);
		logicLayerFive->SetVisAttributes(LFiveVisAtt);

		//Cu top: 0.1 / 2 = 0.05 + Air gap: 0.5 + Half of ABS: 0.75 = Final center Y = 3.57 mm
		G4VPhysicalVolume* physLayerFive = new G4PVPlacement(0, G4ThreeVector(0, 3.57 * mm, 0), logicLayerFive, "LayerFive", logicWorld, false, 0, true);
	}
	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");

	logicPinD->SetSensitiveDetector(sensDet); 
}
	