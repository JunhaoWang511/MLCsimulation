#include "MLCDetectorMessenger.hh"

#include "MLCDetectorConstruction.hh"

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

MLCDetectorMessenger::MLCDetectorMessenger(MLCDetectorConstruction* detector)
 : fMLCDetector(detector)
 {
    // Setup a command directory for detector controls with guidance
    fDetectorDir = new G4UIdirectory("/MLC/detector/");
    fDetectorDir->SetGuidance("Detector geometry control");
    //UICommand to set scintillation yield for LYSO crystal
    fScintyieldCmd = new G4UIcmdWithADouble("/MLC/detector/yield",this);
    fScintyieldCmd->SetGuidance("Set the scintillation yield for LYSO in photons/MeV.");
    fScintyieldCmd->SetParameterName("yield", false);
    fScintyieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fScintyieldCmd->SetToBeBroadcasted(false);
    //UICommand to set the thickness of degrader
    fPolythickCmd = new G4UIcmdWithADoubleAndUnit("/MLC/detector/degrader",this);
    fPolythickCmd->SetGuidance("Set the thickness of degrader in cm.");
    fPolythickCmd->SetParameterName("degrader", false);
    fPolythickCmd->SetDefaultUnit("cm");
    fPolythickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fPolythickCmd->SetToBeBroadcasted(false);
    //UICommand to set degrader on/off
    fDegraderCmd = new G4UIcmdWithABool("/MLC/detector/add_degrader",this);
    fDegraderCmd->SetGuidance("Add or remove a degrader.");
    fDegraderCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fDegraderCmd->SetToBeBroadcasted(false);
    //UICommand to set id in filename
    fFileidCmd = new G4UIcmdWithAnInteger("/MLC/detector/fileId",this);
    fFileidCmd->SetGuidance("Set file id.");
    fFileidCmd->SetParameterName("fileId", false);
    fFileidCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFileidCmd->SetToBeBroadcasted(false);
    //UICommand to set divide layer on/off
    fLayerCmd = new G4UIcmdWithABool("/MLC/detector/divide_layer",this);
    fDegraderCmd->SetGuidance("divide scintillator into 4 layers.");
    fDegraderCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fDegraderCmd->SetToBeBroadcasted(false);
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 MLCDetectorMessenger::~MLCDetectorMessenger()
 {
    delete fDetectorDir;
    delete fScintyieldCmd;
    delete fPolythickCmd;
    delete fDegraderCmd;
    delete fFileidCmd;
    delete fLayerCmd;
 }

 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 void MLCDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
 {
    if(command == fScintyieldCmd)
    {
      fMLCDetector->SetMainScintYield(fScintyieldCmd->GetNewDoubleValue(newValue));
    }
    else if(command == fPolythickCmd)
    {
      fMLCDetector->SetPolyThickness(fPolythickCmd->GetNewDoubleValue(newValue));
    }
    else if(command == fDegraderCmd)
    {
      fMLCDetector->SetDegraderOn(fDegraderCmd->GetNewBoolValue(newValue));
    }
    else if(command == fFileidCmd)
    {
      fMLCDetector->SetFileId(fFileidCmd->GetNewIntValue(newValue));
    }
    else if(command==fLayerCmd)
    {
      fMLCDetector->SetLayerOn(fLayerCmd->GetNewBoolValue(newValue));
    }
    else{G4cout<<"command format is wrong"<<G4endl;}
 }