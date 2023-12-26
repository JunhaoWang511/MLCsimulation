#include "MLCDetectorConstruction.hh"

// #include "MLCDetectorMessenger.hh"
// #include "MLCMainVolume.hh"
#include "MLCPMTSD.hh"
#include "MLCScintSD.hh"
#include "MLCPSD.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UImanager.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4GDMLParser.hh"
#include "G4Color.hh"
#include "G4LogicalVolumeStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCDetectorConstruction::MLCDetectorConstruction()
    : fLYSO_mt(nullptr)
{
    fExperimentalHall_box = nullptr;
    fExperimentalHall_log = nullptr;
    fExperimentalHall_phys = nullptr;

    fScint_box = nullptr;
    fScint_log = nullptr;

    fHousing_box = nullptr;
    fHousing_log = nullptr;
    fHousing_phy = nullptr;

    fLYSO = fAir = fAl = fGlass = nullptr;
    fLu = fY = fSi = fN = fO = fC = fH = nullptr;

    //    fSaveThreshold = 0;
    SetDefaults();
    DefineMaterials();
    fParser = nullptr;
    fDetectorMessenger = new MLCDetectorMessenger(this);
}

MLCDetectorConstruction::MLCDetectorConstruction(G4String GDMLfilename)
{
    fParser = new G4GDMLParser();
    // DefineMaterials();
    fParser->Read(GDMLfilename);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MLCDetectorConstruction::~MLCDetectorConstruction()
{
    if (fLYSO_mt)
    {
        delete fLYSO_mt;
    }
    if (fDetectorMessenger)
    {
        delete fDetectorMessenger;
    }
    if (fParser)
    {
        delete fParser;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::DefineMaterials()
{
    G4double a; // atomic mass
    G4double z; // atomic number
    G4double density;

    //***Elements
    fH = new G4Element("H", "H", z = 1., a = 1.01 * g / mole);
    fC = new G4Element("C", "C", z = 6., a = 12.01 * g / mole);
    fN = new G4Element("N", "N", z = 7., a = 14.01 * g / mole);
    fO = new G4Element("O", "O", z = 8., a = 16.00 * g / mole);
    fSi = new G4Element("Si", "Si", z = 8., a = 16.00 * g / mole);
    fLu = new G4Element("Lu", "Lu", z = 71., a = 175.00 * g / mole);
    fY = new G4Element("Y", "Y", z = 39., a = 88.9 * g / mole);
    fPb = new G4Element("Pb", "Pb", z = 82., a = 207.2 * g / mole);

    //***Materials
    // LYSO crystal
    fLYSO = new G4Material("LYSO", density = 7.1 * g / cm3, 4);
    fLYSO->AddElement(fLu, 18);
    fLYSO->AddElement(fY, 2);
    fLYSO->AddElement(fSi, 10);
    fLYSO->AddElement(fO, 50);

    // PMMA
    fPMMA = new G4Material("PMMA", density = 1190. * kg / m3, 3);
    fPMMA->AddElement(fH, 8);
    fPMMA->AddElement(fC, 5);
    fPMMA->AddElement(fO, 2);

    // Glass: window material is epoxy resin (C11H12O3)n
    fGlass = new G4Material("Glass", density = 1.2 * g / cm3, 3);
    fGlass->AddElement(fC, 11);
    fGlass->AddElement(fH, 12);
    fGlass->AddElement(fO, 3);

    // Si: photocathode material
    fSI = new G4Material("Si", density = 2.33 * g / cm3, 1);
    fSI->AddElement(fSi, 1);

    // lead material
    fPB = new G4Material("Pb", density = 11.34 * g / cm3, 1);
    fPB->AddElement(fPb, 1);

    // Vacuum
    /*  fVacuum = new G4Material("Vacuum", z = 1., a = 1.01 * g / mole,
        density = universe_mean_density, kStateGas,
        0.1 * kelvin, 1.e-19 * pascal);*/

    // Air
    fAir = new G4Material("Air", density = 1.29 * mg / cm3, 2);
    fAir->AddElement(fN, 70 * perCent);
    fAir->AddElement(fO, 30 * perCent);

    // Aluminum
    fAl = new G4Material("Al", z = 13., a = 26.98 * g / mole, density = 2.7 * g / cm3);

    //***Material properties tables
    // LYSO property
    std::vector<G4double> LYSO_Energy = {2.2545 * eV, 2.2963 * eV, 2.3396 * eV, 2.3846 * eV,
                                         2.4314 * eV, 2.48 * eV, 2.5306 * eV, 2.5833 * eV, 2.6383 * eV, 2.6957 * eV, 2.7556 * eV,
                                         2.8182 * eV, 2.8837 * eV, 2.9524 * eV, 3.0244 * eV, 3.1 * eV, 3.1795 * eV, 3.2632 * eV,
                                         3.3514 * eV, 3.4444 * eV, 3.5429 * eV};

    std::vector<G4double> LYSO_SCINT = {0.0123, 0.0186, 0.0313, 0.0419, 0.0567, 0.0757,
                                        0.1115, 0.1643, 0.2424, 0.3543, 0.5106, 0.6963, 0.8906, 0.9645, 0.9856, 0.9898, 0.7766,
                                        0.2171, 0.0376, 0.0039, 0.0039};

    std::vector<G4double> LYSO_RIND = {1.8060, 1.8078, 1.8115, 1.8133, 1.8133, 1.8133,
                                       1.8169, 1.8206, 1.8206, 1.8206, 1.8224, 1.8242, 1.8279, 1.8315, 1.8352, 1.8370, 1.8370,
                                       1.8352, 1.8352, 1.8370, 1.8388};

    std::vector<G4double> LYSO_ABSL = {238.5 * cm, 226.9 * cm, 216.0 * cm, 205.5 * cm, 193.2 * cm,
                                       179.4 * cm, 165.5 * cm, 152.7 * cm, 140.0 * cm, 123.0 * cm, 107.3 * cm, 91.9 * cm, 75.4 * cm, 56.4 * cm,
                                       38.2 * cm, 15.8 * cm, 2.3 * cm, 0.4 * cm, 0.3 * cm, 0.3 * cm, 0.3 * cm}; // absorbance length.

    fLYSO_mt = new G4MaterialPropertiesTable();
    fLYSO_mt->AddProperty("SCINTILLATIONCOMPONENT1", LYSO_Energy, LYSO_SCINT);
    fLYSO_mt->AddProperty("SCINTILLATIONCOMPONENT2", LYSO_Energy, LYSO_SCINT);
    fLYSO_mt->AddProperty("RINDEX", LYSO_Energy, LYSO_RIND);
    fLYSO_mt->AddProperty("ABSLENGTH", LYSO_Energy, LYSO_ABSL);
    fLYSO_mt->AddConstProperty("SCINTILLATIONYIELD", 0. / MeV); // average scintillation yield is 30K/MeV
    fLYSO_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    fLYSO_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 45. * ns); // scintillation decay time is 45ns
    fLYSO_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 100. * ns);
    fLYSO_mt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    fLYSO_mt->AddConstProperty("SCINTILLATIONYIELD2", 0.0); // no slow decay part
    fLYSO->SetMaterialPropertiesTable(fLYSO_mt);
    // Set the Birks Constant for the LYSO scintillator
    fLYSO->GetIonisation()->SetBirksConstant(0.126 * mm / MeV); // use default Birks constant as example's.

    // glass property
    std::vector<G4double> glass_AbsLength = {420. * cm, 420. * cm, 420. * cm, 420. * cm,
                                             420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm,
                                             420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm, 420. * cm,
                                             420. * cm}; // PMT' optical windows' material.

    std::vector<G4double> glass_RIND = {1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59,
                                        1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59};
    G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
    glass_mt->AddProperty("ABSLENGTH", LYSO_Energy, glass_AbsLength);
    glass_mt->AddProperty("RINDEX", LYSO_Energy, glass_RIND);
    fGlass->SetMaterialPropertiesTable(glass_mt);

    // air property
    std::vector<G4double> air_Energy = {2.2545 * eV, 2.2963 * eV, 2.3396 * eV, 2.3846 * eV,
                                        2.4314 * eV, 2.48 * eV, 2.5306 * eV, 2.5833 * eV, 2.6383 * eV, 2.6957 * eV, 2.7556 * eV,
                                        2.8182 * eV, 2.8837 * eV, 2.9524 * eV, 3.0244 * eV, 3.1 * eV, 3.1795 * eV, 3.2632 * eV,
                                        3.3514 * eV, 3.4444 * eV, 3.5429 * eV};

    std::vector<G4double> air_RIND = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                      1., 1., 1., 1., 1., 1., 1., 1., 1.};

    G4MaterialPropertiesTable *air_mt = new G4MaterialPropertiesTable();
    air_mt->AddProperty("RINDEX", air_Energy, air_RIND);
    fAir->SetMaterialPropertiesTable(air_mt); // Give air rindex property

    // Al property
    std::vector<G4double> Al_Energy = {2.2545 * eV, 2.2963 * eV, 2.3396 * eV, 2.3846 * eV,
                                       2.4314 * eV, 2.48 * eV, 2.5306 * eV, 2.5833 * eV, 2.6383 * eV, 2.6957 * eV, 2.7556 * eV,
                                       2.8182 * eV, 2.8837 * eV, 2.9524 * eV, 3.0244 * eV, 3.1 * eV, 3.1795 * eV, 3.2632 * eV,
                                       3.3514 * eV, 3.4444 * eV, 3.5429 * eV};

    std::vector<G4double> Al_ReR = {0.59062, 0.56655, 0.54352, 0.52153, 0.50046, 0.48023,
                                    0.46085, 0.4422, 0.42422, 0.4069, 0.39018, 0.37404, 0.35841, 0.34329, 0.32866, 0.31448,
                                    0.30073, 0.28738, 0.27442, 0.26184, 0.24961};

    std::vector<G4double> Al_ImR = {5.2762, 5.1792, 5.0818, 4.9843, 4.8866, 4.7887, 4.6906,
                                    4.5925, 4.4941, 4.3956, 4.2969, 4.1981, 4.099, 3.9996, 3.9002, 3.8005, 3.7006, 3.6005,
                                    3.5, 3.3991, 3.2981};

    G4MaterialPropertiesTable *Al_mt = new G4MaterialPropertiesTable();
    Al_mt->AddProperty("REALRINDEX", Al_Energy, Al_ReR);
    Al_mt->AddProperty("IMAGINARYRINDEX", Al_Energy, Al_ImR);
    fAl->SetMaterialPropertiesTable(Al_mt); // Give Al rindex property

    // polystyrene
    G4NistManager *man = G4NistManager::Instance();
    fPoly = man->FindOrBuildMaterial("G4_POLYSTYRENE");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *MLCDetectorConstruction::Construct()
{
    if (fParser)
    {
        // find chips and set their color as red
        const G4GDMLAuxMapType *auxmap = fParser->GetAuxMap();
        for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
             iter != auxmap->end(); iter++)
        {
            G4cout << "Volume " << ((*iter).first)->GetName()
                   << " has the following list of auxiliary information: "
                   << G4endl << G4endl;
            for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                 vit != (*iter).second.end(); vit++)
            {
                if ((*vit).type == "Color")
                {
                    G4cout << "setting color " << (*vit).value
                           << " to volume " << ((*iter).first)->GetName()
                           << G4endl << G4endl;
                    G4LogicalVolume *chipLG = G4LogicalVolumeStore::GetInstance()->GetVolume(((*iter).first)->GetName());
                    G4VisAttributes *chip_va = new G4VisAttributes();
                    if ((*vit).value == "Red")
                    {
                        chip_va->SetColor(G4Color::Red());
                    }
                    else if ((*vit).value == "Blue")
                    {
                        chip_va->SetColor(G4Color::Blue());
                    }
                    else if ((*vit).value == "Green")
                    {
                        chip_va->SetColor(G4Color::Green());
                    }
                    else
                    {
                        G4cout << "could not find the color, set Green as default!" << G4endl;
                        chip_va->SetColor(G4Color::Green());
                    }
                    chipLG->SetVisAttributes(chip_va);
                }
            }
        }

        // if (fDosimeterOn) // create an air dosimeter on the surface of detector
        // {
        //     G4double Ythickness = 1 * mm;
        //     G4double Xlength = 250 * mm;
        //     G4double Zlength = 250 * mm;
        //     fDosimeter_box = new G4Box("dosimeter_box", Xlength / 2., Ythickness / 2., Zlength / 2.);
        //     fDosimeter_log = new G4LogicalVolume(fDosimeter_box, fAir, "dosimeter_log", 0, 0, 0);
        //     fDosimeter_phy = new G4PVPlacement(0, G4ThreeVector(Xlength / 2., 6. * mm, -Zlength / 2.), fDosimeter_log, "dosimeter", fParser->GetWorldVolume()->GetLogicalVolume(), false, 0);
        //     G4VisAttributes *dose_va = new G4VisAttributes(G4Colour(1, 1, 1));
        //     fDosimeter_log->SetVisAttributes(dose_va);
        // }

        return fParser->GetWorldVolume();
    }
    else
    {

        // The experimental hall walls are all 0.2m away from housing walls
        G4double expHall_x = 1 * m;
        G4double expHall_y = 1 * m;
        G4double expHall_z = 1 * m;

        // Create experimental hall
        fExperimentalHall_box =
            new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
        fExperimentalHall_log =
            new G4LogicalVolume(fExperimentalHall_box, fAir, "expHall_log", 0, 0, 0);
        fExperimentalHall_phys = new G4PVPlacement(
            0, G4ThreeVector(), fExperimentalHall_log, "expHall", 0, false, 0);

        fExperimentalHall_log->SetVisAttributes(G4VisAttributes::GetInvisible()); // hide world geometry

        // Create 4 PSDs(position sensitive detector)
        G4double PSD_x = 2.5 * cm;
        G4double PSD_y = 0.5 * mm;
        G4double PSD_z = 2.5 * cm;

        fPSD_box = new G4Box("PSD_box", PSD_x / 2, PSD_y / 2, PSD_z / 2);
        fPSD_log = new G4LogicalVolume(fPSD_box, fSI, "PSD_log", 0, 0, 0);

        fPSD_phy = new G4PVPlacement(0, G4ThreeVector(0, -22.5 * cm, 0), fPSD_log, "PSdetector", fExperimentalHall_log, false, 0); // 0th PSD
        fPSDPositions.push_back(G4ThreeVector(0, -22.5 * cm * cm, 0));

        fPSD_phy = new G4PVPlacement(0, G4ThreeVector(0, -20. * cm, 0), fPSD_log, "PSdetector", fExperimentalHall_log, false, 1); // 1th PSD
        fPSDPositions.push_back(G4ThreeVector(0, -20. * cm, 0));

        fPSD_phy = new G4PVPlacement(0, G4ThreeVector(0, -5. * cm, 0), fPSD_log, "PSdetector", fExperimentalHall_log, false, 2); // 2th PSD
        fPSDPositions.push_back(G4ThreeVector(0, -5. * cm, 0));

        fPSD_phy = new G4PVPlacement(0, G4ThreeVector(0, -2.5 * cm, 0), fPSD_log, "PSdetector", fExperimentalHall_log, false, 3); // 3th PSD
        fPSDPositions.push_back(G4ThreeVector(0, -2.5 * cm, 0));

        // Place degrader in front of scintillator
        if (fDegraderOn)
        {
            fDegrader_box = new G4Box("degrader_box", 1.25 * cm, fPolyThick / 2, 1.25 * cm);
            fDegrader_log = new G4LogicalVolume(fDegrader_box, fPoly, "degrader_log", 0, 0, 0);
            fDegrader_phy = new G4PVPlacement(0, G4ThreeVector(0, -12.5 * cm, 0), fDegrader_log, "degrader", fExperimentalHall_log, false, 0);

            G4VisAttributes *degrader_va = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
            degrader_va->SetForceSolid(true);
            fDegrader_log->SetVisAttributes(degrader_va);
        }

        // Create Housng and Scintillator
        G4double housing_x = fScint_x + 2. * fPmtGap;
        G4double housing_y = fScint_y + 2. * fPmtGap;
        G4double housing_z = fScint_z + 4. * fWinThick;
        if (fLayerOn)
            fScint_box =
                new G4Box("scint_box", fScint_x / 2., fPmtLength / 2., fScint_z / 2.);
        else
            fScint_box =
                new G4Box("scint_box", fScint_x / 2., fScint_y / 2., fScint_z / 2.);

        fHousing_box =
            new G4Box("housing_box", housing_x / 2., housing_y / 2., housing_z / 2.);

        fScint_log = new G4LogicalVolume(fScint_box, G4Material::GetMaterial("LYSO"),
                                         "scint_log", 0, 0, 0);
        fHousing_log = new G4LogicalVolume(
            fHousing_box, G4Material::GetMaterial("Al"), "housing_log", 0, 0, 0);

        fHousing_phy = new G4PVPlacement(
            0, G4ThreeVector(), fHousing_log, "housing", fExperimentalHall_log, false, 0);
        /*fScint_phy = new G4PVPlacement(
            0, G4ThreeVector(), fScint_log, "scintillator", fHousing_log, false, 0);*/

        //****************** Build SiPMs

        fPmt = new G4Box("pmt_box", fPmtLength / 2., fPmtLength / 2., fWinThick); // SiPM is composed of one layer of glass and Si ,
        // both thickness is 0.25mm, length is 3mm

        // the "photocathode" is a metal slab at the back of the glass that
        // is only a very rough approximation of the real thing since it only
        // absorbs or detects the photons based on the efficiency set below

        fPhotocath = new G4Box("photocath_box", fPmtLength / 2., fPmtLength / 2., fWinThick / 2.);

        fPmt_log =
            new G4LogicalVolume(fPmt, G4Material::GetMaterial("Glass"), "pmt_log");
        fPhotocath_log =
            new G4LogicalVolume(fPhotocath, G4Material::GetMaterial("Si"), "photocath_log");

        new G4PVPlacement(0, G4ThreeVector(0., 0., fWinThick / 2.), fPhotocath_log, "photocath", fPmt_log, false, 0);

        //***********Arrange pmts and scintillators inside of housing, and substract edges**********

        G4String str = "scintillator";
        G4double xy0 = -3. / 2 * (fPmtLength + fPmtGap); // the first scintillator(SiPM)'s x/y position
        G4double deltxy = fPmtLength + fPmtGap;          // x/y position increment
        G4double z = fScint_z / 2. + fWinThick;
        // G4double length = (fScint_x + fPmtGap) / 2;
        // G4double length_layer = (1./fLayer*fScint_y+fPmtGap) / 2;
        G4int k = 0;
        G4int j = 0;

        for (G4int i = 0; i < 4; i++)
        {
            // G4String scint_name=str+std::to_string(i+1);
            if (fLayerOn)
                new G4PVPlacement(0, G4ThreeVector(0., xy0 + i * deltxy, 0.), fScint_log, str, fHousing_log, false, i); // i-th scintillator
            for (j = 0; j < 4; j++)
            {
                new G4PVPlacement(nullptr, G4ThreeVector(xy0 + j * deltxy, xy0 + i * deltxy, z), fPmt_log, "pmt", fHousing_log, false, k); //(4*i+j)-th pmt
                fPmtPositions.push_back(G4ThreeVector(xy0 + j * deltxy, xy0 + i * deltxy, z));
                k++;
            }
        }
        if (!fLayerOn)
            new G4PVPlacement(0, G4ThreeVector(0., 0, 0.), fScint_log, str, fHousing_log, false, 0);

        // Set optical surface property
        std::vector<G4double> ephoton1 = {7.0 * eV, 7.14 * eV};
        std::vector<G4double> ephoton2 = {2.2545 * eV, 2.2963 * eV, 2.3396 * eV, 2.3846 * eV,
                                          2.4314 * eV, 2.48 * eV, 2.5306 * eV, 2.5833 * eV, 2.6383 * eV, 2.6957 * eV, 2.7556 * eV,
                                          2.8182 * eV, 2.8837 * eV, 2.9524 * eV, 3.0244 * eV, 3.1 * eV, 3.1795 * eV, 3.2632 * eV,
                                          3.3514 * eV, 3.4444 * eV, 3.5429 * eV};

        //**Scintillator housing properties
        std::vector<G4double> reflectivity = {fRefl, fRefl}; // total reflection
        std::vector<G4double> efficiency = {0.0, 0.0};       // photon-absorption effeciency
        G4MaterialPropertiesTable *scintHsngPT = new G4MaterialPropertiesTable();
        scintHsngPT->AddProperty("REFLECTIVITY", ephoton1, reflectivity);
        scintHsngPT->AddProperty("EFFICIENCY", ephoton1, efficiency);
        G4OpticalSurface *OpScintHousingSurface =
            new G4OpticalSurface("HousingSurface", unified, polished, dielectric_metal);
        OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);

        //**Photocathode surface properties
        std::vector<G4double> photocath_EFF = {0.1817, 0.1904, 0.1974, 0.207, 0.2139, 0.2235,
                                               0.2304, 0.24, 0.247, 0.2539, 0.2583, 0.2617, 0.2643, 0.2661, 0.2670, 0.2661, 0.2643,
                                               0.2617, 0.2574, 0.2522, 0.2470}; // photon detection effeciency
        std::vector<G4double> Reflec = {0.0, 0.0};                              // when photons hit photocathode, they are 100% absorbed, i.e., no reflection.

        G4MaterialPropertiesTable *photocath_mt = new G4MaterialPropertiesTable();
        photocath_mt->AddProperty("REFLECTIVITY", ephoton1, Reflec);
        photocath_mt->AddProperty("EFFICIENCY", ephoton2, photocath_EFF);
        G4OpticalSurface *photocath_opsurf = new G4OpticalSurface(
            "photocath_opsurf", unified, polished, dielectric_metal);
        photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

        /*for(G4int i=0;i<fLayer;i++)
        {
            new G4LogicalBorderSurface("scint_surf",fScint_phy[i],fHousing_phy,OpScintHousingSurface);
        }*/
        new G4LogicalSkinSurface("scint_surf", fHousing_log, OpScintHousingSurface);
        new G4LogicalSkinSurface("photocath_surf", fPhotocath_log, photocath_opsurf);

        // Set step length limit for Scintillator
        G4double maxStep = 0.1 * mm;
        G4UserLimits *fStepLimit = nullptr;
        fStepLimit = new G4UserLimits(maxStep);
        fScint_log->SetUserLimits(fStepLimit);
        // fDegrader_log->SetUserLimits(fStepLimit);

        // Set vis attribute
        G4VisAttributes *housing_va = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
        fHousing_log->SetVisAttributes(housing_va);

        G4VisAttributes *pmt_va = new G4VisAttributes();
        pmt_va->SetForceSolid(true);
        fPmt_log->SetVisAttributes(pmt_va);

        return fExperimentalHall_phys;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::ConstructSDandField()
{
    if (fParser)
    {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        G4String SDName = "ChipSD";
        MLCScintSD *chip_SD = new MLCScintSD(SDName);
        SDman->AddNewDetector(chip_SD);
        // now we are looking for sensitive detectors setting them for the volumes
        const G4GDMLAuxMapType *auxmap = fParser->GetAuxMap();
        for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
             iter != auxmap->end(); iter++)
        {
            G4cout << "Volume " << ((*iter).first)->GetName()
                   << " has the following list of auxiliary information: "
                   << G4endl << G4endl;
            for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                 vit != (*iter).second.end(); vit++)
            {
                if ((*vit).type == "SensDet")
                {
                    G4cout << "Attaching sensitive detector " << (*vit).value
                           << " to volume " << ((*iter).first)->GetName()
                           << G4endl << G4endl;

                    G4VSensitiveDetector *mydet =
                        SDman->FindSensitiveDetector((*vit).value); //(*vit).value should be set as "ChipSD"
                    if (mydet)
                    {
                        G4LogicalVolume *myvol = (*iter).first;
                        myvol->SetSensitiveDetector(mydet);
                    }
                    else
                    {
                        G4cout << (*vit).value << " detector not found" << G4endl;
                    }
                }
            }
        }
    }
    else
    {
        // PMT SD
        MLCPMTSD *pmt = fPmt_SD.Get();
        if (!pmt)
        {
            // Created here so it exists as pmts are being placed
            G4cout << "Construction /MLCDet/pmtSD" << G4endl;
            MLCPMTSD *pmt_SD = new MLCPMTSD("/MLCDet/pmtSD");
            fPmt_SD.Put(pmt_SD);

            pmt_SD->InitPMTs();
            pmt_SD->SetPmtPositions(GetPmtPositions());
        }
        else
        {
            pmt->InitPMTs();
            pmt->SetPmtPositions(GetPmtPositions());
        }
        G4SDManager::GetSDMpointer()->AddNewDetector(fPmt_SD.Get());
        // sensitive detector is not actually on the photocathode.
        // processHits gets done manually by the stepping action.
        // It is used to detect when photons hit and get absorbed & detected at the
        // boundary to the photocathode (which doesn't get done by attaching it to a
        // logical volume.
        // It does however need to be attached to something or else it doesn't get
        // reset at the begining of events

        SetSensitiveDetector(fPhotocath_log, fPmt_SD.Get());

        // PSDs
        MLCPSD *psd = fPSD.Get();
        if (!fPSD.Get())
        {
            // Created here so it exists as PSDs are being placed
            G4cout << "Construction /MLCDet/PSD" << G4endl;
            MLCPSD *pSD = new MLCPSD("/MLCDet/pSD");
            fPSD.Put(pSD);

            pSD->InitPSDs();
            pSD->SetPSDPositions(GetPSDPositions());
        }
        else
        {
            psd->InitPSDs();
            psd->SetPSDPositions(GetPSDPositions());
        }
        G4SDManager::GetSDMpointer()->AddNewDetector(fPSD.Get());
        SetSensitiveDetector(fPSD_log, fPSD.Get());

        // Scint SD

        if (!fScint_SD.Get())
        {
            G4cout << "Construction /MLCDet/scintSD" << G4endl;
            MLCScintSD *scint_SD = new MLCScintSD("/MLCDet/scintSD");
            fScint_SD.Put(scint_SD);
        }
        G4SDManager::GetSDMpointer()->AddNewDetector(fScint_SD.Get());
        SetSensitiveDetector(fScint_log, fScint_SD.Get());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetDimensions(G4ThreeVector dims)
{
    fScint_x = dims[0];
    fScint_y = dims[1];
    fScint_z = dims[2];
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetPMTLength(G4double pmtLength)
{
    fPmtLength = pmtLength;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetPMTThickness(G4double pmtThick)
{
    fPmtGap = pmtThick;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Reset the degrader's thickness
void MLCDetectorConstruction::SetPolyThickness(G4double polyThick)
{
    fPolyThick = polyThick;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetDefaults()
{
    // Resets to default values
    fScint_x = 2.5 * cm;
    fScint_y = 2.5 * cm;
    fScint_z = 2.5 * cm;
    fPmtLength = 6 * mm;
    fPmtGap = 0.2 * mm;
    fWinThick = 0.15 * mm;
    fPolyThick = 3. * cm;
    fRefl = 0.95;
    fFileId = 0;
    fLayerOn = true;
    fDegraderOn = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetHousingReflectivity(G4double r)
{
    fRefl = r;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetDegraderOn(G4bool b)
{
    fDegraderOn = b;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetLayerOn(G4bool b)
{
    fLayerOn = b;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MLCDetectorConstruction::SetMainScintYield(G4double y)
{
    fLYSO_mt->AddConstProperty("SCINTILLATIONYIELD", y / MeV);
}

void MLCDetectorConstruction::SetFileId(G4int i)
{
    fFileId = i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*void MLCDetectorConstruction::SetSaveThreshold(G4int save)
{
    // Sets the save threshold for the random number seed. If the number of
    // photons generated in an event is lower than this, then save the seed for
    // this event in a file called run###evt###.rndm

    fSaveThreshold = save;
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......*/
