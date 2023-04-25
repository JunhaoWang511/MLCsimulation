#ifndef MLCMainVolume_h
#define MLCMainVolume_h 1

#include "MLCDetectorConstruction.hh"

#include "G4PVPlacement.hh"

class G4Box;
class G4LogicalVolume;
class G4Sphere;
class G4Tubs;

class MLCMainVolume : public G4PVPlacement
{
public:
    MLCMainVolume(G4RotationMatrix* pRot, const G4ThreeVector& tlate,
        G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo,
        MLCDetectorConstruction* c);

    G4LogicalVolume* GetLogPhotoCath() { return fPhotocath_log; }
    G4LogicalVolume* GetLogScint() { return fScint_log; }

    std::vector<G4ThreeVector> GetPmtPositions() { return fPmtPositions; }

private:
    void VisAttributes();
    void SurfaceProperties();
    void CopyValues();

    MLCDetectorConstruction* fConstructor;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fPmtLength;
    G4double fPmtThick;
    G4double fRefl;
    G4int fLayer;

    // Basic Volumes
    //
    G4Box* fScint_box;
    G4Box* fHousing_box;
    G4Box* fPmt;
    G4Box* fPhotocath;

    // Logical volumes
    //
    G4LogicalVolume* fScint_log;
    G4LogicalVolume* fHousing_log;
    G4LogicalVolume* fPmt_log;
    G4LogicalVolume* fPhotocath_log;

    //Physical volume
    //
    G4VPhysicalVolume* fHousing_phy;
    G4VPhysicalVolume* fScint_phy;

    // Sensitive Detectors positions
    std::vector<G4ThreeVector> fPmtPositions;
};

#endif

