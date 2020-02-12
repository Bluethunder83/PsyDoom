#pragma once

#include "Doom/doomdef.h"

struct texture_t;

// UV coordinates and size of the menu cursor
static constexpr uint8_t M_SKULL_TEX_U = 132;
static constexpr uint8_t M_SKULL_TEX_V = 192;
static constexpr uint8_t M_SKULL_W = 16;
static constexpr uint8_t M_SKULL_H = 18;

extern const VmPtr<texture_t>               gTex_BACK;
extern const VmPtr<int32_t[MAXPLAYERS]>     gCursorPos;
extern const VmPtr<int32_t>                 gCursorFrame;
extern const VmPtr<int32_t>                 gMenuTimeoutStartTicCon;

gameaction_t RunMenu() noexcept;
void M_Start() noexcept;

void M_Stop(const gameaction_t exitAction) noexcept;
void _thunk_M_Stop() noexcept;

gameaction_t M_Ticker() noexcept;
void _thunk_M_Ticker() noexcept;

void M_Drawer() noexcept;
