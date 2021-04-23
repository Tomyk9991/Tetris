#include "TetrisField.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "Tetromino.h"

TetrisField::TetrisField()
{
    this->currentTetromino.set_color(ofColor(0, 0, 0, 0));
    this->gameField.fill(ofColor(0, 0, 0));
    
    ofAddListener(ofEvents().keyReleased, this, &TetrisField::key_pressed);
}

void TetrisField::update()
{
    if (ofGetElapsedTimef() - lastTime > TimeTick)
    {
        lastTime = ofGetElapsedTimef();

        if (this->currentTetromino.get_block_type() != BlockType::None)
        {
            this->currentTetromino.move_down();
        }
    }
}

void TetrisField::draw_game_bounds(const ofColor& borderColor)
{
    int offsetX = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth;
    int offsetY = 10;
    
    ofSetColor(borderColor);

    //horizontal
    ofDrawRectangle(offsetX, offsetY, boxWidth * this->width, boxHeight);
    ofDrawRectangle(offsetX, offsetY + boxHeight * this->height - boxHeight, boxWidth * this->width, boxHeight);

    //vertical
    ofDrawRectangle(offsetX, offsetY, boxHeight, boxHeight * this->height);
    ofDrawRectangle(offsetX + boxWidth * this->width, offsetY, boxHeight, boxHeight * this->height);
}

void TetrisField::draw_game_grid()
{
    const int offsetX = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth;
    const int offsetY = 10;
    
    ofSetColor(ofColor::white);
    for (int x = 0; x < width; ++x)
    {
        ofDrawLine(offsetX + x * boxWidth, offsetY, offsetX + x * boxWidth, boxHeight * this->height);
    }
    
}

void TetrisField::draw_game_field()
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            ofSetColor(this->gameField[x * width + y]);
            ofDrawRectangle(local_to_world(x, y), boxWidth, boxHeight);
        }
    }
}

void TetrisField::draw()
{
    draw_game_grid();
    draw_game_bounds(ofColor::darkGray);
    draw_game_field();

    this->currentTetromino.render();
}

void TetrisField::key_pressed(ofKeyEventArgs& e)
{
    if(e.keycode == ' ')
    {
        Tetromino::generate_new_random(this->currentTetromino);
    }
}

const glm::vec2& TetrisField::local_to_world(int x, int y)
{
    float xOrigin = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth + boxWidth;
    float yOrigin = 10 + boxHeight * height - boxHeight;
    
    return glm::vec2(xOrigin + x * boxWidth, yOrigin + y * boxHeight);
}
