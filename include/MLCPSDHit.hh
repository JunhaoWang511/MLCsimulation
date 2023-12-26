#ifndef MLCPSDHit_h
#define MLCPSDHit_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4VPhysicalVolume.hh"

class MLCPSDHit : public G4VHit
{
public:
    MLCPSDHit();
    MLCPSDHit(const MLCPSDHit &right);
    ~MLCPSDHit();

    const MLCPSDHit &operator=(const MLCPSDHit &right);
    G4bool operator==(const MLCPSDHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b) { fDrawit = b; }
    inline G4bool GetDrawit() { return fDrawit; }

    inline void SetPSDNumber(G4int n) { fPSDNumber = n; }
    inline G4int GetPSDNumber() { return fPSDNumber; }

    inline void SetPSDPhysVol(G4VPhysicalVolume *physVol)
    {
        this->fPhysVol = physVol;
    }
    inline G4VPhysicalVolume *GetPSDPhysVol() { return fPhysVol; }

    inline void SetPSDPos(G4double x, G4double y, G4double z)
    {
        fPos = G4ThreeVector(x, y, z);
    }

    inline G4ThreeVector GetPSDPos() { return fPos; }

    inline void SetPos(G4ThreeVector xyz) { fPosition = xyz; }
    inline G4ThreeVector GetPos() { return fPosition; }

private:
    G4int fPSDNumber;
    G4ThreeVector fPos;
    G4ThreeVector fPosition; // hit position by proton
    G4VPhysicalVolume *fPhysVol;
    G4bool fDrawit;
};

typedef G4THitsCollection<MLCPSDHit> MLCPSDHitsCollection;

extern G4ThreadLocal G4Allocator<MLCPSDHit> *MLCPSDHitAllocator;

inline void *MLCPSDHit::operator new(size_t)
{
    if (!MLCPSDHitAllocator)
        MLCPSDHitAllocator = new G4Allocator<MLCPSDHit>;
    return (void *)MLCPSDHitAllocator->MallocSingle();
}

inline void MLCPSDHit::operator delete(void *aHit)
{
    MLCPSDHitAllocator->FreeSingle((MLCPSDHit *)aHit);
}

#endif
