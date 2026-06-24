#ifndef PMEventAction_h
#define PMEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"

class PMEventAction : public G4UserEventAction {
public:
  PMEventAction() = default;
  ~PMEventAction() override = default;

  void BeginOfEventAction(const G4Event*) override 
  { 
    fNpe = 0; 
    fNpe2 = 0; 
    fTotalEdep = 0.;
  }
  void EndOfEventAction(const G4Event*) override;

  void AddPE() { ++fNpe; }
  void AddPE2() { ++fNpe2; }
  int  GetPE() const { return fNpe; }
  int  GetPE2() const { return fNpe2; }

  void AddEdep(double edep) { fTotalEdep += edep; }
  double GetTotalEdep() const { return fTotalEdep; }

private:
  int fNpe = 0;
  int fNpe2 = 0;
  double fTotalEdep = 0.;
};

#endif
