#ifndef MLCEventAction_h
#define MLCEventAction_h 1

//#include "MLCEventMessenger.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"

class G4Event;
class MLCDetectorConstruction;

class MLCEventAction : public G4UserEventAction
{
public:
    MLCEventAction(const MLCDetectorConstruction*);
    ~MLCEventAction();

public:
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void SetEventVerbose(G4int v) { fVerbose = v; }

    void SetPMTThreshold(G4int t) { fPMTThreshold = t; }

    void SetForceDrawPhotons(G4bool b) { fForcedrawphotons = b; }
    void SetForceDrawNoPhotons(G4bool b) { fForcenophotons = b; }

    void IncPhotonCount_Scint() { ++fPhotonCount_Scint; }
    void IncPhotonCount_Ceren() { ++fPhotonCount_Ceren; }
    void IncEDep(G4double dep) { fTotE += dep; }
    void IncAbsorption() { ++fAbsorptionCount; }
    void IncBoundaryAbsorption() { ++fBoundaryAbsorptionCount; }
    void IncHitCount(G4int i = 1) { fHitCount += i; }

    void SetEWeightPos(const G4ThreeVector& p) { fEWeightPos = p; }
    void SetReconPos(const G4ThreeVector& p) { fReconPos = p; }
    void SetConvPos(const G4ThreeVector& p)
    {
        fConvPos = p;
        fConvPosSet = true;
    }
    void SetPosMax(const G4ThreeVector& p, G4double edep)
    {
        fPosMax = p;
        fEdepMax = edep;
    }

    G4int GetPhotonCount_Scint() const { return fPhotonCount_Scint; }
    G4int GetPhotonCount_Ceren() const { return fPhotonCount_Ceren; }
    G4int GetHitCount() const { return fHitCount; }
    G4double GetEDep() const { return fTotE; }
    G4int GetAbsorptionCount() const { return fAbsorptionCount; }
    G4int GetBoundaryAbsorptionCount() const { return fBoundaryAbsorptionCount; }

    G4ThreeVector GetEWeightPos() { return fEWeightPos; }
    G4ThreeVector GetReconPos() { return fReconPos; }
    G4ThreeVector GetConvPos() { return fConvPos; }
    G4ThreeVector GetPosMax() { return fPosMax; }
    G4double GetEDepMax() { return fEdepMax; }
    G4double IsConvPosSet() { return fConvPosSet; }

    G4int GetEventID() {return fEventID;}

    // Gets the total photon count produced
    G4int GetPhotonCount() { return fPhotonCount_Scint + fPhotonCount_Ceren; }

    void IncPMTSAboveThreshold() { ++fPMTsAboveThreshold; }
    G4int GetPMTSAboveThreshold() { return fPMTsAboveThreshold; }

private:
    //    MLCEventMessenger* fEventMessenger;
    const MLCDetectorConstruction* fDetector;

    G4int fScintCollID;
    G4int fPMTCollID;
    G4int fPSDCollID;

    G4int fVerbose;

    G4int fPMTThreshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

    G4int fHitCount;
    G4int fPhotonCount_Scint;
    G4int fPhotonCount_Ceren;
    G4int fAbsorptionCount;
    G4int fBoundaryAbsorptionCount;

    G4int fEventID;
    G4double fTotE;

    // These only have meaning if totE > 0
    // If totE = 0 then these won't be set by EndOfEventAction
    G4ThreeVector fEWeightPos;
    G4ThreeVector fReconPos;  // Also relies on hitCount>0
    G4ThreeVector fConvPos;   // true (initial) converstion position
    G4bool fConvPosSet;
    G4ThreeVector fPosMax;
    G4double fEdepMax;

    G4int fPMTsAboveThreshold;
};

#endif

