#ifndef JAM_GAME_II_SOUNDS_H
#define JAM_GAME_II_SOUNDS_H

#include <framework/math.h>

enum Sound {
    SOUND_PLIKUPLEKU,
    SOUND_SNIBETISNAB,
    SOUND_RIPPERINO,
    SOUND_YEET,
    SOUND_BIGYEET,
    SOUND_PICKUP,
    SOUND_LAST
};

void LoadAllSounds();

void SetPlaying (Sound, bool);
void SetPosition (Sound, tram::vec3);
void PlayOnce (Sound);

#endif // JAM_GAME_II_SOUNDS_H 