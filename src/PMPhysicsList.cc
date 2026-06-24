#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
    RegisterPhysics(new G4OpticalPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PMPhysicsList::~PMPhysicsList()
{}
