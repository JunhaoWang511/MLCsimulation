#include "MLCPSD.hh"

#include "MLCDetectorConstruction.hh"
#include "MLCPSDHit.hh"
#include "MLCUserTrackInformation.hh"

#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPSD::MLCPSD(G4String name)
    : G4VSensitiveDetector(name), fPSDHitCollection(nullptr), fPSDPositionsX(nullptr), fPSDPositionsY(nullptr), fPSDPositionsZ(nullptr), fHitCID(-1)
{
    collectionName.insert("PSDHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPSD::~MLCPSD()
{
    delete fPSDPositionsX;
    delete fPSDPositionsY;
    delete fPSDPositionsZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPSD::SetPSDPositions(const std::vector<G4ThreeVector> &positions)
{
    for (size_t i = 0; i < positions.size(); ++i)
    {
        if (fPSDPositionsX)
            fPSDPositionsX->push_back(positions[i].x());
        if (fPSDPositionsY)
            fPSDPositionsY->push_back(positions[i].y());
        if (fPSDPositionsZ)
            fPSDPositionsZ->push_back(positions[i].z());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPSD::Initialize(G4HCofThisEvent *hitsCE)
{
    fPSDHitCollection =
        new MLCPSDHitsCollection(SensitiveDetectorName, collectionName[0]);

    if (fHitCID < 0)
    {
        fHitCID = G4SDManager::GetSDMpointer()->GetCollectionID(fPSDHitCollection);
    }
    hitsCE->AddHitsCollection(fHitCID, fPSDHitCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCPSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    // Only record primary particle proton
    if (aStep->GetTrack()->GetParentID() != 0)
        return false;
    // User replica number 0 since PSD is a mother volume
    // to the world which was replicated
    G4int PSDNumber =
        aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber();

    G4VPhysicalVolume *physVol =
        aStep->GetPreStepPoint()->GetTouchable()->GetVolume();

    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();

    // Find the correct hit collection
    size_t n = fPSDHitCollection->entries();
    MLCPSDHit *hit = nullptr;
    for (size_t i = 0; i < n; ++i)
    {
        if ((*fPSDHitCollection)[i]->GetPSDNumber() == PSDNumber)
        {
            hit = (*fPSDHitCollection)[i];
            break;
        }
    }

    if (hit == nullptr)
    {                          // this PSD wasn't previously hit in this event
        hit = new MLCPSDHit(); // so create new hit
        hit->SetPSDNumber(PSDNumber);
        hit->SetPSDPhysVol(physVol);
        fPSDHitCollection->insert(hit);
        hit->SetPSDPos((*fPSDPositionsX)[PSDNumber], (*fPSDPositionsY)[PSDNumber],
                       (*fPSDPositionsZ)[PSDNumber]);
    }

    hit->SetPos(pos);

    return true;
}
