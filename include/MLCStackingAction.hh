#ifndef MLCStackingAction_h
#define MLCStackingAction_h 1

#include "G4UserStackingAction.hh"

class MLCEventAction;

class MLCStackingAction : public G4UserStackingAction
{
public:
	MLCStackingAction(MLCEventAction *);
	~MLCStackingAction();

	G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *aTrack) override;

private:
	MLCEventAction *fEventAction;
};

#endif
