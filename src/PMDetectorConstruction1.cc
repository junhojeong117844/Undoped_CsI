#include "PMDetectorConstruction.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"
#include "G4Sphere.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
    fMaterials = new PMMaterials();
    fMaterials -> DefineMaterials();

    fMessenger = new G4GenericMessenger(this, "/det/", "Detector control");
    fMessenger -> DeclareProperty("crystal", fCrystalType, "Crystal type: LYSO or CsI");
}

PMDetectorConstruction::~PMDetectorConstruction()
{}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    auto worldMat = fMaterials -> GetWorldMat();
    auto pmtMat = fMaterials -> GetPMTMat();
    auto LYSOMat = fMaterials -> GetLYSO();
    auto CsIMat = fMaterials -> GetCsI();
    auto teflonMat = fMaterials -> GetTeflon();
    auto cookieMat = fMaterials -> GetCookie();
    auto GreaseMat = fMaterials -> GetGrease();
    auto sodiumMat = fMaterials -> GetSodium();

    G4bool checkOverlaps = true;


    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    //  월드 크기 정의
    G4double xWorld = 0.5* m;
    G4double yWorld = 0.5* m;
    G4double zWorld = 0.5* m;

    G4ThreeVector pos = G4ThreeVector(0., 0., 6.025*cm);

    //  월드 크기 정의
    G4Box *solidWorld = new G4Box("solidWorld", xWorld/2, yWorld/2, zWorld/2);

    //  월드 논리 볼륨 (형상 + 재질)
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    //  월드 물리 볼륨 (실제 배치)
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
                                    // G4PVPlacement(회전, 위치벡터, 논리볼륨, 이름, 부모물리볼륨, boolean, 복제번호, 겹침체크)

    G4Sphere *solidNa = new G4Sphere("solidNa", 0., 0.1*mm, 0., 360.*deg, 0., 180.*deg);
    G4LogicalVolume *logicNa = new G4LogicalVolume(solidNa, sodiumMat, "logicNa");
    G4VPhysicalVolume *physNa = new G4PVPlacement(0, G4ThreeVector(0., 1.06*cm, 3.*cm), logicNa, "physNa", logicWorld, false, 0, checkOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    G4Box *solidTeflon = new G4Box("solidTeflon", 0.5010*cm, 0.5010*cm, 6.025*cm);
    G4LogicalVolume *logicTeflon = new G4LogicalVolume(solidTeflon, teflonMat, "logicTeflon");
    G4VPhysicalVolume *physTeflon = new G4PVPlacement(0, pos, logicTeflon, "physTeflon", logicWorld, false, 0, checkOverlaps);

    G4Box *airGap = new G4Box("airGap", 0.55*cm, 0.55*cm, 6.005*cm);
    G4LogicalVolume *logicAirGap = new G4LogicalVolume(airGap, worldMat, "logicAirGap");
    G4VPhysicalVolume *physAirGap = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.02*cm ), logicAirGap, "physAirGap", logicTeflon, false, 0, checkOverlaps);

    G4Box *solidCrystal = new G4Box("solidCrystal", 0.5*cm, 0.5*cm, 6.*cm);

    G4Material *cryMat = LYSOMat;
    if (fCrystalType == "CsI") cryMat = CsIMat;

    logicCrystal = new G4LogicalVolume(solidCrystal, cryMat, "logicCrystal");

    G4VPhysicalVolume *physCrystal = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.005*cm), logicCrystal, "physCrystal", logicAirGap, false, 0, checkOverlaps);

    G4Box *solidCookie = new G4Box("solidCookie", 0.5*cm, 0.5*cm, 0.005*cm);
    G4LogicalVolume *logicCookie = new G4LogicalVolume(solidCookie, GreaseMat, "logicCookie");
    G4VPhysicalVolume *physCookie = new G4PVPlacement(0, G4ThreeVector(0., 0., 12.055*cm), logicCookie, "physCookie", logicWorld, false, 0, checkOverlaps);

    G4Box *solidPMT = new G4Box("solidPMT", 0.3*cm, 0.3*cm, 0.1*cm);
    logicPMT = new G4LogicalVolume(solidPMT, pmtMat, "logicPMT");
    G4VPhysicalVolume *physPMT = new G4PVPlacement(0, G4ThreeVector(0., 0., 12.160*cm), logicPMT, "physPMT", logicWorld, false, 0, checkOverlaps);
    
    G4VisAttributes *red = new G4VisAttributes(G4Color(1., 0., 0., 0.3));
    red -> SetForceSolid(true);
    logicCrystal -> SetVisAttributes(red);

    G4VisAttributes *blue = new G4VisAttributes(G4Color(0., 0., 1., 0.3));
    blue -> SetForceSolid(true);
    logicPMT -> SetVisAttributes(blue);

    G4VisAttributes *gray = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 0.3));
    gray -> SetForceSolid(true);
    logicAirGap -> SetVisAttributes(gray);

    G4VisAttributes *white = new G4VisAttributes(G4Color(1., 1., 1., 0.9));
    white -> SetForceSolid(true);
    logicTeflon -> SetVisAttributes(white);

    new G4LogicalBorderSurface("Crystal_to_AirGap", physCrystal, physAirGap, fMaterials -> GetCrystalSurf());
    new G4LogicalBorderSurface("AirGap_to_Crystal", physAirGap, physCrystal, fMaterials -> GetCrystalSurf());
    new G4LogicalBorderSurface("CrystalTeflonSurface", physAirGap, physTeflon, fMaterials -> GetTeflonSurf());
    new G4LogicalSkinSurface("PMT_SkinSurface", logicPMT, fMaterials -> GetPMTSurf());
    

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    auto caloSD = new PMSensitiveDetector("caloSD");
    G4SDManager::GetSDMpointer() -> AddNewDetector(caloSD);

    logicPMT -> SetSensitiveDetector(caloSD);
    logicCrystal -> SetSensitiveDetector(caloSD);
}
