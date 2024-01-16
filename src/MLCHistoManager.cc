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
    // Ntuple id = 0
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
    // Ntuple id = 1
    analysisManager->CreateNtuple("EnergyDeposition_ParticleName", "EnergyDeposition_ParticleName");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->CreateNtupleSColumn("PName");
    analysisManager->CreateNtupleIColumn("EventId");
    analysisManager->FinishNtuple();

    // Create all histograms as inactivated
    for (G4int i = 0; i < analysisManager->GetNofH1s(); ++i)
    {
        analysisManager->SetH1Activation(i, false);
    }
}
