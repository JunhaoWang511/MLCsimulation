#include "MLCMainVolume.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCMainVolume::MLCMainVolume(G4RotationMatrix* pRot, const G4ThreeVector& tlate,G4LogicalVolume* pMotherLogical,  G4bool pMany,G4int pCopyNo, MLCDetectorConstruction* c)

    // Pass info to the G4PVPlacement constructor
    : G4PVPlacement(pRot, tlate,  new G4LogicalVolume(new G4Box("temp", 1, 1, 1), G4Material::GetMaterial("Air"), "temp", 0, 0, 0),"housing", pMotherLogical, pMany, pCopyNo),
      fConstructor(c)
{
    CopyValues();

    G4double housing_x = fScint_x + 2* fPmtThick;
    G4double housing_y = fScint_y + (2.*fLayer)* fPmtThick;
    G4double housing_z = fScint_z + 2* fPmtThick;

    //*************************** housing and scintillator
    fScint_box =
        new G4Box("scint_box", fScint_x / 2., fScint_y / (2.*fLayer), fScint_z / 2.);//!!!! photocathode material is Al, thickness is 0.25mm
    fHousing_box =
        new G4Box("housing_box", housing_x / 2., housing_y / 2., housing_z / 2.);

    fScint_log = new G4LogicalVolume(fScint_box, G4Material::GetMaterial("LYSO"),
        "scint_log", 0, 0, 0);
    fHousing_log = new G4LogicalVolume(
        fHousing_box, G4Material::GetMaterial("Al"), "housing_log", 0, 0, 0); 
        
    G4double maxStep = 0.1*mm;
    G4UserLimits* fStepLimit = nullptr;
    fStepLimit = new G4UserLimits(maxStep);
    fScint_log->SetUserLimits(fStepLimit);


    //****************** Build PMTs   

    fPmt = new G4Box("pmt_box", fScint_x / 2., fScint_y / (2.*fLayer), fPmtThick / 2.);//!!!!PMT is composed of one layer of glass and Al ,
    // both thickness is 0.25mm, length is 3mm

    // the "photocathode" is a metal slab at the back of the glass that
    // is only a very rough approximation of the real thing since it only
    // absorbs or detects the photons based on the efficiency set below

    fPhotocath = new G4Box("photocath_box", fScint_x / 2., fScint_y / (2.*fLayer), fPmtThick / 4.);

    fPmt_log =
        new G4LogicalVolume(fPmt, G4Material::GetMaterial("Glass"), "pmt_log");
    fPhotocath_log = 
        new G4LogicalVolume( fPhotocath, G4Material::GetMaterial("Si"), "photocath_log");

    new G4PVPlacement(0, G4ThreeVector(0., 0., fPmtThick / 4.), fPhotocath_log,"photocath", fPmt_log, false, 0);

    //***********Arrange pmts and scintillators inside of housing**********

    G4String str="scintillator";
    G4double y0=-((1./2-1./(2*fLayer))*fScint_y+(fLayer-1)*fPmtThick);//the first scintillator's y position
    G4double delty=1./fLayer*fScint_y+2*fPmtThick;//y position increment
    G4double z = (fScint_z + fPmtThick) / 2;
    G4int k=0;

    for(G4int i=0;i<fLayer;i++){
        //G4String scint_name=str+std::to_string(i+1);
        fScint_phy = new G4PVPlacement(0, G4ThreeVector(0.,y0+i*delty ,0.), fScint_log, str, fHousing_log, false, k);
        new G4PVPlacement(nullptr, G4ThreeVector(0.,y0+i*delty , z), fPmt_log, "pmt",fHousing_log, false, k);//the i-th layer
        fPmtPositions.push_back(G4ThreeVector(0.,y0+i*delty, z));
        k++;
    }

    VisAttributes();
    SurfaceProperties();
    
    SetLogicalVolume(fHousing_log);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCMainVolume::CopyValues()
{
    fScint_x = fConstructor->GetScintX();
    fScint_y = fConstructor->GetScintY();
    fScint_z = fConstructor->GetScintZ();
    fPmtThick = fConstructor->GetPMTThickness();
    fPmtLength = fConstructor->GetPMTLength();
    fRefl = fConstructor->GetHousingReflectivity();
    fLayer = fConstructor->GetLayers();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MLCMainVolume::VisAttributes()
{
    G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
    fHousing_log->SetVisAttributes(housing_va);

    G4VisAttributes* pmt_va = new G4VisAttributes();
    pmt_va->SetForceSolid(true);
    fPmt_log->SetVisAttributes(pmt_va);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCMainVolume::SurfaceProperties()//!!!define later
{
    std::vector<G4double> ephoton1 = { 7.0 * eV, 7.14 * eV };
    std::vector<G4double> ephoton2 = { 2.2545 * eV,2.2963 * eV,2.3396 * eV,2.3846 * eV,
    2.4314 * eV,2.48 * eV,2.5306 * eV,2.5833 * eV,2.6383 * eV,2.6957 * eV,2.7556 * eV,
    2.8182 * eV,2.8837 * eV,2.9524 * eV,3.0244 * eV,3.1 * eV,3.1795 * eV,3.2632 * eV,
    3.3514 * eV,3.4444 * eV,3.5429 * eV };

    //**Scintillator housing properties
    std::vector<G4double> reflectivity = { fRefl, fRefl };//total reflection
    std::vector<G4double> efficiency = { 0.0, 0.0 };//photon-absorption effeciency
    G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
    scintHsngPT->AddProperty("REFLECTIVITY", ephoton1, reflectivity);
    scintHsngPT->AddProperty("EFFICIENCY", ephoton1, efficiency);
    G4OpticalSurface* OpScintHousingSurface =
        new G4OpticalSurface("HousingSurface", unified, polished, dielectric_metal);
    OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);

    //**Photocathode surface properties
    std::vector<G4double> photocath_EFF = { 0.1817, 0.1904, 0.1974, 0.207, 0.2139, 0.2235,
    0.2304, 0.24, 0.247, 0.2539, 0.2583, 0.2617, 0.2643, 0.2661, 0.2670, 0.2661, 0.2643,
    0.2617, 0.2574, 0.2522, 0.2470 };//photon detection effeciency
    std::vector<G4double> Reflec = { 0.0,0.0 };//when photons hit photocathode, they are 100% absorbed, i.e., no reflection.

    G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
    photocath_mt->AddProperty("REFLECTIVITY", ephoton1, Reflec);
    photocath_mt->AddProperty("EFFICIENCY", ephoton2, photocath_EFF);
    G4OpticalSurface* photocath_opsurf = new G4OpticalSurface(
        "photocath_opsurf", glisur, polished, dielectric_metal);
    photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

    //**Create logical skin surfaces
    new G4LogicalSkinSurface("scint_surf", fHousing_log,OpScintHousingSurface);
    new G4LogicalSkinSurface("photocath_surf", fPhotocath_log, photocath_opsurf);
}
