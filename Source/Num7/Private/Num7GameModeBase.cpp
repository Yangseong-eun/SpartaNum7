#include "Num7GameModeBase.h"
#include "Num7Character.h"
#include "Num7PlayerController.h"



ANum7GameModeBase::ANum7GameModeBase()
{
    DefaultPawnClass = ANum7Character::StaticClass();
    PlayerControllerClass = ANum7PlayerController::StaticClass();
}
