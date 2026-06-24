#include "PMRunAction.hh"
#include "PMPrimaryGenerator.hh"

PMRunAction::PMRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    fMessenger = new G4GenericMessenger(this, "/out/", "Output control");
    fMessenger -> DeclareProperty("name", fOutName, "Output file prefix (without .root)");

   // analysisManager -> CreateH1("TotalEdep", "Total Energy deposit", 10000, 0., 10. * MeV);
    analysisManager -> CreateH1("S13 NPE", "Number of Optical Photons", 400, 0., 200.);
    analysisManager -> CreateH1("S14 NPE", "Number of Optical Photons", 400, 0., 200.);
   // analysisManager -> CreateH2("Lu", "Lutetuim", 5000, 0., 10.*MeV, 5000, 0., 5000.);
   // analysisManager -> CreateH2("Na", "Sodium", 5000, 0., 10.*MeV, 5000, 0., 5000.);
   // analysisManager -> CreateH2("Cs", "Cesium", 5000, 0., 10.*MeV, 5000, 0., 5000.);
    
    analysisManager -> CreateNtuple("Gamma", "Gamma in Crystal");
    analysisManager -> CreateNtupleIColumn("iEvent");
    analysisManager -> CreateNtupleDColumn("fEdep");
    analysisManager -> CreateNtupleDColumn("fX");
    analysisManager -> CreateNtupleDColumn("fY");
    analysisManager -> CreateNtupleDColumn("fZ");
    analysisManager -> FinishNtuple(0);
    
    analysisManager -> CreateNtuple("PhotoElectrons", "PhotoElectrons");
    analysisManager -> CreateNtupleIColumn("iEvent");
    analysisManager -> CreateNtupleIColumn("fNpe");
    analysisManager -> CreateNtupleIColumn("fNpe2"); 
    analysisManager -> FinishNtuple(1);
}

PMRunAction::~PMRunAction()
{}

void PMRunAction::BeginOfRunAction(const G4Run *run)
{
    long seed = time(NULL) + run -> GetRunID();
    CLHEP::HepRandom::setTheSeed(seed);

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int runID = run -> GetRunID();

    std::stringstream ss;
    ss << fOutName << "_run" << run -> GetRunID() << ".root";
    analysisManager -> OpenFile(ss.str());

    start_time = std::chrono::steady_clock::now();
}

void PMRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager -> Write();

    analysisManager -> CloseFile();

    G4int runID = run -> GetRunID();

    G4cout << "FInishing run" << runID << G4endl;

    end_time = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time-start_time);

    long long run_time = total_time.count();
    
    G4int hour = run_time/3600;
    G4int minute = (run_time - hour*3600)/60;
    G4int second = run_time % 60;

    G4cout << hour << "h " << minute << "m " << second << "s " << G4endl;

}
