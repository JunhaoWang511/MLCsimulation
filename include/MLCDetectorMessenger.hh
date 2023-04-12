#ifndef MLCDetectorMessenger_h
#define MLCDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MLCDetectorConstruction;

class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;
class G4UIdirectory;

class MLCDetectorMessenger : public G4UImessenger
{
  public:
  MLCDetectorMessenger(MLCDetectorConstruction*);
  ~MLCDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;   

  private:
  MLCDetectorConstruction* fMLCDetector;
  G4UIdirectory* fDetectorDir;
  G4UIcmdWithAnInteger* fNLayerCmd;
  G4UIcmdWithADouble* fScintyieldCmd;
  G4UIcmdWithADoubleAndUnit* fPolythickCmd;
  G4UIcmdWithABool* fDegraderCmd;
  G4UIcmdWithAnInteger* fFileidCmd;
};

#endif