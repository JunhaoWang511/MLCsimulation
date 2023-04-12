#include "MLCActionInitialization.hh"

#include "MLCDetectorConstruction.hh"
#include "MLCEventAction.hh"
#include "MLCPrimaryGeneratorAction.hh"
#include "MLCRunAction.hh"
#include "MLCStackingAction.hh"
#include "MLCSteppingAction.hh"
#include "MLCTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCActionInitialization::MLCActionInitialization(const MLCDetectorConstruction* det)
	: G4VUserActionInitialization()
	, fDetector(det)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCActionInitialization::~MLCActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCActionInitialization::BuildForMaster() const
{
	SetUserAction(new MLCRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCActionInitialization::Build() const
{
	SetUserAction(new MLCPrimaryGeneratorAction());

	MLCEventAction* eventAction = new MLCEventAction(fDetector);
	SetUserAction(eventAction);
	SetUserAction(new MLCStackingAction(eventAction));

	SetUserAction(new MLCRunAction());
	SetUserAction(new MLCTrackingAction());
	SetUserAction(new MLCSteppingAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
