#ifndef MLCPMTHit_h
#define MLCPMTHit_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4VPhysicalVolume.hh"

class MLCPMTHit : public G4VHit
{
public:
    MLCPMTHit();
    MLCPMTHit(const MLCPMTHit& right);
    ~MLCPMTHit();

    const MLCPMTHit& operator=(const MLCPMTHit& right);
    G4bool operator==(const MLCPMTHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b) { fDrawit = b; }
    inline G4bool GetDrawit() { return fDrawit; }

    inline void IncPhotonCount() { ++fPhotons; }
    inline G4int GetPhotonCount() { return fPhotons; }

    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol)
    {
        this->fPhysVol = physVol;
    }
    inline G4VPhysicalVolume* GetPMTPhysVol() { return fPhysVol; }

    inline void SetPMTPos(G4double x, G4double y, G4double z)
    {
        fPos = G4ThreeVector(x, y, z);
    }

    inline G4ThreeVector GetPMTPos() { return fPos; }

private:
    G4int fPmtNumber;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;
};

typedef G4THitsCollection<MLCPMTHit> MLCPMTHitsCollection;

extern G4ThreadLocal G4Allocator<MLCPMTHit>* MLCPMTHitAllocator;

inline void* MLCPMTHit::operator new(size_t)
{
    if (!MLCPMTHitAllocator)
        MLCPMTHitAllocator = new G4Allocator<MLCPMTHit>;
    return (void*)MLCPMTHitAllocator->MallocSingle();
}

inline void MLCPMTHit::operator delete(void* aHit)
{
    MLCPMTHitAllocator->FreeSingle((MLCPMTHit*)aHit);
}

#endif

