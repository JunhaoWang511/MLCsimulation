#ifndef MLCDetectorConstruction_h
#define MLCDetectorConstruction_h 1

#include "MLCDetectorMessenger.hh"

#include "G4Cache.hh"
#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4GDMLAuxStructType.hh"

class MLCPMTSD;
class MLCScintSD;
class MLCPSD;

class G4Box;
class G4Element;
class G4LogicalVolume;
class G4Material;
class G4MaterialPropertiesTable;
class G4Sphere;
class G4Tubs;
class G4VPhysicalVolume;
class G4GDMLParser;

class MLCDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MLCDetectorConstruction();
	MLCDetectorConstruction(G4String GDMLfilename);
	~MLCDetectorConstruction();

	G4VPhysicalVolume *Construct() override;
	void ConstructSDandField() override;
	void SetDimensions(G4ThreeVector);
	void SetPMTLength(G4double);
	void SetPMTThickness(G4double);
	void SetDefaults();
	void SetLayers(G4int);
	void SetPolyThickness(G4double);
	void SetFileId(G4int);

	// Get values
	G4double GetScintX() const { return fScint_x; }
	G4double GetScintY() const { return fScint_y; }
	G4double GetScintZ() const { return fScint_z; }
	G4double GetPMTLength() const { return fPmtLength; }
	G4int GetFileId() const { return fFileId; }
	G4bool GetLayerOn() const { return fLayerOn; }
	void SetHousingReflectivity(G4double);
	G4double GetHousingReflectivity() const { return fRefl; }
	void SetDegraderOn(G4bool);
	void SetLayerOn(G4bool);

	/*void SetMainVolumeOn(G4bool b);
	G4bool GetMainVolumeOn() const { return fMainVolumeOn; }*/

	void SetMainScintYield(G4double);

private:
	// GDML reader functions
	void DumpStructure();
	void DumpVolume(G4VPhysicalVolume *physvol, G4String prefix, G4bool expanded = true) const;
	void ReadAuxiliary();
	void PrintAuxiliary(const G4GDMLAuxListType *, G4String);
	void ReadSurfaceProperty(const G4GDMLAuxListType *, G4String);
	G4bool ReadBorderProperty(const G4GDMLAuxListType *, G4String);
	G4bool ReadSkinProperty(const G4GDMLAuxListType *, G4String);
	void ReadSensitiveDetector(const G4GDMLAuxListType *, G4String);
	void RegisterSensitiveDetector(const G4GDMLAuxListType *, G4String);
	void ReadVisProperty(const G4GDMLAuxListType *, G4String);
	void SetVisProperty(const G4GDMLAuxListType *, G4String);
	// void ReadVisAttribute(const G4GDMLAuxListType *);
	G4VPhysicalVolume *fWorldPV;
	G4LogicalVolume *fPMT_det;
	G4LogicalVolume *fSCIN_det;
	G4String fSCIN_name;
	G4String fPMT_name;

	void DefineMaterials();
	MLCDetectorMessenger *fDetectorMessenger;

	G4Box *fExperimentalHall_box;
	G4LogicalVolume *fExperimentalHall_log;
	G4VPhysicalVolume *fExperimentalHall_phys;

	G4Box *fScint_box;
	G4Box *fPSD_box;
	G4Box *fHousing_box;
	G4Box *fPmt;
	G4Box *fPhotocath;
	G4Box *fDegrader_box;
	G4Box *fFoil_box;

	G4LogicalVolume *fScint_log;
	G4LogicalVolume *fPSD_log;
	G4LogicalVolume *fHousing_log;
	G4LogicalVolume *fPmt_log;
	G4LogicalVolume *fPhotocath_log;
	G4LogicalVolume *fDegrader_log;
	G4LogicalVolume *fFoil_log;

	G4VPhysicalVolume *fHousing_phy;
	G4VPhysicalVolume *fScint_phy;
	G4VPhysicalVolume *fPSD_phy;
	G4VPhysicalVolume *fDegrader_phy;
	G4VPhysicalVolume *fFoil_phy;

	// Materials & Elements
	G4Material *fLYSO;
	G4Element *fLu;
	G4Element *fY;
	G4Element *fSi;
	G4Element *fO;
	G4Element *fC;
	G4Element *fH;
	G4Element *fN;
	G4Element *fPb;
	G4Material *fAl; // fully reflective wrapping material
	G4Material *fAir;
	G4Material *fGlass;
	G4Material *fSI;
	G4Material *fPB;
	G4Material *fPoly;
	G4Material *fPMMA;
	// Geometry
	G4double fPmtGap;
	G4double fWinThick;
	G4double fPmtLength;
	G4double fPmtThick;
	G4double fPolyThick;
	G4double fScint_x;
	G4double fScint_y;
	G4double fScint_z;

	G4int fFileId;
	G4double fRefl;
	G4bool fDegraderOn;
	G4bool fLayerOn;

	G4MaterialPropertiesTable *fLYSO_mt;

	// Sensitive Detectors
	G4Cache<MLCScintSD *> fScint_SD;
	G4Cache<MLCPMTSD *> fPmt_SD;
	G4Cache<MLCPSD *> fPSD;

	// GDML parser
	G4GDMLParser *fParser;

	// PMT positions
	std::vector<G4ThreeVector> GetPmtPositions() { return fPmtPositions; }
	std::vector<G4ThreeVector> fPmtPositions;

	// PSD positions
	std::vector<G4ThreeVector> GetPSDPositions() { return fPSDPositions; }
	std::vector<G4ThreeVector> fPSDPositions;
};
#endif // !MLCDetectorConstruction_h
