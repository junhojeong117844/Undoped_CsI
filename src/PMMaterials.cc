#include "PMMaterials.hh"

PMMaterials::PMMaterials()
{
    fMessenger = new G4GenericMessenger(this, "/material/", "Absorber Material");
    fAbsorberMaterial = "G4_Pb";

    fMessenger -> DeclareProperty("absorber", fAbsorberMaterial, "Absorber material (Pb, W, Cu)");
}

PMMaterials::~PMMaterials()
{}

void PMMaterials::DefineMaterials()
{       
    //  재질 가져오기 (NIST)
    G4NistManager *nist = G4NistManager::Instance();

    H = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
    C = new G4Element("Carbon"  , "C", 6., 12.01*g/mole);
    O = new G4Element("Oxygen"  , "O", 8., 16.00*g/mole);
    F = new G4Element("Flourine", "F", 9., 18.9984*g/mole);

    auto* Lu = new G4Element("Lutetium", "Lu", 71., 174.9668*g/mole);
    auto* Y  = new G4Element("Yttrium" , "Y", 39., 88.9059*g/mole);
    auto* Si = nist -> FindOrBuildElement("Si");
    //auto* O = nist -> FindOrBuildElement("O");
    auto* Cs = nist -> FindOrBuildElement("Cs");
    auto* I  = nist -> FindOrBuildElement("I");
    auto* Na = nist -> FindOrBuildElement("Na");
    auto* Fe = nist -> FindOrBuildElement("Fe");

    auto densityLYSO = 7.25*g/cm3;
    auto densityCsI = 4.51*g/cm3;

	colliMat = new G4Material("colliMat", 7.874*g/cm3, 1);
	colliMat -> AddElement(Fe, 1);

    cookieMat = new G4Material("OpticalCookie", 1.2*g/cm3, 1);
    cookieMat -> AddElement(Si, 1);

	greaseMat = new G4Material("OpticalGrease", 1.06*g/cm3, 4);
	greaseMat -> AddElement(H, 8);
	greaseMat -> AddElement(C, 2);
	greaseMat -> AddElement(O, 1);
	greaseMat -> AddElement(Si, 1);

    sodiumMat = new G4Material("Sodium", 0.97*g/cm3, 1);
    sodiumMat -> AddElement(Na, 1);

    LYSOMat = new G4Material("LYSO", densityLYSO, 4);
    LYSOMat -> AddElement(Lu, (313.2/439.));
    LYSOMat -> AddElement(Y, (17.8/439.));
    LYSOMat -> AddElement(Si, (28./439.));
    LYSOMat -> AddElement(O, (80./439.));

    CsIMat = new G4Material("CsI", densityCsI, 2);
    CsIMat -> AddElement(Cs, 1);
    CsIMat -> AddElement(I, 1);

	PSMat = new G4Material("PS", 1.032*g/cm3, 2);
	PSMat -> AddElement(C, 9);
	PSMat -> AddElement(H, 10);

    // World
    worldMat = nist -> FindOrBuildMaterial("G4_AIR");
    pmtMat = nist -> FindOrBuildMaterial("G4_AIR");
    pmtMat2 = nist -> FindOrBuildMaterial("G4_AIR");
    teflonMat = nist -> FindOrBuildMaterial("G4_TEFLON");
    

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // 물성 정의
    // Refractive Index
    const G4int nEntries = 22;

    G4double photonEnergy[nEntries] = 
    {
        1.80*eV, 1.85*eV, 1.91*eV, 1.97*eV, 2.03*eV,
        2.10*eV, 2.17*eV, 2.25*eV, 2.34*eV, 2.43*eV,
        2.53*eV, 2.64*eV, 2.75*eV, 2.88*eV, 3.02*eV,
        3.18*eV, 3.35*eV, 3.54*eV, 3.76*eV, 4.00*eV,
        4.28*eV, 4.59*eV
    };


    // Scintillation Options
    G4double lysoRIndex[nEntries] = {
        1.82, 1.82, 1.82, 1.82, 1.82,
        1.82, 1.82, 1.82, 1.82, 1.82,
        1.82, 1.82, 1.82, 1.82, 1.82,
        1.82, 1.82, 1.82, 1.82, 1.82,
        1.82, 1.82
    };

    G4double lysoAbsLength[nEntries] = 
    {
        // default abs length (From A High Count-Rate and Depth-of-Interaction Resolving Single Layered One-Side Readout Pixelated Scintillator Crystal Array for PET Applications)
        /*
        3724.5*mm, 3509.5*mm, 3452.1*mm, 3233.5*mm, 3180.5*mm,
        2890.4*mm, 2662.8*mm, 2389.4*mm, 2318.2*mm, 2061.8*mm, 
        1799.9*mm, 1546.9*mm, 1078.9*mm, 722.84*mm, 399.70*mm,
        36.343*mm, 3.9308*mm, 3.5459*mm, 3.4924*mm, 3.4219*mm,
        3.1918*mm, 3.0848*mm   
        */
        
        // abs length ver2 (From Ce:LYSO, from scintillator to solid-state lighting as a blue luminescent concentrator)
        
        10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm,
        10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm,
        10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm,
        14.620*mm, 0.7052*mm, 0.3273*mm, 0.3796*mm, 0.1775*mm,
        0.1270*mm, 0.1270*mm
        
        // abs length ver3 (From Opticalsimulation of monolithic scintillator detector using GATE/Geant4 #5)
        /*
        10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm,
        10000.*mm, 10000.*mm, 10000.*mm, 10000.*mm, 12.658*mm,
        11.937*mm, 12.344*mm, 12.633*mm, 12.466*mm, 12.408*mm,
        10.751*mm, 2.7262*mm, 1.5215*mm, 2.2928*mm, 1.0435*mm, 
        0.6139*mm, 0.4939*mm
        */

        // abs length ver4 (Transmittance -> Abs length conversion)
        /*
        30000.*cm, 30000.*cm, 30000.*cm, 30000.*cm, 30000.*cm,
        29204.*cm, 6145.4*mm, 3101.7*mm, 1382.6*mm, 887.58*mm,
        589.85*mm, 361.71*mm, 233.96*mm, 153.45*mm, 94.466*mm,
        26.617*mm, 2.2285*mm, 2.2285*mm, 4.0688*mm, 2.2285*mm,
        2.2285*mm, 2.2285*mm
        */
    };


    G4double lysoFast[nEntries] = {
        0.00, 0.00, 0.01, 0.01, 0.02,
        0.03, 0.03, 0.04, 0.06, 0.09,
        0.13, 0.28, 0.56, 0.88, 1.00, 
        0.63, 0.01, 0.00, 0.00, 0.00,
        0.00, 0.00
    };
    
    G4double csiRIndex[nEntries] = {
        1.7310, 1.7315, 1.7330, 1.7345, 1.7360,
        1.7380, 1.7395, 1.7420, 1.7460, 1.7500,
        1.7545, 1.7615, 1.7700, 1.7790, 1.7890,
        1.8000, 1.8140, 1.8335, 1.8560, 1.8845,
        1.9285, 1.9990
    };

    G4double csiAbsLength[nEntries] = {
        1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm,
        1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm,
        1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm,
        1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm, 1000.*mm,
        1000.*mm, 1000.*mm
    };

    G4double csiFast[nEntries] = {
        0.00, 0.00, 0.00, 0.00, 0.00,
        0.03, 0.05, 0.07, 0.08, 0.09, 
        0.09, 0.09, 0.09, 0.08, 0.08,
        0.08, 0.13, 0.24, 0.61, 0.96,
        0.75, 0.14
    };

    G4double airRIndex[nEntries] = {
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0
    };

    G4double airAbsLength[nEntries] = {
        10.*m, 10.*m, 10.*m, 10.*m, 10.*m,
        10.*m, 10.*m, 10.*m, 10.*m, 10.*m,
        10.*m, 10.*m, 10.*m, 10.*m, 10.*m,
        10.*m, 10.*m, 10.*m, 10.*m, 10.*m,
        10.*m, 10.*m
    };

    G4double QE2[nEntries] = {
        18.00, 21.02, 23.27, 25.96, 28.19,
        31.78, 34.33, 36.31, 40.23, 44.61,
        48.08, 50.00, 51.42, 50.51, 48.00,
        44.50, 40.50, 38.56, 36.90, 30.00,
        19.00, 00.60
    }; //S14

    for (int i = 0; i < nEntries; i++)
        QE2[i] = QE2[i] / 100.;

	G4double QE[nEntries] = {
        18.00, 21.02, 23.27, 25.96, 28.19,     
        31.78, 34.33, 36.31, 40.23, 44.61,
	    48.08, 50.00, 51.42, 50.51, 48.00,
		44.50, 40.50, 38.56, 20.90, 10.00,
        00.50, 00.00
    }; //S13

    for (int i = 0; i < nEntries; i++)
        QE[i] = QE[i] / 100.;


    G4double pmtRefl[nEntries] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0
    };

    G4double refl[nEntries];
    G4double eff[nEntries];

    for (int i = 0; i < nEntries; i++)
    {
        refl[i] = 0.92;         // default: 0.95
        eff[i] = 0.0;
    }

    G4double cookieRindex[nEntries];
    G4double cookieAbs[nEntries];

    for (int i = 0; i < nEntries; i++)
    {
        //cookieRindex[i] = 1.403;
        cookieRindex[i] = 1.43;
        cookieAbs[i] = 10000.0*mm;
    }

    G4double GreaseRindex[nEntries];
    G4double GreaseAbs[nEntries];

    for (int i = 0; i < nEntries; i++)
    {
        GreaseRindex[i] = 1.465;
        GreaseAbs[i] = 10000.0*mm;
    }


    mptCookie = new G4MaterialPropertiesTable();
    mptCookie -> AddProperty("RINDEX", photonEnergy, cookieRindex, nEntries);
    mptCookie -> AddProperty("ABSLENGTH", photonEnergy, cookieAbs, nEntries);
    cookieMat -> SetMaterialPropertiesTable(mptCookie);

	mptGrease = new G4MaterialPropertiesTable();
    mptGrease -> AddProperty("RINDEX", photonEnergy, GreaseRindex, nEntries);
    mptGrease -> AddProperty("ABSLENGTH", photonEnergy, GreaseAbs, nEntries);
    greaseMat -> SetMaterialPropertiesTable(mptGrease);


    crystalSurf = new G4OpticalSurface("CrystalSurf");
    crystalSurf -> SetModel(unified);
    crystalSurf -> SetType(dielectric_dielectric);
    crystalSurf -> SetFinish(polished);

    teflonSurf = new G4OpticalSurface("TeflonWrapSurf");
    teflonSurf -> SetModel(unified);
    teflonSurf -> SetType(dielectric_dielectric);
    teflonSurf -> SetFinish(ground);                         
    //teflonSurf -> SetSigmaAlpha(0.9);

    pmtSurf = new G4OpticalSurface("PMTSurf");
    pmtSurf -> SetModel(unified);
    pmtSurf -> SetType(dielectric_metal);
    pmtSurf -> SetFinish(polished);

	pmtSurf2 = new G4OpticalSurface("PMTSurf2");
    pmtSurf2 -> SetModel(unified);
    pmtSurf2 -> SetType(dielectric_metal);
    pmtSurf2 -> SetFinish(polished);


    mptTeflon = new G4MaterialPropertiesTable();
    mptTeflon -> AddProperty("REFLECTIVITY", photonEnergy, refl, nEntries);
    mptTeflon -> AddProperty("EFFICIENCY", photonEnergy, eff, nEntries);
    teflonSurf -> SetMaterialPropertiesTable(mptTeflon);

    mptSurf = new G4MaterialPropertiesTable();
    mptSurf -> AddProperty("EFFICIENCY", photonEnergy, QE, nEntries);
    mptSurf -> AddProperty("REFLECTIVITY", photonEnergy, pmtRefl, nEntries);
    pmtSurf -> SetMaterialPropertiesTable(mptSurf);

	mptSurf2 = new G4MaterialPropertiesTable();
    mptSurf2 -> AddProperty("EFFICIENCY", photonEnergy, QE2, nEntries);
    mptSurf2 -> AddProperty("REFLECTIVITY", photonEnergy, pmtRefl, nEntries);
    pmtSurf2 -> SetMaterialPropertiesTable(mptSurf2);


    mptAir = new G4MaterialPropertiesTable();

    mptLYSO = new G4MaterialPropertiesTable();
    mptLYSO -> AddProperty("RINDEX", photonEnergy, lysoRIndex, nEntries);
    mptLYSO -> AddProperty("ABSLENGTH", photonEnergy, lysoAbsLength, nEntries);
    mptLYSO -> AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, lysoFast, nEntries);
    mptLYSO -> AddConstProperty("SCINTILLATIONYIELD", 28500./MeV);                         
    mptLYSO -> AddConstProperty("RESOLUTIONSCALE", 10.0);
    mptLYSO -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 40.*ns);

    mptCsI = new G4MaterialPropertiesTable();
    mptCsI -> AddProperty("RINDEX", photonEnergy, csiRIndex, nEntries);
    mptCsI -> AddProperty("ABSLENGTH", photonEnergy, csiAbsLength, nEntries);
    mptCsI -> AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, csiFast, nEntries);
    mptCsI -> AddConstProperty("SCINTILLATIONYIELD", 2100./MeV);
    mptCsI -> AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptCsI -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 16.*ns);

    mptAir = new G4MaterialPropertiesTable();
    mptAir -> AddProperty("RINDEX", photonEnergy, airRIndex, nEntries);
    mptAir -> AddProperty("ABSLENGTH", photonEnergy, airAbsLength, nEntries);

    LYSOMat -> SetMaterialPropertiesTable(mptLYSO);
    //LYSOMat -> GetIonisation()->SetBirksConstant(0.02*mm/MeV);

    CsIMat -> SetMaterialPropertiesTable(mptCsI);
    //CsIMat -> GetIonisation()->SetBirksConstant(0.003*mm/MeV);
    pmtMat -> SetMaterialPropertiesTable(mptAir);
    pmtMat2 -> SetMaterialPropertiesTable(mptAir);
    worldMat -> SetMaterialPropertiesTable(mptAir);
}
