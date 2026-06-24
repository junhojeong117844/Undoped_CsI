#ifndef PMMATERIALS_HH
#define PMMATERIALS_HH 1

#include "G4Material.hh"

#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"

class PMMaterials
{
public:
    PMMaterials();
    ~PMMaterials();    

    void DefineMaterials();

    G4Material *GetWorldMat()       { return worldMat; }
    G4Material *GetPMTMat()         { return pmtMat; }
    G4Material *GetLYSO()           { return LYSOMat; }
    G4Material *GetCsI()            { return CsIMat; }
    G4Material *GetTeflon()         { return teflonMat; }
    G4Material *GetCookie()         { return cookieMat; }
    G4Material *GetGrease()         { return greaseMat; }
    G4Material *GetSodium()     { return sodiumMat; }
    G4Material *GetPs()     { return PSMat; }
    G4Material *GetColli()     { return colliMat; }
    G4OpticalSurface *GetTeflonSurf() { return teflonSurf; }
    G4OpticalSurface *GetCrystalSurf() { return crystalSurf;}
    G4OpticalSurface *GetPMTSurf()      { return pmtSurf; }
    G4OpticalSurface *GetPMTSurf2()      { return pmtSurf2; }

private:
    G4Element *H, *C, *O, *F;
    G4Material *worldMat, *pmtMat, *pmtMat2, *LYSOMat, *CsIMat, *teflonMat, *cookieMat, *sodiumMat, *greaseMat, *PSMat, *colliMat;

    G4MaterialPropertiesTable *mptAir, *mptPMT, *mptLYSO, *mptCsI, *mptTeflon, *mptCookie, *mptSurf, *mptSurf2, *mptGrease;

    G4OpticalSurface *teflonSurf;
    G4OpticalSurface *pmtSurf;
    G4OpticalSurface *pmtSurf2;
    G4OpticalSurface *crystalSurf;

    G4GenericMessenger *fMessenger;
    G4String fAbsorberMaterial;
};

#endif
