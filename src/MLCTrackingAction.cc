#include "MLCTrackingAction.hh"

#include "MLCDetectorConstruction.hh"
#include "MLCTrajectory.hh"
#include "MLCUserTrackInformation.hh"

#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCTrackingAction::MLCTrackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCTrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
    // Use custom trajectory class
    fpTrackingManager->SetTrajectory(new MLCTrajectory(aTrack));

    // This user track information is only relevant to the photons
    fpTrackingManager->SetUserTrackInformation(new MLCUserTrackInformation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCTrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
    MLCTrajectory *trajectory =
        (MLCTrajectory *)fpTrackingManager->GimmeTrajectory();
    MLCUserTrackInformation *trackInformation =
        (MLCUserTrackInformation *)aTrack->GetUserInformation();

    // draw only the photons that hit a pmt
    if (aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        if (trackInformation->GetTrackStatus() & hitPMT)
            trajectory->SetDrawTrajectory(true);
    }
    // draw all other (not optical photon) trajectories
    else
        trajectory->SetDrawTrajectory(true);

    if (trackInformation->GetForceDrawTrajectory())
        trajectory->SetDrawTrajectory(true);
}
