#pragma once

#include "Doom/doomdef.h"

extern const VmPtr<gameaction_t>            gGameAction;
extern const VmPtr<skill_t>                 gGameSkill;
extern const VmPtr<gametype_t>              gNetGame;
extern const VmPtr<int32_t>                 gGameMap;
extern const VmPtr<int32_t>                 gNextMap;
extern const VmPtr<player_t[MAXPLAYERS]>    gPlayers;
extern const VmPtr<bool32_t[MAXPLAYERS]>    gbPlayerInGame;
extern const VmPtr<int32_t>                 gGameTic;
extern const VmPtr<int32_t>                 gPrevGameTic;
extern const VmPtr<int32_t>                 gLastTgtGameTicCount;
extern const VmPtr<int32_t>                 gTotalKills;
extern const VmPtr<int32_t>                 gTotalItems;
extern const VmPtr<int32_t>                 gTotalSecret;
extern const VmPtr<bool32_t>                gbDemoPlayback;
extern const VmPtr<bool32_t>                gbDemoRecording;
extern const VmPtr<bool32_t>                gbIsLevelBeingRestarted;

void G_DoLoadLevel() noexcept;
void G_PlayerFinishLevel(int32_t playerIdx) noexcept;
void G_PlayerReborn(const int32_t playerIdx) noexcept;
void G_DoReborn(const int32_t playerIdx) noexcept;
void G_CompleteLevel() noexcept;
void G_InitNew(const skill_t skill, const int32_t mapNum, const gametype_t gameType) noexcept;
void G_RunGame() noexcept;
gameaction_t G_PlayDemoPtr() noexcept;
void G_EndDemoRecording() noexcept;
