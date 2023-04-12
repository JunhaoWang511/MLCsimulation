#include "MLCUserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCUserTrackInformation::MLCUserTrackInformation()
    : fStatus(active)
    , fReflections(0)
    , fForcedraw(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCUserTrackInformation::~MLCUserTrackInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCUserTrackInformation::AddTrackStatusFlag(int s)
{
    if (s & active)           // track is now active
        fStatus &= ~inactive;  // remove any flags indicating it is inactive
    else if (s & inactive)    // track is now inactive
        fStatus &= ~active;    // remove any flags indicating it is active
    fStatus |= s;            // add new flags
}
