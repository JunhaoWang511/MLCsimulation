#ifndef MLCSteppingAction_h
#define MLCSteppingACtion_h 1

#include "globals.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UserSteppingAction.hh"

class MLCEventAction;
class MLCTrackingAction;
// class MLCSteppingMessenger;

class MLCSteppingAction : public G4UserSteppingAction
{
public:
	MLCSteppingAction(MLCEventAction *);
	~MLCSteppingAction();

	void UserSteppingAction(const G4Step *) override;

	void SetOneStepPrimaries(G4bool b) { fOneStepPrimaries = b; }
	G4bool GetOneStepPrimaries() { return fOneStepPrimaries; }

private:
	G4bool fOneStepPrimaries;
	MLCEventAction *fEventAction;

	G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
