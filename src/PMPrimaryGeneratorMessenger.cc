#include "PMPrimaryGeneratorMessenger.hh"
#include "PMPrimaryGenerator.hh"


PMPrimaryGeneratorMessenger::PMPrimaryGeneratorMessenger(PMPrimaryGenerator* generator)
: fGenerator(generator)
{
    fDir = new G4UIdirectory("/mygen/");
    fDir->SetGuidance("Primary generator control");

    fSrcDir = new G4UIdirectory("/src/");
    fSrcDir->SetGuidance("Radioactive source control");

    fExtCmd = new G4UIcmdWithAString("/src/external", this);
    fExtCmd->SetGuidance("External source: Na22 or Cs137");
    fExtCmd->SetParameterName("src", false);

    fLuOnCmd = new G4UIcmdWithABool("/src/enableLu", this);
    fLuOnCmd->SetGuidance("Enable Lu-176 internal background (true/false)");
    fLuOnCmd->SetParameterName("on", false);

    fLuFracCmd = new G4UIcmdWithADouble("/src/luFrac", this);
    fLuFracCmd->SetGuidance("Lu fraction in [0,1] when enableLu=true");
    fLuFracCmd->SetParameterName("frac", false);
    fLuFracCmd->SetRange("frac>=0.0 && frac<=1.0");

    fExtPosZCmd = new G4UIcmdWithADoubleAndUnit("/src/extPosZ", this);
    fExtPosZCmd->SetGuidance("External source Z position");
    fExtPosZCmd->SetUnitCategory("Length");
    fExtPosZCmd->SetParameterName("z", false);

    // /mygen/particle
    fParticleCmd = new G4UIcmdWithAString("/mygen/particle", this);
    fParticleCmd->SetGuidance("Set primary particle name");
    fParticleCmd->SetParameterName("particleName", false);

    // /mygen/energy
    fEnergyCmd = new G4UIcmdWithADoubleAndUnit("/mygen/energy", this);
    fEnergyCmd->SetGuidance("Set primary energy");
    fEnergyCmd->SetParameterName("energy", false);
    fEnergyCmd->SetUnitCategory("Energy");
    fEnergyCmd->SetRange("energy>0.");

    // /mygen/useRandomPos
    fUseRandomPosCmd = new G4UIcmdWithABool("/mygen/useRandomPos", this);
    fUseRandomPosCmd->SetGuidance("Toggle random position within radius disk");
    fUseRandomPosCmd->SetParameterName("useRandomPos", false);

    // /mygen/radius
    fRadiusCmd = new G4UIcmdWithADoubleAndUnit("/mygen/radius", this);
    fRadiusCmd->SetGuidance("Set radius of beam spot");
    fRadiusCmd->SetParameterName("radius", false);
    fRadiusCmd->SetUnitCategory("Length");
    fRadiusCmd->SetRange("radius>=0.");
}

PMPrimaryGeneratorMessenger::~PMPrimaryGeneratorMessenger()
{
    delete fParticleCmd;
    delete fEnergyCmd;
    delete fUseRandomPosCmd;
    delete fRadiusCmd;
    delete fDir;
    delete fExtCmd;
    delete fLuOnCmd;
    delete fLuFracCmd;
    delete fExtPosZCmd;
    delete fSrcDir;
}

void PMPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fParticleCmd) {
        fGenerator->SetParticleName(newValue);
    }
    else if (command == fEnergyCmd) {
        fGenerator->SetEnergy(fEnergyCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fUseRandomPosCmd) {
        fGenerator->SetUseRandomPos(fUseRandomPosCmd->GetNewBoolValue(newValue));
    }
    else if (command == fRadiusCmd) {
        fGenerator->SetRadius(fRadiusCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fExtCmd) {
        fGenerator->SetExternalSource(newValue);
    }
    else if (command == fLuOnCmd) {
        fGenerator->SetEnableLuBG(fLuOnCmd->GetNewBoolValue(newValue));
    }
    else if (command == fLuFracCmd) {
        fGenerator->SetLuFraction(fLuFracCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fExtPosZCmd) {
        fGenerator->SetExtSourcePosZ(fExtPosZCmd->GetNewDoubleValue(newValue));
    }
}