#ifndef MLCTrackingAction_h
#define MLCTrackingAction_h 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"

class MLCTrackingAction : public G4UserTrackingAction
{
public:
	MLCTrackingAction();
	~MLCTrackingAction() {};

	void PreUserTrackingAction(const G4Track*) override;
	void PostUserTrackingAction(const G4Track*) override;

private:
};

#endif

