#include "MLCSteppingAction.hh"

#include "MLCEventAction.hh"
#include "MLCPMTSD.hh"
#include "MLCTrajectory.hh"
#include "MLCUserTrackInformation.hh"

#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCSteppingAction::MLCSteppingAction(MLCEventAction *ea)
    : fOneStepPrimaries(false), fEventAction(ea)
{
    fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCSteppingAction::~MLCSteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCSteppingAction::UserSteppingAction(const G4Step *theStep)
{
    G4Track *theTrack = theStep->GetTrack();

    if (theTrack->GetCurrentStepNumber() == 1)
        fExpectedNextStatus = Undefined;

    MLCUserTrackInformation *trackInformation =
        (MLCUserTrackInformation *)theTrack->GetUserInformation();

    G4StepPoint *thePrePoint = theStep->GetPreStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();

    G4StepPoint *thePostPoint = theStep->GetPostStepPoint();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    static G4ThreadLocal G4OpBoundaryProcess *boundary = nullptr;

    // find the boundary process only once
    if (!boundary)
    {
        G4ProcessManager *pm =
            theStep->GetTrack()->GetDefinition()->GetProcessManager();
        G4int nprocesses = pm->GetProcessListLength();
        G4ProcessVector *pv = pm->GetProcessList();
        for (G4int i = 0; i < nprocesses; ++i)
        {
            if ((*pv)[i]->GetProcessName() == "OpBoundary")
            {
                boundary = (G4OpBoundaryProcess *)(*pv)[i];
                break;
            }
        }
    }

    if (theTrack->GetParentID() == 0)
    {
        // This is a primary track

        G4TrackVector *fSecondary = fpSteppingManager->GetfSecondary();
        G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt() +
                              fpSteppingManager->GetfN2ndariesAlongStepDoIt() +
                              fpSteppingManager->GetfN2ndariesPostStepDoIt();

        // If we haven't already found the conversion position and there were
        // secondaries generated, then search for it
        if (!fEventAction->IsConvPosSet() && tN2ndariesTot > 0)
        {
            for (size_t lp1 = (*fSecondary).size() - tN2ndariesTot;
                 lp1 < (*fSecondary).size(); ++lp1)
            {
                const G4VProcess *creator = (*fSecondary)[lp1]->GetCreatorProcess();
                if (creator)
                {
                    G4String creatorName = creator->GetProcessName();
                    if (creatorName == "phot" || creatorName == "compt" ||
                        creatorName == "conv")
                    {
                        // since this is happening before the secondary is being tracked,
                        // the vertex position has not been set yet (set in initial step)
                        fEventAction->SetConvPos((*fSecondary)[lp1]->GetPosition());
                    }
                }
            }
        }

        if (fOneStepPrimaries && thePrePV->GetName() == "scintillator")
            theTrack->SetTrackStatus(fStopAndKill);
    }

    if (!thePostPV)
    { // out of world
        fExpectedNextStatus = Undefined;
        return;
    }

    if (theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        // Optical photon only
        if (thePostPV->GetName() == "expHall" || thePostPV->GetName() == "World")
            // Kill photons entering expHall or World (GDML construction)
            theTrack->SetTrackStatus(fStopAndKill);

        // Was the photon absorbed by the absorption process
        if (thePostPoint->GetProcessDefinedStep()->GetProcessName() ==
            "OpAbsorption")
        {
            fEventAction->IncAbsorption();
            trackInformation->AddTrackStatusFlag(absorbed);
        }

        boundaryStatus = boundary->GetStatus();
        if (thePostPoint->GetStepStatus() == fGeomBoundary)
        {
            // Check to see if the particle was actually at a boundary
            // Otherwise the boundary status may not be valid
            if (fExpectedNextStatus == StepTooSmall)
            {
                if (boundaryStatus != StepTooSmall)
                {
                    G4ExceptionDescription ed;
                    ed << "MLCSteppingAction::UserSteppingAction(): "
                       << "No reallocation step after reflection!" << G4endl;
                    G4Exception("MLCSteppingAction::UserSteppingAction()", "MLCExpl01",
                                JustWarning, ed,
                                "Something is wrong with the surface normal or geometry");
                }
            }
            fExpectedNextStatus = Undefined;
            switch (boundaryStatus)
            {
            case Absorption:
                trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
                fEventAction->IncBoundaryAbsorption();
                break;
            case Detection: // Note, this assumes that the volume causing detection
                            // is the photocathode because it is the only one with
                            // non-zero efficiency
            {
                // Trigger sensitive detector manually since photon is
                // absorbed but status was Detection
                G4SDManager *SDman = G4SDManager::GetSDMpointer();
                G4String sdName = "PMT_SD";
                MLCPMTSD *pmtSD = (MLCPMTSD *)SDman->FindSensitiveDetector(sdName);
                if (pmtSD)
                    pmtSD->ProcessHits_boundary(theStep, nullptr);
                else
                    G4cout << "PMT not found in SteppingAction" << G4endl;
                trackInformation->AddTrackStatusFlag(hitPMT);
                break;
            }
            case FresnelReflection:
            case TotalInternalReflection:
            case LambertianReflection:
            case LobeReflection:
            case SpikeReflection:
            case BackScattering:
                trackInformation->IncReflections();
                fExpectedNextStatus = StepTooSmall;
                break;
            default:
                break;
            }
        }
    }
}
