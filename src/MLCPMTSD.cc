#include "MLCPMTSD.hh"

#include "MLCDetectorConstruction.hh"
#include "MLCPMTHit.hh"
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

MLCPMTSD::MLCPMTSD(G4String name)
    : G4VSensitiveDetector(name), fPMTHitCollection(nullptr), fPMTPositionsX(nullptr), fPMTPositionsY(nullptr), fPMTPositionsZ(nullptr), fHitCID(-1)
{
    collectionName.insert("pmtHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPMTSD::~MLCPMTSD()
{
    delete fPMTPositionsX;
    delete fPMTPositionsY;
    delete fPMTPositionsZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPMTSD::SetPmtPositions(const std::vector<G4ThreeVector> &positions)
{
    for (size_t i = 0; i < positions.size(); ++i)
    {
        if (fPMTPositionsX)
            fPMTPositionsX->push_back(positions[i].x());
        if (fPMTPositionsY)
            fPMTPositionsY->push_back(positions[i].y());
        if (fPMTPositionsZ)
            fPMTPositionsZ->push_back(positions[i].z());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPMTSD::Initialize(G4HCofThisEvent *hitsCE)
{
    fPMTHitCollection =
        new MLCPMTHitsCollection(SensitiveDetectorName, collectionName[0]);

    if (fHitCID < 0)
    {
        fHitCID = G4SDManager::GetSDMpointer()->GetCollectionID(fPMTHitCollection);
    }
    hitsCE->AddHitsCollection(fHitCID, fPMTHitCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCPMTSD::ProcessHits(G4Step *, G4TouchableHistory *) { return false; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Generates a hit and uses the postStepPoint's mother volume replica number
// PostStepPoint because the hit is generated manually when the photon is
// absorbed by the photocathode

G4bool MLCPMTSD::ProcessHits_boundary(const G4Step *aStep, G4TouchableHistory *)
{
    // need to know if this is an optical photon
    if (aStep->GetTrack()->GetDefinition() !=
        G4OpticalPhoton::OpticalPhotonDefinition())
        return false;

    // User replica number 1 since photocathode is a daughter volume
    // to the pmt which was replicated
    G4int pmtNumber =
        aStep->GetPostStepPoint()->GetPhysicalVolume()->GetCopyNo();

    G4VPhysicalVolume *physVol =
        aStep->GetPostStepPoint()->GetPhysicalVolume();

    // Find the correct hit collection
    size_t n = fPMTHitCollection->entries();
    MLCPMTHit *hit = nullptr;
    for (size_t i = 0; i < n; ++i)
    {
        if ((*fPMTHitCollection)[i]->GetPMTNumber() == pmtNumber)
        {
            hit = (*fPMTHitCollection)[i];
            break;
        }
    }

    if (hit == nullptr)
    {                          // this pmt wasn't previously hit in this event
        hit = new MLCPMTHit(); // so create new hit
        hit->SetPMTNumber(pmtNumber);
        hit->SetPMTPhysVol(physVol);
        fPMTHitCollection->insert(hit);
        // hit->SetPMTPos((*fPMTPositionsX)[pmtNumber], (*fPMTPositionsY)[pmtNumber],
        //                (*fPMTPositionsZ)[pmtNumber]);
    }

    hit->IncPhotonCount(); // increment hit for the selected pmt

    return true;
}
