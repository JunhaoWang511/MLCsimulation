#ifndef MLCHistoMessenger_h
#define MLCHistoMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MLCHistoManager;

class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;
class G4UIdirectory;

class MLCHistoMessenger : public G4UImessenger
{
public:
  MLCHistoMessenger(MLCHistoManager *);
  ~MLCHistoMessenger();

  void SetNewValue(G4UIcommand *, G4String) override;

private:
  MLCHistoManager *fHisto;
  G4UIdirectory *fHistoDir;
  G4UIcmdWithAnInteger *fFileidCmd;
};

#endif