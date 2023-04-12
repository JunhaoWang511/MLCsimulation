#ifndef MLCPSD_h
#define MLCPSD_h 1

#include "MLCPSDHit.hh"

#include "G4VSensitiveDetector.hh"

#include <vector>

class G4DataVector;
class G4HCofThisEvent;
class G4Step;

class MLCPSD : public G4VSensitiveDetector
{
public:
    MLCPSD(G4String name);
    ~MLCPSD();

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) override;

    // Initialize the arrays to store PSD possitions
    inline void InitPSDs()
    {
        if (fPSDPositionsX)
            delete fPSDPositionsX;
        if (fPSDPositionsY)
            delete fPSDPositionsY;
        if (fPSDPositionsZ)
            delete fPSDPositionsZ;
        fPSDPositionsX = new G4DataVector();
        fPSDPositionsY = new G4DataVector();
        fPSDPositionsZ = new G4DataVector();
    }

    // Store a PSD position
    void SetPSDPositions(const std::vector<G4ThreeVector>& positions);

private:
    MLCPSDHitsCollection* fPSDHitCollection;

    G4DataVector* fPSDPositionsX;
    G4DataVector* fPSDPositionsY;
    G4DataVector* fPSDPositionsZ;

    G4int fHitCID;
};

#endif

