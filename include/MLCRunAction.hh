#include "G4UserRunAction.hh"

#ifndef MLCRunAction_h
#define MLCRunAction_h 1

class MLCRun;
class MLCHistoManager;


class G4Run;
class MLCRunAction : public G4UserRunAction
{
public:
	MLCRunAction();
	~MLCRunAction();

	G4Run* GenerateRun() override;
	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

private:
	MLCRun* fRun;
	MLCHistoManager* fHistoManager;
};

#endif
