#include "score.h"

#include "levelswitch.h"

#include <framework/entity.h>

using namespace tram;

static uint32_t total_score = 0;
static uint32_t total_juice = 0;
static bool is_win = false;

uint32_t GetScore () {
    return total_score;
}

void AddScore (uint32_t score) {
    total_score += score;
}

bool IsWin () {
    return is_win;
}

void JuiceCollect () {    
    switch (total_juice) {
    case 0:
        Entity::Find("majas-ieksa-sula-1")->SetAutoLoad(true);
        break;
    case 1:
        Entity::Find("majas-ieksa-sula-2")->SetAutoLoad(true);
        break;
    case 2:
        Entity::Find("majas-ieksa-sula-3")->SetAutoLoad(true);
        break;
    }
    
    total_juice++;
    
    if (total_juice >= 3) {
        Entity::Find("majas-ieksa-kuka")->SetAutoLoad(true);
        
        is_win = true;
        
        MakeNextLoadHome();
    }
}