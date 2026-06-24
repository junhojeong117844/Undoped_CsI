#include <iostream>

#include "G4RunManager.hh"      // Single-threaded run manager
#include "G4MTRunManager.hh"    // Multi-threaded run manager
#include "G4UImanager.hh"       // UI manager (macro 실행, 명령어 처리하는 Geant4의 UI 관리자)
#include "G4VisManager.hh"      // Visualization manager
#include "G4VisExecutive.hh"    // Visualization executive (시각화 설정 및 초기화)
#include "G4UIExecutive.hh"     // interactive 모드에서 UI 세션 관리 (Qt, tcsh, etc)

#include "PMPhysicsList.hh"
#include "PMDetectorConstruction.hh"
#include "PMActionInitialization.hh"

int main(int argc, char** argv)
{
    G4UIExecutive *ui = nullptr;

    #ifdef G4MULTITHREAD
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Physics List
    runManager -> SetUserInitialization(new PMPhysicsList());

    // Detector Construction
    runManager -> SetUserInitialization(new PMDetectorConstruction());

    // Action initialization
    runManager -> SetUserInitialization(new PMActionInitialization());

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);

        G4VisManager *visManager = new G4VisExecutive();
        visManager -> Initialize();

        UImanager -> ApplyCommand("/control/execute vis.mac");
        UImanager -> ApplyCommand("/process/had/rdm/thresholdForVeryLongDecayTime 1.0e+60 year");
        ui -> SessionStart();

        delete ui;
        delete visManager;
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager -> ApplyCommand(command + fileName);
    }

    delete runManager;

    return 0;
}