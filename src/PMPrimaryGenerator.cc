#include "PMPrimaryGenerator.hh"
#include "G4IonTable.hh"
#include "G4GenericIon.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
/*
    auto gamma = G4ParticleTable::GetParticleTable() -> FindParticle("gamma");
    fParticleGun -> SetParticleDefinition(gamma);
    fParticleGun -> SetParticleEnergy(1.*MeV);
    fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
*/
    fMessenger = new PMPrimaryGeneratorMessenger(this);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
    delete fMessenger;
}

// ===== Messenger에서 호출할 setter 구현 =====
void PMPrimaryGenerator::SetParticleName(const G4String& name)
{
    fParticleName = name;

    auto table = G4ParticleTable::GetParticleTable();
    auto particle = table->FindParticle(fParticleName);
    if (particle) {
        fParticleGun->SetParticleDefinition(particle);
    } else {
        G4Exception("PMPrimaryGenerator::SetParticleName",
                    "InvalidParticle", JustWarning,
                    ("Unknown particle: " + name).c_str());
    }
}

void PMPrimaryGenerator::SetEnergy(G4double E)
{
    fEnergy = E;
    fParticleGun->SetParticleEnergy(fEnergy);
}

void PMPrimaryGenerator::SetUseRandomPos(G4bool use)
{
    fUseRandomPos = use;
}

void PMPrimaryGenerator::SetRadius(G4double r)
{
    fRadius = r;
}

// ===== 실제 이벤트마다 불리는 부분 =====
void PMPrimaryGenerator::GeneratePrimaries(G4Event* event)
{
    G4GenericIon::GenericIonDefinition();

    int Z = 11, A = 22;
    //if (fExtSource == "Cs137") { Z = 55; A = 137; }
    if (fExtSource == "Cs137") { Z = 38; A = 90; }//Sr-90

   /* bool makeLu = false;
    if (fEnableLuBG)
    {
        if (G4UniformRand() > fLuFrac)
            makeLu = true;
    }

    if (makeLu) { Z = 71; A = 176; }*/

    auto *ion = G4ParticleTable::GetParticleTable() -> GetIonTable() -> GetIon(Z, A, 0.0*keV);

	/*G4double thetaMax = std::atan((3.5*mm) / (32*mm));
	G4double cost = 1.0 - G4UniformRand()*(1.0 - std::cos(thetaMax));
	G4double sint = std::sqrt(1 - cost*cost);
	G4ThreeVector dir(sint*cos(CLHEP::pi), sint*sin(CLHEP::pi), cost);
    fParticleGun -> SetParticleMomentumDirection(dir);
	*/
    fParticleGun -> SetParticleDefinition(ion);
    fParticleGun -> SetParticleCharge(0.);
    fParticleGun -> SetParticleEnergy(0.0*keV);

    fParticleGun -> SetParticlePosition(G4ThreeVector(0.*cm, 3.9*cm, 6.*cm));

    fParticleGun->GeneratePrimaryVertex(event);
}
