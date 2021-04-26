#pragma once
#include <array>

#include "ofColor.h"
#include "ofEvents.h"
#include "ofTrueTypeFont.h"
#include "Tetromino.h"

class TetrisField
{
public:
    TetrisField();

    void add_current_tetromino();
    void check_rows();
    void reduce_row(int targetRow);

    void update();

    void draw_game_bounds(const ofColor& baseColor) const;
    void draw_game_grid() const;
    void draw_game_field();
    void draw_score();
    void draw_game_over();
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
    bool canLose = false;
    bool gameOver = false;
    int score = 0;
    ofTrueTypeFont font;

    float elapsedTime = 0.0f;
    ofColor targetColor;
    ofColor previousColor;
};
