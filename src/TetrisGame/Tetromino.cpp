#include <map>

#include "Tetromino.h"
#include "ofGraphics.h"
#include "ofMath.h"
#include "TetrisField.h"

void Tetromino::render()
{
    glm::vec2 g = TetrisField::local_to_world(this->local_position.x, this->local_position.y);

    
    float gx = g.x;
    float gy = g.y;
    
    int i = 0;
    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            const bool isFilled = this->type & 1 << i;
            ofSetColor(isFilled ? this->color : ofColor(0, 0, 0, 0));
            ofDrawRectangle(gx + x * TetrisField::boxWidth, gy + y * TetrisField::boxHeight, TetrisField::boxWidth,
                            TetrisField::boxHeight);

            // ofSetColor(ofColor::white);
            // ofDrawBitmapString(std::to_string(i),
            //                    gx + x * TetrisField::boxWidth + TetrisField::boxHeight / 2,
            //                    gy + y * TetrisField::boxWidth + TetrisField::boxHeight / 2
            // );
            i++;
        }
    }


    const bool debug_draw = false;
    if(debug_draw)
    {
        ofSetColor(ofColor::red);

        ofDrawLine(gx, gy, gx + TetrisField::boxWidth * 4, gy);
        ofDrawLine(gx + TetrisField::boxWidth * 4, gy, gx + TetrisField::boxWidth * 4, gy + TetrisField::boxHeight * 2);
        ofDrawLine(gx + TetrisField::boxWidth * 4, gy + TetrisField::boxHeight * 2, gx, gy + TetrisField::boxHeight * 2);
        ofDrawLine(gx, gy + TetrisField::boxHeight * 2, gx, gy);
    }
}

const ofColor& Tetromino::get_color() const
{
    return this->color;
}

void Tetromino::set_color(const ofColor& color)
{
    this->color = color;
}

void Tetromino::set_block_type(BlockType type)
{
    this->type = type;
}

BlockType Tetromino::get_block_type() const
{
    return this->type;
}

void Tetromino::set_position(int x, int y)
{
    this->local_position.x = x;
    this->local_position.y = -y;
    
}

void Tetromino::move(const glm::vec2& offset)
{
    this->local_position += offset;
}

void Tetromino::generate_new_random(Tetromino& tetromino)
{
    static const std::map<int, const char*> enumStrings{
        {Longi, "BlockType::Longi"},
        {L, "BlockType::L"},
        {LInv, "BlockType::LInv"},
        {Quad, "BlockType::Quad"},
        {Stair, "BlockType::Stair"},
        {T, "BlockType::T"},
        {StairInv, "BlockType::StairInv"},
    };

    static const std::map<int, int> intMapping = {
        {0, 0b00001111},
        {1, 0b10001110},
        {2, 0b00010111},
        {3, 0b00110011},
        {4, 0b00110110},
        {5, 0b00100111},
        {6, 0b01100011},
    };

    BlockType randomBlock = static_cast<BlockType>(intMapping.at(rand() % 7));
    ofColor randomColor = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));

    tetromino.set_block_type(randomBlock);
    tetromino.set_color(randomColor);
    tetromino.set_position(TetrisField::width / 2, TetrisField::height);
}
