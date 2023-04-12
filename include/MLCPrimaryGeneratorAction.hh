#ifndef MLCPrimaryGeneratorAction_h
#define MLCPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class MLCPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	MLCPrimaryGeneratorAction();
	~MLCPrimaryGeneratorAction();

	void GeneratePrimaries(G4Event* anEvent) override;

private:
	G4ParticleGun* fParticleGun;
};

#endif

