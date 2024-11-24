#include <sys/types.h>

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "raylib.h"

const uint width = 1000;
const uint height = 1000;

const uint rows = 60;
const uint cols = 60;

typedef std::array<std::array<bool, cols>, rows> grid_t;

std::array<std::pair<int, int>, 8> directions = {
    std::make_pair(-1, 0), std::make_pair(1, 0),   std::make_pair(0, -1),
    std::make_pair(0, 1),  std::make_pair(-1, -1), std::make_pair(-1, 1),
    std::make_pair(1, -1), std::make_pair(1, 1)};

void tick(grid_t &grid) {
  grid_t tmp_grid = grid;

  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      bool live = grid[i][j];
      uint count = 0;

      for (auto [dx, dy] : directions) {
        uint x = i + dx;
        uint y = j + dy;

        if (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y]) {
          count++;
        }
      }

      // Underpopulation
      if (live && count < 2) {
        tmp_grid[i][j] = false;
        continue;
      }

      // Perfect population
      if (live && (count == 2 || count == 3)) {
        continue;
      }

      // Overpopulation
      if (live && count > 3) {
        tmp_grid[i][j] = false;
        continue;
      }

      // Reproduction
      if (!live && count == 3) {
        tmp_grid[i][j] = true;
        continue;
      }
    }
  }

  // print tmp_grid then newline

  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      std::cout << tmp_grid[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;

  grid = tmp_grid;
}

int main() {
  InitWindow(width, height + 30, "Game of Life");
  SetTargetFPS(60);

  grid_t grid = {};

  bool running = false;

  double last_time = GetTime();

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(RAYWHITE);

      for (uint i = 0; i < rows; i++) {
        DrawLine(0, i * height / rows, width, i * height / rows, LIGHTGRAY);
      }
      DrawLine(0, height, width, height, LIGHTGRAY);

      for (uint i = 0; i < cols; i++) {
        DrawLine(i * width / cols, 0, i * width / cols, height, LIGHTGRAY);
      }

      // draw guide at the bottom side by side
      const char *guide = "SPACE: Start/Stop | C: Clear | S: Step";
      DrawText(guide, 10, height + 10, 10, DARKGRAY);

      for (uint i = 0; i < rows; i++) {
        for (uint j = 0; j < cols; j++) {
          if (grid[i][j]) {
            DrawRectangle(j * width / cols, i * height / rows, width / cols,
                          height / rows, BLACK);
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        uint x = mouse.x / ((float)width / cols);
        uint y = mouse.y / ((float)height / rows);
        grid[y][x] = !grid[y][x];
      }

      if (IsKeyPressed(KEY_SPACE)) {
        running = !running;
      }

      if (IsKeyPressed(KEY_C)) {
        running = false;
        for (uint i = 0; i < rows; i++) {
          for (uint j = 0; j < cols; j++) {
            grid[i][j] = false;
          }
        }
      }

      if (!running && IsKeyPressed(KEY_S)) {
        tick(grid);
      }

      if (running) {
        double current_time = GetTime();
        if (current_time - last_time > 0.5) {
          tick(grid);
          last_time = current_time;
        }
      }

      // draw green cirle on top left for runnig
      if (running) {
        DrawCircle(10, 10, 7, GREEN);
      } else {
        DrawCircle(10, 10, 7, RED);
      }
    }
    EndDrawing();
  }

  return 0;
}