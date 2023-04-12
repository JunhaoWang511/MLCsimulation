#include "MLCPMTHit.hh"

#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<MLCPMTHit>* MLCPMTHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPMTHit::MLCPMTHit()
    : fPmtNumber(-1)
    , fPhotons(0)
    , fPhysVol(nullptr)
    , fDrawit(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPMTHit::~MLCPMTHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPMTHit::MLCPMTHit(const MLCPMTHit& right)
    : G4VHit()
{
    fPmtNumber = right.fPmtNumber;
    fPhotons = right.fPhotons;
    fPhysVol = right.fPhysVol;
    fDrawit = right.fDrawit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MLCPMTHit& MLCPMTHit::operator=(const MLCPMTHit& right)
{
    fPmtNumber = right.fPmtNumber;
    fPhotons = right.fPhotons;
    fPhysVol = right.fPhysVol;
    fDrawit = right.fDrawit;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCPMTHit::operator==(const MLCPMTHit& right) const
{
    return (fPmtNumber == right.fPmtNumber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPMTHit::Draw()
{
    if (fDrawit && fPhysVol)
    {  // Redraw only the PMTs that have hit counts > 0
      // Also need a physical volume to be able to draw anything
        G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
        if (pVVisManager)
        {  // Make sure that the VisManager exists
            G4VisAttributes attribs(G4Colour(1., 0., 0.));
            attribs.SetForceSolid(true);
            G4RotationMatrix rot;
            if (fPhysVol->GetRotation())  // If a rotation is defined use it
                rot = *(fPhysVol->GetRotation());
            G4Transform3D trans(rot, fPhysVol->GetTranslation());  // Create transform
            pVVisManager->Draw(*fPhysVol, attribs, trans);         // Draw it
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPMTHit::Print() {}
