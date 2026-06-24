#include "PMSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"

#include "G4SystemOfUnits.hh"

PMSteppingAction::PMSteppingAction(PMEventAction* ea)
: G4UserSteppingAction(), fEventAction(ea)
{}

void PMSteppingAction::FindBoundaryProcess()
{
    if (fBoundary) return;

    // Optical photon의 process manager에서 G4OpBoundaryProcess 찾기
    auto* pm = G4OpticalPhoton::OpticalPhotonDefinition()->GetProcessManager();
    if (!pm) return;

    auto* pv = pm->GetProcessList();
    if (!pv) return;

    for (G4int i = 0; i < pv->size(); i++) {
        auto* p = (*pv)[i];
        if (auto* b = dynamic_cast<G4OpBoundaryProcess*>(p)) {
            fBoundary = b;
            break;
        }
    }
}

void PMSteppingAction::UserSteppingAction(const G4Step* step)
{
    auto* ana = G4AnalysisManager::Instance();

    G4double fEdep = step->GetTotalEnergyDeposit();
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint* post = step->GetPostStepPoint();
    G4ThreeVector Pos = post -> GetPosition();

    G4LogicalVolume *volume = preStepPoint -> GetTouchableHandle() -> GetVolume() -> GetLogicalVolume();
    G4String lvName = volume -> GetName();
    auto* track = step->GetTrack();
    if (!track) return;
    
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    if (fEdep > 0. && lvName == "logicCrystal")
    {
        fEventAction -> AddEdep(fEdep);
        //G4String pd = track -> GetParticleDefinition() -> GetParticleName();
        auto *pd = track -> GetParticleDefinition();

        if (pd -> GetPDGCharge() != 0.0 && pd != G4OpticalPhoton::Definition())
        {
            ana -> FillNtupleIColumn(0, 0, eventID);
            ana -> FillNtupleDColumn(0, 1, fEdep);
            ana -> FillNtupleDColumn(0, 2, Pos.x()/cm);
            ana -> FillNtupleDColumn(0, 3, Pos.y()/cm);
            ana -> FillNtupleDColumn(0, 4, Pos.z()/cm);
            ana -> AddNtupleRow(0);           
        }
    }

    // optical photon만
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return;

    // boundary에서만 의미 있음
    if (post->GetStepStatus() != fGeomBoundary) return;

    // boundary process 찾기(1회 캐시)
    FindBoundaryProcess();
    if (!fBoundary) return;

    // Surface에서 검출되었는지 확인
    if (fBoundary->GetStatus() == Detection)
    {
        // 광자 에너지/파장
        G4double E = post->GetKineticEnergy();
        G4double wlen_nm = 1239.841939 / (E/eV);

        G4VPhysicalVolume* postPV = post->GetPhysicalVolume();
        if (!postPV) return;

        G4LogicalVolume* postLV = postPV->GetLogicalVolume();
        if (!postLV) return;

        G4String postName = postLV->GetName();

		        //G4cout << "Detection at post volume = " << postName << G4endl;

        if (fEventAction)
        {
            if (postName == "logicPMT")
            {
                fEventAction -> AddPE();      // S13
            }
            else if (postName == "logicPMT2")
            {
                fEventAction -> AddPE2();     // S14
            }
        }

        // 검출된 광자는 종료
        track->SetTrackStatus(fStopAndKill);
    }

    // Surface에서 검출되었는지 확인
    /*if (fBoundary->GetStatus() == Detection)
    {
        // 광자 에너지/파장
        G4double E = post->GetKineticEnergy();         // (pre나 post나 동일해도 OK)
        G4double wlen_nm = 1239.841939 / (E/eV);       // nm

        if (fEventAction) fEventAction ->AddPE();
        // (선택) 검출된 광자는 더 진행할 필요 없으면 kill
        track->SetTrackStatus(fStopAndKill);
    }*/
}

