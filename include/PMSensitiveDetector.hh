#ifndef PMSENSITIVEDETECTOR_HH
#define PMSENSITIVEDETECTOR_HH 1

#include "G4VSensitiveDetector.hh"      // particle이 이 볼륨 안을 지날 때마다 ProcessHits()를 자동으로 호출해 줌

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4OpticalPhoton.hh"


class PMSensitiveDetector : public G4VSensitiveDetector
{
public:
    PMSensitiveDetector(G4String);
    ~PMSensitiveDetector();

    void Initialize(G4HCofThisEvent* hce) override;
    void EndOfEvent(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    G4int nOpticalPhoton1 = 0; //S13
    G4int nOpticalPhoton2 = 0; //S14
    G4double totalEdep = 0.;


private:

    G4int eventID;
};

#endif

// 수정
