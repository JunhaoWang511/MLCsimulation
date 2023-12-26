#include "MLCPSDHit.hh"

#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<MLCPSDHit> *MLCPSDHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPSDHit::MLCPSDHit()
    : fPSDNumber(-1), fPhysVol(nullptr), fDrawit(true)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPSDHit::~MLCPSDHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPSDHit::MLCPSDHit(const MLCPSDHit &right)
    : G4VHit()
{
    fPSDNumber = right.fPSDNumber;
    fPhysVol = right.fPhysVol;
    fDrawit = right.fDrawit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MLCPSDHit &MLCPSDHit::operator=(const MLCPSDHit &right)
{
    fPSDNumber = right.fPSDNumber;
    fPhysVol = right.fPhysVol;
    fDrawit = right.fDrawit;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCPSDHit::operator==(const MLCPSDHit &right) const
{
    return (fPSDNumber == right.fPSDNumber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPSDHit::Draw()
{
    if (fDrawit && fPhysVol)
    {
        // Need a physical volume to be able to draw anything
        G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
        if (pVVisManager)
        { // Make sure that the VisManager exists
            G4VisAttributes attribs(G4Colour(1., 0., 0.));
            attribs.SetForceSolid(true);
            G4RotationMatrix rot;
            if (fPhysVol->GetRotation()) // If a rotation is defined use it
                rot = *(fPhysVol->GetRotation());
            G4Transform3D trans(rot, fPhysVol->GetTranslation()); // Create transform
            pVVisManager->Draw(*fPhysVol, attribs, trans);        // Draw it
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPSDHit::Print() {}
