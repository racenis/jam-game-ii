#ifndef JAM_GAME_II_SCORE_H
#define JAM_GAME_II_SCORE_H

#include <framework/math.h>

uint32_t GetScore();
void AddScore(uint32_t score);

bool IsWin();

void JuiceCollect();

#endif // JAM_GAME_II_SCORE_H 