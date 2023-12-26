#include "MLCScintSD.hh"

#include "MLCScintHit.hh"

#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4VTouchable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintSD::MLCScintSD(G4String name)
    : G4VSensitiveDetector(name), fHitsCID(-1)
{
    fScintCollection = nullptr;
    collectionName.insert("scintCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintSD::~MLCScintSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCScintSD::Initialize(G4HCofThisEvent *hitsCE)
{
    fScintCollection =
        new MLCScintHitsCollection(SensitiveDetectorName, collectionName[0]);

    if (fHitsCID < 0)
    {
        fHitsCID = G4SDManager::GetSDMpointer()->GetCollectionID(fScintCollection);
    }
    hitsCE->AddHitsCollection(fHitsCID, fScintCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCScintSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4double stepL = aStep->GetStepLength();
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.)
        return false; // No edep so don't count as hit

    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4TouchableHistory *theTouchable =
        (G4TouchableHistory *)(aStep->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume *thePrePV = theTouchable->GetVolume();

    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();

    // Get the average position of the hit
    G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
    pos /= 2.;

    G4ThreeVector pos0 = thePrePoint->GetPosition();
    G4ThreeVector pos1 = thePostPoint->GetPosition();

    G4double KEnergy = thePrePoint->GetKineticEnergy();

    MLCScintHit *scintHit = new MLCScintHit(thePrePV);

    G4Track *theTrack = aStep->GetTrack();
    if (theTrack->GetParentID() == 0)
    {
        scintHit->SetPrimary(true);
    }
    else
    {
        scintHit->SetPrimary(false);
    }

    scintHit->SetKineticEnergy(KEnergy);
    scintHit->SetParticleName(theTrack->GetParticleDefinition()->GetParticleName());
    scintHit->SetTrackid(theTrack->GetTrackID());
    scintHit->SetParentid(theTrack->GetParentID());
    scintHit->SetStepNumber(theTrack->GetCurrentStepNumber());
    scintHit->SetDirection(thePrePoint->GetMomentumDirection());
    scintHit->SetEdep(edep);
    scintHit->SetPos(pos);
    scintHit->SetStepL(stepL);
    scintHit->SetPos0(pos0);
    scintHit->SetPos1(pos1);
    scintHit->SetPrePhysVolumeName(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    scintHit->SetPostPhysVolumeName(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName());

    fScintCollection->insert(scintHit);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
