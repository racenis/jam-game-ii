#include "score.h"

static uint32_t total_score = 0;

uint32_t GetScore() {
    return total_score;
}

void AddScore(uint32_t score) {
    total_score += score;
}