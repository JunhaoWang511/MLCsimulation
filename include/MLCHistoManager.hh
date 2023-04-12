#ifndef MLCHistoManager_h
#define MLCHistoManager_h 1

//#include "globals.hh"
//#include "g4analysis_defs.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MLCHistoManager
{
public:
	MLCHistoManager();
	~MLCHistoManager();

private:
	void Book();
	G4String fFileName;
	G4int fLayer;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

