#include "MLCHistoMessenger.hh"

#include "MLCHistoManager.hh"

#include "G4RunManager.hh"
#include "G4Scintillation.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoMessenger::MLCHistoMessenger(MLCHistoManager *Histo)
    : fHisto(Histo)
{
   // Setup a command directory for Histogram controls with guidance
   fHistoDir = new G4UIdirectory("/MLC/Histogram/");
   fHistoDir->SetGuidance("File id control");
   // UIComand to set file ID
   fFileidCmd = new G4UIcmdWithAnInteger("/MLC/Histogram/fileId", this);
   fFileidCmd->SetGuidance("Set file id.");
   fFileidCmd->SetParameterName("fileId", false);
   fFileidCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
   fFileidCmd->SetToBeBroadcasted(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCHistoMessenger::~MLCHistoMessenger()
{
   delete fHistoDir;
   delete fFileidCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCHistoMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
   if (command == fFileidCmd)
   {
      fHisto->SetFileID(fFileidCmd->GetNewIntValue(newValue));
   }
   else
   {
      G4cout << "command format is wrong" << G4endl;
   }
}