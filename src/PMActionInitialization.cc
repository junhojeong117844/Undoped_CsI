#include "PMActionInitialization.hh"

PMActionInitialization::PMActionInitialization()
{}

PMActionInitialization::~PMActionInitialization()
{}

void PMActionInitialization::BuildForMaster() const
{
    PMRunAction *runAction = new PMRunAction();
    SetUserAction(runAction);
}

void PMActionInitialization::Build() const
{
    // Worker thread에서 돌아갈 유저 액션 등록
    PMPrimaryGenerator *generator = new PMPrimaryGenerator();
    SetUserAction(generator);

    PMRunAction *runAction = new PMRunAction();
    SetUserAction(runAction);

    PMEventAction *eventAction = new PMEventAction();
    SetUserAction(eventAction);

    PMSteppingAction *steppingAction = new PMSteppingAction(eventAction);
    SetUserAction(steppingAction);    
}
