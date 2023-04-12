#include "MLCHistoManager.hh"
#include "MLCDetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoManager::MLCHistoManager()
//    : fFileName("MLC")
{
    const MLCDetectorConstruction* fDetector = static_cast<const MLCDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fLayer = fDetector->GetLayers();
    G4int Fileid = fDetector->GetFileId();
    //G4cout<<"file id is "<<Fileid<<G4endl;
    G4int runid = G4RunManager::GetRunManager()->GetNonConstCurrentRun()->GetRunID();
    fFileName = "./output/MLC_" +std::to_string(Fileid)/*+"_"+ std::to_string(runid)*/;
    Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoManager::~MLCHistoManager() {delete G4AnalysisManager::Instance();}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCHistoManager::Book()
{
    // Create or get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName(fFileName);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetActivation(true);  // enable inactivation of histograms
    analysisManager->SetNtupleMerging(true);
    
    // Define histogram indices, titles

    // Default values (to be reset via /analysis/h1/set command)
    /*G4int nbins = 1000;
    G4double vmin = 0.;
    G4double vmax = 1000.;

    // 0
    analysisManager->CreateH1("0", "dummy", nbins, vmin, vmax);
    // 1
    analysisManager->CreateH1("hits per event", "hits per event", nbins, vmin,vmax);
    // 2
    analysisManager->CreateH1("hits above threshold", "hits per event above threshold", nbins, vmin, vmax);
    // 3
    analysisManager->CreateH1("scintillation", "scintillation photons per event", nbins, vmin, vmax);
    // 4
    analysisManager->CreateH1("Cerenkov", "Cerenkov photons per event", nbins, vmin, vmax);
    // 5
    analysisManager->CreateH1("absorbed", "absorbed photons per event", nbins, vmin, vmax);
    // 6
    analysisManager->CreateH1("boundary absorbed", "photons absorbed at boundary per event", nbins, vmin, vmax);
    // 7
    analysisManager->CreateH1("E dep", "energy deposition in scintillator per event", nbins, vmin, vmax);*/

    // 0
    analysisManager->CreateH1("stop Y", "stop Y position", 1000, 0, 10);
    
    // photon distribution histograms(1,2,3...)
    G4int PhotonNumber = G4int(300000./fLayer);
    G4cout<<"photonnumber are "<<PhotonNumber<<G4endl;
    G4cout<<"layer number are "<<fLayer<<G4endl;
    G4String Name = "100MeV_layer";
    for(G4int i=0 ;i < fLayer;i++)
    {
        G4String NameSerial = Name + std::to_string(i+1);
        analysisManager->CreateH1(NameSerial, "photon_number_distribution", PhotonNumber, 0, PhotonNumber,"none","none");
    }

    // 0
    analysisManager->CreateH2("stop X-Z", "stop X-Z position", 100, -1.25, 1.25 , 100, -1.25, 1.25,"none","none","none","none");

    analysisManager->CreateNtuple("PSDposition_PMTcollection","PSDposition_PMTcollection");
    analysisManager->CreateNtupleDColumn("Pos1X");
    analysisManager->CreateNtupleDColumn("Pos1Z");
    analysisManager->CreateNtupleDColumn("Pos2X");
    analysisManager->CreateNtupleDColumn("Pos2Z");
    analysisManager->CreateNtupleDColumn("Pos3X");
    analysisManager->CreateNtupleDColumn("Pos3Z");
    analysisManager->CreateNtupleDColumn("Pos4X");
    analysisManager->CreateNtupleDColumn("Pos4Z");
    for(G4int i=0 ;i < fLayer;i++)
    {
        G4String Namei = "Photon" + std::to_string(i+1);
        analysisManager->CreateNtupleIColumn(Namei);
    }
    analysisManager->FinishNtuple();

    // Create all histograms as inactivated
    for (G4int i = 0; i < analysisManager->GetNofH1s(); ++i)
    {
        analysisManager->SetH1Activation(i, false);
    }
    //analysisManager->SetH1Activation(0, true);
    for(G4int i=0 ;i < fLayer;i++)
    {
        analysisManager->SetH1Activation(i+1, true);
    }
    analysisManager->SetH2Activation(0, false);
}
