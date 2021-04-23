#pragma once
#include <array>

#include "ofColor.h"
#include "ofEvents.h"
#include "Tetromino.h"

class TetrisField
{
public:
    TetrisField();
    void update();
    void draw_game_bounds(const ofColor& baseColor);
    void draw_game_grid();
    void draw_game_field();
    void draw();
    void key_pressed(ofKeyEventArgs &e);

    static int constexpr width = 10;
    static int constexpr height = 20;
    static int constexpr boxWidth = 37;
    static int constexpr boxHeight = 37;
    static float constexpr TimeTick = 0.75f; 
    
    static const glm::vec2& local_to_world(int x, int y);
    std::array<ofColor, width * height> gameField;

private:
    float lastTime = 0.;
    Tetromino currentTetromino;
};
