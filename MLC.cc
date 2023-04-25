//brief Main program of MLC simulation

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MLCActionInitialization.hh"
#include "MLCDetectorConstruction.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"
#include "time.h"
#include "G4StepLimiterPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
    // detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    G4bool SetId = false;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    if (argc==3)
    {
        SetId = true;
    }
    
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine()); 
    G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);
    
    /*long* fEventSeeds = new long[2];
    fEventSeeds[0]=1923059987;
    fEventSeeds[1]=355212931;
    CLHEP::HepRandom::setTheSeeds(fEventSeeds,171);*/
    
    auto runManager = G4RunManagerFactory::CreateRunManager();

    MLCDetectorConstruction* det = new MLCDetectorConstruction();
    runManager->SetUserInitialization(det);

    G4VModularPhysicsList* physicsList = new FTFP_BERT(0);
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4(0));

    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    auto opticalParams = G4OpticalParameters::Instance();

    opticalParams->SetWLSTimeProfile("delta");
    opticalParams->SetScintTrackSecondariesFirst(true);    

    opticalParams->SetCerenkovMaxPhotonsPerStep(100);
    opticalParams->SetCerenkovMaxBetaChange(10.0);
    opticalParams->SetCerenkovTrackSecondariesFirst(true);

    physicsList->RegisterPhysics(opticalPhysics);
    G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
    physicsList->RegisterPhysics(stepLimitPhys);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new MLCActionInitialization(det));
    runManager->SetNumberOfThreads(1);

    // initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui)
    {
        // interactive mode
        //UImanager->ApplyCommand("/control/execute vis.mac");
        if (ui->IsGUI())
        {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }
    else if (SetId == true)
    {
        //set id in filename
        G4String command = "/control/execute ";
        G4String fileId = argv[2];
        UImanager->ApplyCommand("/MLC/detector/fileId "+fileId);
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // job termination
    delete visManager;
    delete runManager;
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
