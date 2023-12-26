#ifndef MLCActionInitialization_h
#define MLCActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class MLCDetectorConstruction;

class MLCActionInitialization : public G4VUserActionInitialization
{
public:
	MLCActionInitialization(const MLCDetectorConstruction *det);
	~MLCActionInitialization();

	void BuildForMaster() const override;
	void Build() const override;

private:
	const MLCDetectorConstruction *fDetector;
};

#endif
