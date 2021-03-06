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
    font.load("FiraCode-Bold.ttf", 20);

    this->previousColor =  ofColor::fromHsb(ofRandom(0., 255.), 255., ofRandom(0., 255.));
    this->targetColor =  ofColor::fromHsb(ofRandom(0., 255.), 255., ofRandom(0., 255.));
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

void TetrisField::check_rows()
{
    const ofColor nothing(0, 0, 0, 0);
    for (int y = height - 1; y >= 0; --y)
    {
        bool reducible = true;
        
        for (int x = 0; x < width - 1; ++x)
        {
            if (this->gameField[x * height + y] == nothing)
            {
                reducible = false;
                break;
            }
        }


        if (reducible == true)
        {
            reduce_row(y);
        }
    }
}

void TetrisField::reduce_row(int targetRow)
{
    score++;
    for (int y = targetRow; y < height - 1; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            this->gameField[x * height + y].set(this->gameField[x * height + (y + 1)]);
            this->gameField[x * height + (y + 1)].set(0, 0, 0, 0);
        }
    }

    for (int x = 0; x < width; ++x)
    {
        this->gameField[x * height + (height - 1)].set(0, 0, 0, 0);
    }
}

void TetrisField::update()
{
    this->elapsedTime += ofGetLastFrameTime();

    if (this->elapsedTime >= 1.)
    {
        this->previousColor = this->targetColor;
        this->targetColor = ofColor::fromHsb(ofRandom(0., 255.), 255., ofRandom(0., 255.));
        this->elapsedTime = 0.;
    }
    
    if (ofGetElapsedTimef() - lastTime > TimeTick)
    {
        lastTime = ofGetElapsedTimef();

        if (this->currentTetromino.get_block_type() != BlockType::None)
        {
            if (this->currentTetromino.move(glm::vec2(0, 1), this->gameField.data()))
            {
                if (canLose)
                {
                    std::cout << "GAME OVER" << std::endl;
                    gameOver = true;
                    return;
                }
                add_current_tetromino();
                check_rows();
                Tetromino::generate_new_random(this->currentTetromino);
                
                this->canLose = true;
            }
            else
            {
                this->canLose = false;
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

void TetrisField::draw_score()
{
    ofSetColor(previousColor.lerp(this->targetColor, elapsedTime));
    
    char str[17];
    sprintf_s(str, "Score:\n%i", score * 1000);
    font.drawString(str, 10, 50);
}

void TetrisField::draw_game_over()
{
    ofRectMode oldMode = ofGetRectMode();
    ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

    ofSetColor(ofColor::red);
    ofDrawRectRounded(ofGetWidth() / 2, ofGetHeight() / 2, ofGetWidth() / 1.5, ofGetHeight() / 1.5, 10);

    ofSetColor(ofColor::white);
    char str[35];
    sprintf_s(str, "Score:\n%i\nPlay AGAINE (Enter)", score * 1000);

    font.drawString(str, ofGetWidth() / 2 - font.stringWidth(str) / 2,
        ofGetHeight() / 2 - font.stringHeight(str) / 2);
    
    ofSetRectMode(oldMode);
}

void TetrisField::draw()
{
    if(!gameOver)
    {
        draw_game_grid();
        draw_game_bounds(ofColor::darkGray);
        draw_game_field();

        this->currentTetromino.render();
        draw_score();
    }
    else
    {
        draw_game_over();
    }
}

void TetrisField::key_pressed(ofKeyEventArgs& e)
{
    if(!gameOver)
    {
        if (e.key == 'a' || e.key == ofKey::OF_KEY_LEFT)
        {
            this->currentTetromino.move(glm::vec2(-1, 0), this->gameField.data());
        }

        if (e.key == 'd' || e.key == ofKey::OF_KEY_RIGHT)
        {
            this->currentTetromino.move(glm::vec2(1, 0), this->gameField.data());
        }

        if (e.key == 's' || e.key == ofKey::OF_KEY_DOWN)
        {
            if (this->currentTetromino.move(glm::vec2(0, 1), this->gameField.data()))
            {
                add_current_tetromino();
                check_rows();
                Tetromino::generate_new_random(this->currentTetromino);
                canLose = false;
            }
        }

        if (e.key == 'r')
        {
            Tetromino::generate_new_random(this->currentTetromino);
        }

        if (e.key == ' ')
        {
            Tetromino::rotate_tetromino(this->currentTetromino, this->gameField.data());
        }
    }
    else
    {
        if(e.key == OF_KEY_RETURN)
        {
            *this = TetrisField();
        }
    }
}

const glm::vec2& TetrisField::local_to_world(int x, int y)
{
    float xOrigin = ofGetWidth() / 2 - (width + 2) / 2 * boxWidth + boxWidth;
    float yOrigin = 10 + boxHeight * height - boxHeight;

    return glm::vec2(xOrigin + x * boxWidth, yOrigin + y * boxHeight);
}
