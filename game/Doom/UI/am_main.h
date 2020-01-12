#pragma once

#include <cstdint>

void AM_Start() noexcept;
void AM_Control() noexcept;
void AM_Drawer() noexcept;

void DrawLine(const uint32_t color, const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2) noexcept;
void _thunk_DrawLine() noexcept;
