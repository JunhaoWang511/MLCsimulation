#ifndef MLCRun_h
#define MLCRun_h 1

#include "globals.hh"
#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MLCRun : public G4Run
{
public:
    MLCRun();
    ~MLCRun();

    void IncPhotonCount_Scint(G4int count)
    {
        fPhotonCount_Scint += count;
        fPhotonCount_Scint2 += count * count;
    }
    void IncPhotonCount_Ceren(G4int count)
    {
        fPhotonCount_Ceren += count;
        fPhotonCount_Ceren2 += count * count;
    }
    void IncEDep(G4double dep)
    {
        fTotE += dep;
        fTotE2 += dep * dep;
    }
    void IncAbsorption(G4int count)
    {
        fAbsorptionCount += count;
        fAbsorptionCount2 += count * count;
    }
    void IncBoundaryAbsorption(G4int count)
    {
        fBoundaryAbsorptionCount += count;
        fBoundaryAbsorptionCount2 += count * count;
    }
    void IncHitCount(G4int count)
    {
        fHitCount += count;
        fHitCount2 += count * count;
    }
    void IncHitsAboveThreshold(G4int count)
    {
        fPMTsAboveThreshold += count;
        fPMTsAboveThreshold2 += count * count;
    }

    void Merge(const G4Run* run) override;

    void EndOfRun();

private:
    G4int fHitCount;
    G4int fHitCount2;
    G4int fPhotonCount_Scint;
    G4int fPhotonCount_Scint2;
    G4int fPhotonCount_Ceren;
    G4int fPhotonCount_Ceren2;
    G4int fAbsorptionCount;
    G4int fAbsorptionCount2;
    G4int fBoundaryAbsorptionCount;
    G4int fBoundaryAbsorptionCount2;
    G4int fPMTsAboveThreshold;
    G4int fPMTsAboveThreshold2;

    G4double fTotE;
    G4double fTotE2;
};

#endif  // MLCRun_h

