#include "MLCRunAction.hh"

#include "MLCHistoManager.hh"
#include "MLCRun.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCRunAction::MLCRunAction()
    : fRun(nullptr)
    , fHistoManager(nullptr)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCRunAction::~MLCRunAction() { delete fHistoManager; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Run* MLCRunAction::GenerateRun()
{
    fRun = new MLCRun();
    return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    //G4cout<<"current run ID is "<<run->GetRunID()<<G4endl;
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // Book predefined histograms

        fHistoManager = new MLCHistoManager();
        analysisManager->OpenFile();
    
    /*if (analysisManager->IsActive())
    {
        analysisManager->OpenFile();
    }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCRunAction::EndOfRunAction(const G4Run* /*run*/)
{
    if (isMaster)
        fRun->EndOfRun();

    // save histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    /*if (analysisManager->IsActive())
    {
        analysisManager->Write();
        analysisManager->CloseFile();
    }*/
 
        analysisManager->Write();
        analysisManager->CloseFile();
        analysisManager->Clear();
}
