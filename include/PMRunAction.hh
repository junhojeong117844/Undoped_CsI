#ifndef PMRUNACTION__HH
#define PMRUNACTION__HH 1

#include "G4UserRunAction.hh"       // run 단위 행동을 정의하는 베이스 클래스
#include "G4Run.hh"                 // 이 run에 대한 정보(이벤트 수, run ID 등)를 담은 글래스
#include "G4AnalysisManager.hh"     // 히스토그램, ntuple, 파일 출력 등을 관리하는 분석 관리 클래스
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include <chrono>

class PMRunAction : public G4UserRunAction
{
public:
    PMRunAction();
    ~PMRunAction();

    void SetOutName(const G4String &n) { fOutName = n; }

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);

private:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;

    G4GenericMessenger *fMessenger = nullptr;
    G4String fOutName = "output";
};

#endif