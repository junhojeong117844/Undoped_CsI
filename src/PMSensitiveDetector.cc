#include "PMSensitiveDetector.hh"
#include "PMDetectorConstruction.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    //fTotalEnergyDeposited = 0.;
    //fScintEnergyDeposited = 0.;
    //fCherenEnergyDeposited = 0.;
}

PMSensitiveDetector::~PMSensitiveDetector()
{}

void PMSensitiveDetector::Initialize(G4HCofThisEvent *)
{
    totalEdep = 0.;
    nOpticalPhoton1 = 0;
	nOpticalPhoton2 = 0;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4Track *track = aStep -> GetTrack();
	if (track->GetDefinition() != G4OpticalPhoton::Definition())
		return true;
    const G4VProcess *creator = track -> GetCreatorProcess();

    // PreStepPoint
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();
    G4LogicalVolume *volume = preStepPoint -> GetTouchableHandle() -> GetVolume() -> GetLogicalVolume();
    G4String lvName = volume -> GetName();
    G4int copyNo = preStepPoint -> GetTouchableHandle() -> GetVolume() -> GetCopyNo();
    if (copyNo == 0)
	{
		nOpticalPhoton1++;
	}
	else if (copyNo == 1)
	{
		nOpticalPhoton2++;
	}

	G4double fEnergyDeposited = aStep -> GetTotalEnergyDeposit();
    G4ThreeVector momPhoton = preStepPoint -> GetMomentum();

    // PostStepPoint
    G4StepPoint *postStepPoint = aStep -> GetPostStepPoint();

    G4double fMomPhotonMag = momPhoton.mag();
    G4double E = preStepPoint -> GetKineticEnergy();

    G4double fWlen = 1239.841939 / (E/eV);

	G4cout
<< volume->GetName()
<< " copyNo = "
<< copyNo
<< G4endl;

    if(fEnergyDeposited > 0 )
    {
        totalEdep += fEnergyDeposited;
    /*
        analysisManager -> FillNtupleIColumn(0, 0, eventID);
        analysisManager -> FillNtupleDColumn(0, 1, fEnergyDeposited);
        analysisManager -> AddNtupleRow(0);
    */
    }


    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
}
