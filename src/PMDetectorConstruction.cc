#include "PMDetectorConstruction.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"

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
    auto PSMat = fMaterials -> GetPs();
    auto colliMat = fMaterials -> GetColli();

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
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    //  월드 물리 볼륨 (실제 배치)
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
                                    // G4PVPlacement(회전, 위치벡터, 논리볼륨, 이름, 부모물리볼륨, boolean, 복제번호, 겹침체크)

    G4Sphere *solidNa = new G4Sphere("solidNa", 0., 0.1*mm, 0., 360.*deg, 0., 180.*deg);
    G4LogicalVolume *logicNa = new G4LogicalVolume(solidNa, sodiumMat, "logicNa");
    G4VPhysicalVolume *physNa = new G4PVPlacement(0, G4ThreeVector(0., 1.06*cm, 3.*cm), logicNa, "physNa", logicWorld, false, 0, checkOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    G4Box *solidTeflon = new G4Box("solidTeflon", 0.51*cm, 0.51*cm, 6.0*cm);
    G4LogicalVolume *logicTeflon = new G4LogicalVolume(solidTeflon, teflonMat, "logicTeflon");
    G4VPhysicalVolume *physTeflon = new G4PVPlacement(0, pos, logicTeflon, "physTeflon", logicWorld, false, 0, checkOverlaps);

    G4Box *airGap = new G4Box("airGap", 0.505*cm, 0.505*cm, 6.0*cm);
    G4LogicalVolume *logicAirGap = new G4LogicalVolume(airGap, worldMat, "logicAirGap");
    G4VPhysicalVolume *physAirGap = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.*cm ), logicAirGap, "physAirGap", logicTeflon, false, 0, checkOverlaps);

    G4Box *solidCrystal = new G4Box("solidCrystal", 0.5*cm, 0.5*cm, 6.*cm);
	G4Box *PS = new G4Box("PS", 3.0*cm, 0.1*cm, 0.5*cm);

	G4Box* solidCollimatorBox =
    new G4Box("CollimatorBox",
              1.0*cm,
              1.6*cm,
              1.0*cm);   // 32 mm / 2

	G4Tubs* solidHole =
    new G4Tubs("Hole",
               0.0*mm,
               1.6*mm,
               3.5*cm,
               0.*deg,
               360.*deg);

	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(90*deg);

	G4SubtractionSolid* solidCollimator =
    new G4SubtractionSolid("Collimator",
                           solidCollimatorBox,
                           solidHole,
                           rot,
                           G4ThreeVector(0,0,0));

	G4LogicalVolume* logicCollimator =
    new G4LogicalVolume(solidCollimator,
                        colliMat,
                        "logicCollimator");

	new G4PVPlacement(0,
                  G4ThreeVector(0,2.4*cm,6*cm),
                  logicCollimator,
                  "physCollimator",
                  logicWorld,
                  false,
                  0,
                  true);


    G4Material *cryMat = LYSOMat;
    if (fCrystalType == "CsI") cryMat = CsIMat;

    logicCrystal = new G4LogicalVolume(solidCrystal, cryMat, "logicCrystal");
	logicPS = new G4LogicalVolume(PS, PSMat, "logicPS");

    G4VPhysicalVolume *physCrystal = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicCrystal, "physCrystal", logicAirGap, false, 0, checkOverlaps);
	G4VPhysicalVolume *physPS = new G4PVPlacement(0, G4ThreeVector(-2.5*cm, 0.66*cm, 6.0*cm), logicPS, "physPS", logicWorld, false, 0, checkOverlaps);

    G4Box *solidCookie = new G4Box("solidCookie", 0.5*cm, 0.5*cm, 0.01*cm);
    G4LogicalVolume *logicCookie = new G4LogicalVolume(solidCookie, GreaseMat, "logicCookie");
    G4VPhysicalVolume *physCookie = new G4PVPlacement(0, G4ThreeVector(0., 0., 12.035*cm), logicCookie, "physCookie", logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume *physCookie2 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.015*cm), logicCookie, "physCookie2", logicWorld, false, 1, checkOverlaps);

    G4Box *solidPMT = new G4Box("solidPMT", 0.3*cm, 0.3*cm, 0.1*cm);
    logicPMT = new G4LogicalVolume(solidPMT, pmtMat, "logicPMT");
    logicPMT2 = new G4LogicalVolume(solidPMT, pmtMat, "logicPMT2");
    G4VPhysicalVolume *physPMT = new G4PVPlacement(0, G4ThreeVector(0., 0., 12.145*cm), logicPMT, "physPMT", logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume *physPMT2 = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.095*cm), logicPMT2, "physPMT2", logicWorld, false, 1, checkOverlaps);
    
    G4VisAttributes *red = new G4VisAttributes(G4Color(0., 0., 0.2, 0.1));
    red -> SetForceSolid(true);
    logicCrystal -> SetVisAttributes(red);

    G4VisAttributes *blue = new G4VisAttributes(G4Color(0.4, 0.4, 0.4, 1.0));
    blue -> SetForceSolid(true);
    logicPMT -> SetVisAttributes(blue);

    G4VisAttributes *gray = new G4VisAttributes(G4Color(0.1, 0.1, 0.3, 0.1));
    gray -> SetForceSolid(true);
    logicAirGap -> SetVisAttributes(gray);

    G4VisAttributes *white = new G4VisAttributes(G4Color(1., 1., 1., 0.2));
    white -> SetForceSolid(true);
    logicTeflon -> SetVisAttributes(white);
    logicPS -> SetVisAttributes(white);
	logicCollimator -> SetVisAttributes(white);

    new G4LogicalBorderSurface("Crystal_to_AirGap", physCrystal, physAirGap, fMaterials -> GetCrystalSurf());
    new G4LogicalBorderSurface("AirGap_to_Crystal", physAirGap, physCrystal, fMaterials -> GetCrystalSurf());
    new G4LogicalBorderSurface("CrystalTeflonSurface", physAirGap, physTeflon, fMaterials -> GetTeflonSurf());
    new G4LogicalSkinSurface("PMT_SkinSurface", logicPMT, fMaterials -> GetPMTSurf());
    new G4LogicalSkinSurface("PMT_SkinSurface2", logicPMT2, fMaterials -> GetPMTSurf2());
    

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    auto caloSD = new PMSensitiveDetector("caloSD");
    G4SDManager::GetSDMpointer() -> AddNewDetector(caloSD);

    logicPMT -> SetSensitiveDetector(caloSD);
    logicPMT2 -> SetSensitiveDetector(caloSD);
    //logicCrystal -> SetSensitiveDetector(caloSD);
}
