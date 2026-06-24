#ifndef PMSteppingAction_h
#define PMSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "PMEventAction.hh"

class G4OpBoundaryProcess;
class G4Step;

class PMSteppingAction : public G4UserSteppingAction
{
public:
    explicit PMSteppingAction(PMEventAction* ea);
    ~PMSteppingAction() override = default;

    void UserSteppingAction(const G4Step* step) override;

private:
    void FindBoundaryProcess();

    PMEventAction* fEventAction = nullptr;
    G4OpBoundaryProcess* fBoundary = nullptr;
};

#endif