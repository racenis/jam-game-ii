#ifndef JAM_GAME_II_LEVELSWITCH_H
#define JAM_GAME_II_LEVELSWITCH_H

#include <framework/worldcell.h>

using namespace Core;

void InitLevelSwitch();

void LoadHomeLevel();

void SwitchLevel(name_t level_name);

void StartLoader();

float GetLoaderProgress();

bool IsLoaderLoading();

#endif // JAM_GAME_II_LEVELSWITCH_H