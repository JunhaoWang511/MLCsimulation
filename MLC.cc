// brief Main program of MLC simulation

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

int main(int argc, char **argv)
{
    G4cout << "you can give no more than 4 parameters:" << G4endl << "[0] is ./MLC, [1] is .gdml filename, [2] is .mac filename, [3] is file ID." << G4endl;
    // detect interactive mode (if no arguments) and define UI session
    G4UIExecutive *ui = nullptr;
    G4bool SetId = false;
    G4bool UseGDML = false;
    G4String GDMLFileName;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    else if (argc == 2)
    {
        ui = new G4UIExecutive(argc, argv);
        UseGDML = true;
    }
    else if (argc == 3)
    {
        UseGDML = true;
    }
    else if (argc == 4)
    {
        SetId = true;
        UseGDML = true;
    }
    else
    {
        G4cout << "error!: parameter number is too more." << G4endl;
    }

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    G4long seed = time(NULL);
    if (argc == 4)
    {
        seed *= std::stoi(argv[3]);
    }
    CLHEP::HepRandom::setTheSeed(seed);

    // creat run manager
    auto runManager = G4RunManagerFactory::CreateRunManager();
    // creat detector by using GDML files or default geometry
    MLCDetectorConstruction *det;
    if (UseGDML)
    {
        GDMLFileName = argv[1];
        det = new MLCDetectorConstruction(GDMLFileName);
    }
    else
    {
        det = new MLCDetectorConstruction();
    }

    runManager->SetUserInitialization(det);

    G4VModularPhysicsList *physicsList = new FTFP_BERT(0);
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4(0));

    G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
    auto opticalParams = G4OpticalParameters::Instance();

    opticalParams->SetWLSTimeProfile("delta");
    opticalParams->SetScintTrackSecondariesFirst(true);

    opticalParams->SetCerenkovMaxPhotonsPerStep(100);
    opticalParams->SetCerenkovMaxBetaChange(10.0);
    opticalParams->SetCerenkovTrackSecondariesFirst(true);

    physicsList->RegisterPhysics(opticalPhysics);
    G4StepLimiterPhysics *stepLimitPhys = new G4StepLimiterPhysics();
    physicsList->RegisterPhysics(stepLimitPhys);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new MLCActionInitialization(det));
    runManager->SetNumberOfThreads(1);

    // initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (ui)
    {
        // interactive mode
        // UImanager->ApplyCommand("/control/execute vis.mac");
        if (ui->IsGUI())
        {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }
    else if (SetId == true)
    {
        // set id in filename
        G4String command = "/control/execute ";
        G4String fileId = argv[3];
        UImanager->ApplyCommand("/MLC/Histogram/fileId " + fileId);
        G4String fileName = argv[2];
        UImanager->ApplyCommand(command + fileName);
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
        UImanager->ApplyCommand(command + fileName);
    }
    if (argc >= 3)
    {
        clock_t endtime = clock();
        std::cout << "job ends with time consumption: " << endtime / CLOCKS_PER_SEC << " s." << G4endl;
    }
    // job termination
    delete visManager;
    delete runManager;
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
