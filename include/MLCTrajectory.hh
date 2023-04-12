#ifndef MLCTrajectory_h
#define MLCTrajectory_h 1

#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"

class G4Polyline;
class G4ParticleDefinition;

class MLCTrajectory : public G4Trajectory
{
public:
    MLCTrajectory();
    MLCTrajectory(const G4Track* aTrack);
    MLCTrajectory(MLCTrajectory&);
    ~MLCTrajectory();

    void DrawTrajectory() const override;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    void SetDrawTrajectory(G4bool b) { fDrawit = b; }
    void SetForceDrawTrajectory(G4bool b) { fForceDraw = b; }
    void SetForceNoDrawTrajectory(G4bool b) { fForceNoDraw = b; }

private:
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<MLCTrajectory>* MLCTrajectoryAllocator;

inline void* MLCTrajectory::operator new(size_t)
{
    if (!MLCTrajectoryAllocator)
        MLCTrajectoryAllocator = new G4Allocator<MLCTrajectory>;
    return (void*)MLCTrajectoryAllocator->MallocSingle();
}

inline void MLCTrajectory::operator delete(void* aTrajectory)
{
    MLCTrajectoryAllocator->FreeSingle((MLCTrajectory*)aTrajectory);
}

#endif

