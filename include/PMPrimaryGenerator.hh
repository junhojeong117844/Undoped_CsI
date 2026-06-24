// 시뮬레이션이 입자를 발생시키는 방식을 정의
// 어떤 입자를, 어느 위치에서, 어느 방향으로, 몇 개 발사시킬지 결정

#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH 1

#include "PMPrimaryGeneratorMessenger.hh"

#include "G4VUserPrimaryGeneratorAction.hh"     //Primary particle 생성 클래스의 인터페이스, GeneratePrimaries() 함수를 반드시 구현해야 함.
#include "G4ParticleDefinition.hh"              // 입자 정의 클래스
#include "G4ParticleGun.hh"                     // 단순한 1-입자 발생기
#include "G4ParticleTable.hh"                   // Geant4에서 정의된 모든 particle을 검색하는 테이블. FindParticle("gamma") 같은 함수 제공
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

#include <cmath>
#include "Randomize.hh"

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);      // 입자 발생을 실제로 수행하는 함수. 반드시 구현해야 함.

    void SetParticleName(const G4String &name);
    void SetEnergy(G4double E);
    void SetUseRandomPos(G4bool use);
    void SetRadius(G4double r);

    void SetExternalSource(const G4String& src) { fExtSource = src; }
    void SetEnableLuBG(G4bool on) { fEnableLuBG = on; }
    void SetLuFraction(G4double f) { fLuFrac = f; }
    void SetExtSourcePosZ(G4double z) { fExtPosZ = z; }

private:
    G4ParticleGun *fParticleGun;                    // ParticleGun은 단일 입자를 쏘는 간단한 소스 (energy, position, particle type, direction 설정)
    PMPrimaryGeneratorMessenger *fMessenger;

    G4String fExtSource = "Na22";
    G4bool fEnableLuBG = true;
    G4double fLuFrac = 37.0/48.4;

    G4double fExtPosZ = -0.1*mm;
    G4double fLuZmin = 0.1*cm;
    G4double fLuZmax = 12.0*cm;

    G4String fParticleName;
    G4double fEnergy;
    G4bool fUseRandomPos;
    G4double fRadius;
};

#endif

/*
PrimaryGenerator에서 쏜 입자들이 
 -> Detector volume을 지나가며
 -> PhysicsList에서 정의한 process(ionization 등)로 에너지를 떨굼
 -> 그 에너지 정보가 SensitiveDetector의 ProcessHits에서 잡히게 됨. 
*/