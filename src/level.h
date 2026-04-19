#ifndef LEVEL_H
#define LEVEL_H

#include "tafa.h"

void LoadLevel(t_level *level, SubGameScreen levelType);
void UpdateLevel(t_level *level, float dt, t_player *player);
void DrawLevel(t_level level, t_player player);
void UnloadLevel(t_level *level, t_player *player);

#endif // LEVEL_H
