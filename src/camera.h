#ifndef JAM_GAME_II_CAMERA_H
#define JAM_GAME_II_CAMERA_H

#include <framework/math.h>

using namespace tram;

void MongusCameraUpdate();

void MongusCameraMove(vec3);

void MongusCameraLock(bool);
void MongusCameraDynamic(bool);
void MongusCameraLessbright(bool);

void MongusCameraNudgeLeft(bool);
void MongusCameraNudgeRight(bool);

#endif // JAM_GAME_II_CAMERA_H