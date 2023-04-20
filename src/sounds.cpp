#include "sounds.h"

#include <components/audiocomponent.h>

//#include <audio/audio.h>
//#include <audio/sound.h>

using namespace tram;

static Audio::Sound* all_sounds [SOUND_LAST];
static AudioComponent* all_players [SOUND_LAST];

void LoadAllSounds() {
    all_sounds[SOUND_PLIKUPLEKU] =  Audio::Sound::Find("plikuplak");
    all_sounds[SOUND_SNIBETISNAB] = Audio::Sound::Find("snibetisnab");
    all_sounds[SOUND_RIPPERINO] =   Audio::Sound::Find("ripperino");
    all_sounds[SOUND_YEET] =        Audio::Sound::Find("yeet");
    all_sounds[SOUND_BIGYEET] =     Audio::Sound::Find("bigyeet");
    all_sounds[SOUND_PICKUP] =      Audio::Sound::Find("pickup");
    
    for (auto sound : all_sounds) sound->Load();
    
    for (auto& player : all_players) player = PoolProxy<AudioComponent>::New();
    
    all_players[SOUND_PLIKUPLEKU]->SetSound("plikuplak");
    all_players[SOUND_SNIBETISNAB]->SetSound("snibetisnab");
    all_players[SOUND_RIPPERINO]->SetSound("ripperino");
    all_players[SOUND_YEET]->SetSound("yeet");
    all_players[SOUND_BIGYEET] ->SetSound("bigyeet");
    all_players[SOUND_PICKUP]->SetSound("pickup");
    
    for (auto player : all_players) player->Init();
}

void SetPlaying (Sound sound_enum, bool playing) {
    auto sound = all_players[sound_enum];
    
    if (playing)  {
        sound->SetRepeating(true);
        sound->Play();
    } else {
        sound->SetRepeating(false);
        sound->Stop();
    }
}

void SetPosition (Sound sound_enum, vec3 position) {
    all_players[sound_enum]->SetLocation(position);
}

void PlayOnce (Sound sound_enum) {
    all_players[sound_enum]->Play();
}