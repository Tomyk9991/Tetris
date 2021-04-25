#include "TetrisField.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "Tetromino.h"

TetrisField::TetrisField()
{
    this->currentTetromino.set_color(ofColor(0, 0, 0, 0));

    for (int i = 0; i < width * height; ++i)
    {
        this->gameField[i].set(0, 0, 0, 0);
    }

    ofAddListener(ofEvents().keyReleased, this, &TetrisField::key_pressed);

    Tetromino::generate_new_random(this->currentTetromino);
}

void TetrisField::add_current_tetromino()
{
    int i = 0;
    glm::vec2 pos = this->currentTetromino.get_position();
    pos.y = -pos.y;
    pos.y -= 4;

    for (int y = 4; y >= 0; --y)
    {
        for (int x = 0; x < 4; ++x)
        {
            const bool isFilled = this->currentTetromino.get_block_type() & 1 << i;
            if (isFilled)
            {
                this->gameField[(pos.x + x) * height + (pos.y + y - 1)].set(this->currentTetromino.get_color());
            }

            i++;
        }
    }
}

void TetrisField::update()
{
    if (ofGetElapsedTimef() - lastTime > TimeTick)
    {
        lastTime = ofGetElapsedTimef();

        if (this->currentTetromino.get_block_type() != BlockType::None)
        {
            if (this->currentTetromino.move(glm::vec2(0, 1), this->gameField.data()))
            {
                add_current_tetromino();
                Tetromino::generate_new_random(this->currentTetromino);
            }
        }
    }
}

void TetrisField::draw_game_bounds(const ofColor& borderColor) const
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

void TetrisField::draw_game_grid() const
{
    const int offsetX = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth;
    const int offsetY = 10;

    ofSetColor(ofColor::white);
    for (int x = 1; x < width; ++x)
    {
        ofDrawLine(offsetX + x * boxWidth, offsetY, offsetX + x * boxWidth, boxHeight * this->height);
    }

    for (int y = 1; y < height; ++y)
    {
        ofDrawLine(offsetX, offsetY + y * boxHeight, offsetX + width * this->boxWidth, offsetY + y * boxHeight);
    }
}

void TetrisField::draw_game_field()
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            ofSetColor(this->gameField[x * height + y]);

            int originX = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth + boxWidth;
            int originY = 10 + boxHeight * height - boxHeight * 2;
            ofDrawRectangle(originX + x * boxWidth, originY - y * boxHeight, boxWidth, boxHeight);
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
    if (e.key == 'a' || e.key == ofKey::OF_KEY_LEFT)
    {
        if (this->currentTetromino.move(glm::vec2(-1, 0), this->gameField.data()))
        {
            add_current_tetromino();
            Tetromino::generate_new_random(this->currentTetromino);
        }
    }

    if (e.key == 'd' || e.key == ofKey::OF_KEY_RIGHT)
    {
        if (this->currentTetromino.move(glm::vec2(1, 0), this->gameField.data()))
        {
            add_current_tetromino();
            Tetromino::generate_new_random(this->currentTetromino);
        }
    }

    if (e.key == 's' || e.key == ofKey::OF_KEY_DOWN)
    {
        if(this->currentTetromino.move(glm::vec2(0, 1), this->gameField.data()))
        {
            add_current_tetromino();
            Tetromino::generate_new_random(this->currentTetromino);
        }
    }

    if (e.key == ' ')
    {
        Tetromino::rotate_tetromino(this->currentTetromino);
    }
}

const glm::vec2& TetrisField::local_to_world(int x, int y)
{
    float xOrigin = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth + boxWidth;
    float yOrigin = 10 + boxHeight * height - boxHeight;

    return glm::vec2(xOrigin + x * boxWidth, yOrigin + y * boxHeight);
}
