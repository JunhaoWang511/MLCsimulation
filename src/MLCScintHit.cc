#include "MLCScintHit.hh"

#include "G4ios.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<MLCScintHit>* MLCScintHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintHit::MLCScintHit()
	: fEdep(0.)
	, fPos(0.)
	, fPhysVol(nullptr)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintHit::MLCScintHit(G4VPhysicalVolume* pVol)
	: fPhysVol(pVol)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintHit::~MLCScintHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCScintHit::MLCScintHit(const MLCScintHit& right)
	: G4VHit()
{
	fEdep = right.fEdep;
	fPos = right.fPos;
	fPhysVol = right.fPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MLCScintHit& MLCScintHit::operator=(const MLCScintHit& right)
{
	fEdep = right.fEdep;
	fPos = right.fPos;
	fPhysVol = right.fPhysVol;
	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MLCScintHit::operator==(const MLCScintHit&) const
{
	return false;
	// returns false because there currently isn't need to check for equality
}
