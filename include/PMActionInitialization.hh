// Geant4에서 유저 액션(Primary, Run, Event, Tracking 등)을 등록하는 클래스
// DetectorConstruction, PhysicsList, PrimaryGenerator들을 RunManager에 연결해줌

#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH 1

#include "G4VUserActionInitialization.hh"   // Geant4에서 사용자 액션(PrimaryGenerator, RunAction, EventAction 등)을 등록하는 "핵심 클래스"
                                            // Build(), BuildForMaster()를 구현할 때 반드시 필요

#include "PMPrimaryGenerator.hh"            // PrimaryGenerator를 생성하고 등록하려면 필요
                                            // ActionInitialization에서 SetUserAction 호출할 때 필요
#include "PMRunAction.hh"
#include "PMSteppingAction.hh"

// Geant4는 유지거 직접 정의한 액션(class)을 실행 전에 등록해야 함. 
// ActionInitialization은 그 등록을 맡는 클래스. 
class PMActionInitialization : public G4VUserActionInitialization
{
public:
    PMActionInitialization();
    ~PMActionInitialization();

    virtual void BuildForMaster() const;    // 멀티스레드 사용 시 Master thread 전용 액션 등록
    virtual void Build() const;             // Worker thread에 PrimaryGenerator 및 기타 액션 등록
};
// 지금은 PrimaryGenerator만 등록하는 최소 구조지만 나중에 RunAction, EventAction, SteppingAction 등을 더 넣을 수 있음. 

#endif

