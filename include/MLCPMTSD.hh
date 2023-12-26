#ifndef MLCPMTSD_h
#define MLCPMTSD_h 1

#include "MLCPMTHit.hh"

#include "G4VSensitiveDetector.hh"

#include <vector>

class G4DataVector;
class G4HCofThisEvent;
class G4Step;

class MLCPMTSD : public G4VSensitiveDetector
{
public:
    MLCPMTSD(G4String name);
    ~MLCPMTSD();

    void Initialize(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *) override;

    // A version of processHits active on boundary
    G4bool ProcessHits_boundary(const G4Step *, G4TouchableHistory *);

    // Initialize the arrays to store pmt possitions
    inline void InitPMTs()
    {
        if (fPMTPositionsX)
            delete fPMTPositionsX;
        if (fPMTPositionsY)
            delete fPMTPositionsY;
        if (fPMTPositionsZ)
            delete fPMTPositionsZ;
        fPMTPositionsX = new G4DataVector();
        fPMTPositionsY = new G4DataVector();
        fPMTPositionsZ = new G4DataVector();
    }

    // Store a pmt position
    void SetPmtPositions(const std::vector<G4ThreeVector> &positions);

private:
    MLCPMTHitsCollection *fPMTHitCollection;

    G4DataVector *fPMTPositionsX;
    G4DataVector *fPMTPositionsY;
    G4DataVector *fPMTPositionsZ;

    G4int fHitCID;
};

#endif
