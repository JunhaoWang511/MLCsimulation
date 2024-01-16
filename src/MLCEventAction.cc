#include "MLCEventAction.hh"

#include "MLCDetectorConstruction.hh"
#include "MLCHistoManager.hh"
#include "MLCPMTHit.hh"
#include "MLCPSDHit.hh"
#include "MLCRun.hh"
#include "MLCScintHit.hh"
#include "MLCTrajectory.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCEventAction::MLCEventAction(const MLCDetectorConstruction *det)
    : fDetector(det), fScintCollID(-1), fPMTCollID(-1), fPSDCollID(-1), fVerbose(0) // change to non-zero to show detail information
      ,
      fPMTThreshold(1), fForcedrawphotons(false), fForcenophotons(false)
{
    //    fEventMessenger = new MLCEventMessenger(this);

    fHitCount = 0;
    fPhotonCount_Scint = 0;
    fPhotonCount_Ceren = 0;
    fAbsorptionCount = 0;
    fBoundaryAbsorptionCount = 0;
    fTotE = 0.0;

    fConvPosSet = false;
    fEdepMax = 0.0;

    fPMTsAboveThreshold = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCEventAction::~MLCEventAction()
{ /* delete fEventMessenger;*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCEventAction::BeginOfEventAction(const G4Event *anEvent)
{
    fHitCount = 0;
    fPhotonCount_Scint = 0;
    fPhotonCount_Ceren = 0;
    fAbsorptionCount = 0;
    fBoundaryAbsorptionCount = 0;
    fTotE = 0.0;

    fConvPosSet = false;
    fEdepMax = 0.0;

    fPMTsAboveThreshold = 0;

    fEventID = anEvent->GetEventID();

    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    if (fScintCollID < 0)
        fScintCollID = SDman->GetCollectionID("scintCollection");
    if (fPMTCollID < 0)
        fPMTCollID = SDman->GetCollectionID("pmtHitCollection");
    // if (fPSDCollID < 0)
    //     fPSDCollID = SDman->GetCollectionID("PSDHitCollection");
    if (fEventID % 1000 == 0)
        G4cout << "EventID: " << fEventID << G4endl;
    // G4cout << "index: " << CLHEP::HepRandom::getTheSeed() << " seed1:" << CLHEP::HepRandom::getTheSeeds()[0] << " seed2:" << CLHEP::HepRandom::getTheSeeds()[1] << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCEventAction::EndOfEventAction(const G4Event *anEvent)
{
    G4TrajectoryContainer *trajectoryContainer =
        anEvent->GetTrajectoryContainer();

    G4int n_trajectories = 0;
    if (trajectoryContainer)
        n_trajectories = trajectoryContainer->entries();

    // extract the trajectories and draw them
    if (G4VVisManager::GetConcreteInstance())
    {
        for (G4int i = 0; i < n_trajectories; ++i)
        {
            MLCTrajectory *trj =
                (MLCTrajectory *)((*(anEvent->GetTrajectoryContainer()))[i]);
            if (trj->GetParticleName() == "opticalphoton")
            {
                trj->SetForceDrawTrajectory(fForcedrawphotons);
                trj->SetForceNoDrawTrajectory(fForcenophotons);
            }
            trj->DrawTrajectory();
        }
    }

    MLCScintHitsCollection *scintHC = nullptr;
    MLCPMTHitsCollection *pmtHC = nullptr;
    MLCPSDHitsCollection *psdHC = nullptr;
    G4HCofThisEvent *hitsCE = anEvent->GetHCofThisEvent();

    // Get the hit collections
    if (hitsCE)
    {
        if (fScintCollID >= 0)
        {
            scintHC = (MLCScintHitsCollection *)(hitsCE->GetHC(fScintCollID));
        }
        if (fPMTCollID >= 0)
        {
            pmtHC = (MLCPMTHitsCollection *)(hitsCE->GetHC(fPMTCollID));
        }
        // if (fPSDCollID >= 0)
        // {
        //     psdHC = (MLCPSDHitsCollection *)(hitsCE->GetHC(fPSDCollID));
        // }
    }

    // Hits in scintillator
    if (scintHC)
    {
        size_t n_hit = scintHC->entries();
        // G4cout<<"scintillatior hits entries is "<<n_hit<<G4endl;
        G4ThreeVector eWeightPos(0.);
        G4double edep;
        G4double edepMax = 0;
        G4ThreeVector PosEnd(0.);

        for (size_t i = 0; i < n_hit; ++i)
        {
            // gather info on hits in scintillator
            edep = (*scintHC)[i]->GetEdep();
            fTotE += edep;
            eWeightPos +=
                (*scintHC)[i]->GetPos() * edep; // calculate energy weighted pos
            if (edep > edepMax)
            {
                edepMax = edep; // store max energy deposit
                G4ThreeVector posMax = (*scintHC)[i]->GetPos();
                fPosMax = posMax;
                fEdepMax = edep;
            }
            if ((*scintHC)[i]->GetPrimary())
            {
                PosEnd = (*scintHC)[i]->GetPos();
            }
        }

        if (fTotE == 0.)
        {
            if (fVerbose > 0)
                G4cout << "No hits in the scintillator this event." << G4endl;
        }
        else
        {
            G4AnalysisManager::Instance()->FillNtupleDColumn(1, 0, fTotE / MeV);
            G4AnalysisManager::Instance()->FillNtupleSColumn(1, 1, (*scintHC)[0]->GetParticleName());
            G4AnalysisManager::Instance()->FillNtupleIColumn(1, 2, fEventID);
            G4AnalysisManager::Instance()->AddNtupleRow(1);
            // Finish calculation of energy weighted position
            eWeightPos /= fTotE;
            fEWeightPos = eWeightPos;
            if (fVerbose > 0)
            {
                G4cout << "\tEnergy weighted position of hits in MLC : "
                       << eWeightPos / mm << G4endl;
            }
        }
        if (fVerbose > 0)
        {
            G4cout << "\tTotal energy deposition in scintillator : " << fTotE / keV
                   << " (keV)" << G4endl;
        }
    }

    if (psdHC)
    {
        size_t psds = psdHC->entries();
        // G4cout<<"PSd number is "<<psds<<G4endl;
        for (size_t i = 0; i < psds; ++i)
        {
            G4ThreeVector pos = (*psdHC)[i]->GetPos();
            // G4cout<<"PSD hit position is: x="<<pos.getX()/cm<<" y="<<pos.getY()/cm<<" z="<<pos.getZ()/cm<<G4endl;
            G4AnalysisManager::Instance()->FillNtupleDColumn(0, 2 * i, pos.getX() / cm);
            G4AnalysisManager::Instance()->FillNtupleDColumn(0, 2 * i + 1, pos.getZ() / cm);
        }
    }

    if (pmtHC)
    {
        G4ThreeVector reconPos(0., 0., 0.);
        size_t pmts = pmtHC->entries();
        G4int copynumber;
        // Gather info from all PMTs
        for (size_t i = 0; i < pmts; ++i)
        {
            // G4cout << "the " << (*pmtHC)[i]->GetPMTNumber() << " pmt's photon number is " << (*pmtHC)[i]->GetPhotonCount() << G4endl;
            copynumber = (*pmtHC)[i]->GetPMTNumber();
            fHitCount += (*pmtHC)[i]->GetPhotonCount();
            // G4cout<<"the "<<i<<"th pmt has photon counts="<<(*pmtHC)[i]->GetPhotonCount()<<G4endl;
            // G4AnalysisManager::Instance()->FillNtupleIColumn(0, copynumber + 8, (*pmtHC)[i]->GetPhotonCount());
            reconPos += (*pmtHC)[i]->GetPMTPos() * (*pmtHC)[i]->GetPhotonCount();
            if ((*pmtHC)[i]->GetPhotonCount() >= fPMTThreshold)
            {
                ++fPMTsAboveThreshold;
            }
            else
            { // wasn't above the threshold, turn it back off
                (*pmtHC)[i]->SetDrawit(false);
            }
        }

        if (fHitCount > 0)
        { // don't bother unless there were hits
            reconPos /= fHitCount;
            if (fVerbose > 0)
            {
                G4cout << "\tReconstructed position of hits in MLC : " << reconPos / mm
                       << G4endl;
            }
            fReconPos = reconPos;
        }
        pmtHC->DrawAllHits();
    }
    // G4AnalysisManager::Instance()->AddNtupleRow(0);

    if (fVerbose > 0)
    {
        // End of event output. later to be controlled by a verbose level
        G4cout << "\tNumber of photons that hit PMTs in this event : " << fHitCount
               << G4endl;
        G4cout << "\tNumber of PMTs above threshold(" << fPMTThreshold
               << ") : " << fPMTsAboveThreshold << G4endl;
        G4cout << "\tNumber of photons produced by scintillation in this event : "
               << fPhotonCount_Scint << G4endl;
        G4cout << "\tNumber of photons produced by cerenkov in this event : "
               << fPhotonCount_Ceren << G4endl;
        G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
               << fAbsorptionCount << G4endl;
        G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
               << "this event : " << fBoundaryAbsorptionCount << G4endl;
        G4cout << "Unaccounted for photons in this event : "
               << (fPhotonCount_Scint + fPhotonCount_Ceren - fAbsorptionCount -
                   fHitCount - fBoundaryAbsorptionCount)
               << G4endl;
    }

    // update the run statistics
    MLCRun *run = static_cast<MLCRun *>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    run->IncHitCount(fHitCount);
    run->IncPhotonCount_Scint(fPhotonCount_Scint);
    run->IncPhotonCount_Ceren(fPhotonCount_Ceren);
    run->IncEDep(fTotE);
    run->IncAbsorption(fAbsorptionCount);
    run->IncBoundaryAbsorption(fBoundaryAbsorptionCount);
    run->IncHitsAboveThreshold(fPMTsAboveThreshold);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
