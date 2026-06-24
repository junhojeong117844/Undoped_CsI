// Geometry(월드, 박스, 물체 배치)와 Material을 저의하는 곳
#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"       // Detector geometry를 만드는 기본 인터페이스, 반드시 상속받아 Construct() 구현해야 함

#include "G4Box.hh"                             // 직육면체 형상의 solid 정의, G4Box("name", x/2, y/2, z/2) 이런식으로 사용
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"                   // Solid(모양) + Material(물질) 결합한 논리적 볼륨, Detector의 핵심 단위. 
#include "G4VPhysicalVolume.hh"                 // LogicalVolume을 실제로 공간에 배치하는 물리적 볼륨
#include "G4PVPlacement.hh"                     // 하나의 논리 볼륨을 특정 위치, 회전에 따라 배치하는 가장 기본적은 placement class

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"                       // Sensitive Detector 관리를 위한 매니저 클래스
#include "G4GenericMessenger.hh"

#include "PMSensitiveDetector.hh"
#include "PMMaterials.hh"

class PMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    void SetCrystalType(const G4String& type) { fCrystalType = type; }
    const G4String& GetCrystalType() const { return fCrystalType; }
    virtual G4VPhysicalVolume *Construct();     // 반드시 구현해야 하는 함수. 여기서 World volume 및 geometry 생성

private:
    PMMaterials *fMaterials = nullptr;
    G4GenericMessenger *fMessenger = nullptr;
    G4String fCrystalType = "LYSO";
    G4LogicalVolume *logicPMT;
    G4LogicalVolume *logicPMT2;
    G4LogicalVolume *logicCrystal;
    G4LogicalVolume *logicPS;

    virtual void ConstructSDandField();         // G4VUserDetectorConstruction의 virtual 함수로, detector나 field를 등록할 때 사용
                                                // Construct(): 월드, 박스, geometry 만들기 
                                                // -> ConstructSDandField(): 그 중 일부 logical volume에 이 볼륨은 hit, energy deposition을 기록해라라고 SD를 붙임
};

#endif

/*  G4SDManager
보통 ConstructSDandField() 안에서
    auto sdManager = G4SDManager::GetSDMpointer();
    auto sd = new PMSensitiveDetector("mySD");
    sdManager -> addNewDetector(sd);
    logicDetector -> SetSensitiveDetector(sd);
이런 패턴으로 사용. 즉, 어떤 logical volume에 어떤 센서(SD)를 달 것인지를 연결해주는 역할
*/
