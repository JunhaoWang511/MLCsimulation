#include "MLCPrimaryGeneratorAction.hh"

#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPrimaryGeneratorAction::MLCPrimaryGeneratorAction()
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName;
    fParticleGun->SetParticleDefinition(
        particleTable->FindParticle(particleName = "e-"));
    // Default energy,position,momentum
    fParticleGun->SetParticleEnergy(9 * MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCPrimaryGeneratorAction::~MLCPrimaryGeneratorAction() { delete fParticleGun; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    G4double ymax = 0.2 * cm, xmax = 0.2 * cm;
    G4double y = ymax * (2 * G4UniformRand() - 1);
    G4double x = xmax * (2 * G4UniformRand() - 1);
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, 100 * cm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
