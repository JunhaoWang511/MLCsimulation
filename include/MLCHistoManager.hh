#ifndef MLCHistoManager_h
#define MLCHistoManager_h 1

// #include "globals.hh"
// #include "g4analysis_defs.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "MLCHistoMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MLCHistoManager
{
public:
	MLCHistoManager();
	~MLCHistoManager();
	void SetFileID(G4int fileid) { fFileid = fileid; }
	G4int GetFileID(void) { return fFileid; }
	void Book();

private:
	MLCHistoMessenger *fHistoMessenger;
	G4String fFileName;
	G4int fLayer;
	G4int fFileid = 0;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
