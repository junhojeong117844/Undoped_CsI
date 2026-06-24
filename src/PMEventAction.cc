// PMEventAction.cc
#include "PMEventAction.hh"
#include "G4AnalysisManager.hh"
#include "PMSensitiveDetector.hh"
#include "G4Event.hh"

void PMEventAction::EndOfEventAction(const G4Event* evt)
{
  int Z = -1, A = -1;
  G4String RadName = "Unknown";

  auto* ana = G4AnalysisManager::Instance();

  // primary ion(Z,A) 얻기
  if (auto* vtx = evt->GetPrimaryVertex(0)) {
    if (auto* pp = vtx->GetPrimary(0)) {
      if (auto* pd = pp->GetG4code()) {
        Z = pd->GetAtomicNumber();
        A = pd->GetAtomicMass();
      }
    }
  }

  // 분류 + 히스토 채우기
/*  if (Z == 71 && A == 176) {
    RadName = "Lu176";
    ana->FillH2(0, fTotalEdep, fNpe);
  }
  else if (Z == 11 && A == 22) {
    RadName = "Na22";
    ana->FillH2(1, fTotalEdep, fNpe);
  }
  else if (Z == 55 && A == 137) {
    RadName = "Cs137";
    ana->FillH2(2, fTotalEdep, fNpe);
  }
*/
  // 출력
  if (fNpe > 0 || fNpe2 >  0) {
    ana->FillH1(0, fNpe);
	ana->FillH1(1, fNpe2);
    ana -> FillNtupleIColumn(1, 0, evt -> GetEventID());
    ana -> FillNtupleIColumn(1, 1, fNpe);
    ana -> FillNtupleIColumn(1, 2, fNpe2);
    ana -> AddNtupleRow(1);
    G4cout << "EventID: " << evt->GetEventID() << G4endl;
    //       << " S13: " << fNpe 
	//	   << " S14: " << fNpe2 << G4endl;
  }

  /*if (fTotalEdep > 0.) {
    ana->FillH1(0, fTotalEdep);
    G4cout << "EventID: " << evt->GetEventID()
           << " Total Energy Deposited: " << G4BestUnit(fTotalEdep, "Energy")
           << " Source: " << RadName;

    G4cout << G4endl;
  }*/
}
