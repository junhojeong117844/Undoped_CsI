#ifndef PMPRIMARYGENERATORMESSENGER_HH
#define PMPRIMARYGENERATORMESSENGER_HH 1

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UImessenger.hh"

#include "globals.hh"

class PMPrimaryGenerator;

class PMPrimaryGeneratorMessenger : public G4UImessenger
{
public:
    explicit PMPrimaryGeneratorMessenger(PMPrimaryGenerator* generator);
    ~PMPrimaryGeneratorMessenger() override;

    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    PMPrimaryGenerator* fGenerator = nullptr;

    // 기존 /mygen/
    G4UIdirectory* fDir = nullptr;
    G4UIcmdWithAString* fParticleCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fEnergyCmd = nullptr;
    G4UIcmdWithABool* fUseRandomPosCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fRadiusCmd = nullptr;

    // 새 /src/
    G4UIdirectory* fSrcDir = nullptr;
    G4UIcmdWithAString* fExtCmd = nullptr;                // /src/external
    G4UIcmdWithABool*   fLuOnCmd = nullptr;               // /src/enableLu
    G4UIcmdWithADouble* fLuFracCmd = nullptr;             // /src/luFrac
    G4UIcmdWithADoubleAndUnit* fExtPosZCmd = nullptr;     // /src/extPosZ
};

#endif