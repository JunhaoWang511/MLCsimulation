#include "MLCHistoManager.hh"
#include "MLCDetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoManager::MLCHistoManager()
//    : fFileName("MLC")
{
    fHistoMessenger = new MLCHistoMessenger(this);
    // G4int runid = G4RunManager::GetRunManager()->GetNonConstCurrentRun()->GetRunID();
    fFileName = "./output/MLC_" + std::to_string(fFileid);
    // Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoManager::~MLCHistoManager()
{
    delete G4AnalysisManager::Instance();
    delete fHistoMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCHistoManager::Book()
{
    fFileName = "./output/MLC_" + std::to_string(fFileid);
    // Create or get analysis manager
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName(fFileName);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetActivation(true); // enable inactivation of histograms
    analysisManager->SetNtupleMerging(true);

    // Define histograms and tuples

    // 0
    analysisManager->CreateH1("stop Y", "stop Y position", 1000, 0, 10);

    // photon distribution histograms(1,2,3...)
    G4int PhotonNumber = 100000;
    // G4cout << "photonnumber are " << PhotonNumber << G4endl;
    // G4cout << "layer number are " << fLayer << G4endl;

    // 1-16
    G4String Name = "ChNo.";
    for (G4int i = 0; i < 16; i++)
    {
        G4String NameSerial = Name + std::to_string(i + 1);
        analysisManager->CreateH1(NameSerial, "photon_number_distribution", PhotonNumber, 0, PhotonNumber, "none", "none");
    }

    // 0
    analysisManager->CreateH2("stop X-Z", "stop X-Z position", 100, -1.25, 1.25, 100, -1.25, 1.25, "none", "none", "none", "none");

    analysisManager->CreateNtuple("PSDposition_PMTcollection", "PSDposition_PMTcollection");
    analysisManager->CreateNtupleDColumn("Pos1X"); // 0
    analysisManager->CreateNtupleDColumn("Pos1Z"); // 1
    analysisManager->CreateNtupleDColumn("Pos2X"); // 2
    analysisManager->CreateNtupleDColumn("Pos2Z"); // 3
    analysisManager->CreateNtupleDColumn("Pos3X"); // 4
    analysisManager->CreateNtupleDColumn("Pos3Z"); // 5
    analysisManager->CreateNtupleDColumn("Pos4X"); // 6
    analysisManager->CreateNtupleDColumn("Pos4Z"); // 7
    for (G4int i = 0; i < 16; i++)                 // 8-23
    {
        G4String Namei = "Photon" + std::to_string(i + 1);
        analysisManager->CreateNtupleIColumn(Namei);
    }
    analysisManager->FinishNtuple();

    // Create all histograms as inactivated
    for (G4int i = 0; i < analysisManager->GetNofH1s(); ++i)
    {
        analysisManager->SetH1Activation(i, false);
    }
    analysisManager->SetH2Activation(0, false);
}
