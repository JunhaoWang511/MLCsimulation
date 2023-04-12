
#ifndef MLCScintHit_h
#define MLCScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

class MLCScintHit : public G4VHit
{
public:
    MLCScintHit();
    MLCScintHit(G4VPhysicalVolume* pVol);
    ~MLCScintHit();

    MLCScintHit(const MLCScintHit& right);
    const MLCScintHit& operator=(const MLCScintHit& right);
    G4bool operator==(const MLCScintHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline void SetPos0(G4ThreeVector xyz) { fPos0 = xyz; }
    inline G4ThreeVector GetPos0() { return fPos0; }

    inline void SetPos1(G4ThreeVector xyz) { fPos1 = xyz; }
    inline G4ThreeVector GetPos1() { return fPos1; }

    inline const G4VPhysicalVolume* GetPhysV() { return fPhysVol; }
    
    inline void SetStepL(G4double sl) { fStepL = sl; }
    inline void AddStepL(G4double sl) { fStepL += sl; }
    inline G4double GetStepL() { return fStepL; }

    void SetPrimary(G4bool b) { fPrimary =b; }
    G4bool GetPrimary(void) { return fPrimary; }  

    void SetParticleName(const G4String& name) { fName= name; }
    G4String& GetParticleName(void) { return fName; }
    
    void SetTrackid(G4int idd) { fTrackid = idd; }
    G4int GetTrackid(void) { return fTrackid; }

    void SetParentid(G4int idd) { fParentid = idd; }
    G4int GetParentid(void) { return fParentid; }

    void SetStepNumber(G4int snum) { fStepNumber = snum; }
    G4int GetStepNumber(void) { return fStepNumber; }
private:
    G4double fEdep;
    G4double fStepL;
    G4ThreeVector fPos;
    G4ThreeVector fPos0;
    G4ThreeVector fPos1;
    const G4VPhysicalVolume* fPhysVol;
    G4bool fPrimary;
    G4String fName;
    G4int fTrackid;
    G4int fParentid;
    G4int fStepNumber;
};

typedef G4THitsCollection<MLCScintHit> MLCScintHitsCollection;

extern G4ThreadLocal G4Allocator<MLCScintHit>* MLCScintHitAllocator;

inline void* MLCScintHit::operator new(size_t)
{
    if (!MLCScintHitAllocator)
        MLCScintHitAllocator = new G4Allocator<MLCScintHit>;
    return (void*)MLCScintHitAllocator->MallocSingle();
}

inline void MLCScintHit::operator delete(void* aHit)
{
    MLCScintHitAllocator->FreeSingle((MLCScintHit*)aHit);
}

#endif

