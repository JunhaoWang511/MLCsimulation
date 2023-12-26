#ifndef MLCScintSD_h
#define MLCScintSD_h 1

#include "MLCScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class MLCScintSD : public G4VSensitiveDetector
{
public:
	MLCScintSD(G4String name);
	~MLCScintSD();

	void Initialize(G4HCofThisEvent *) override;
	G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *) override;

private:
	MLCScintHitsCollection *fScintCollection;
	G4int fHitsCID;
};

#endif
