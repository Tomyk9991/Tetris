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
            i++;
        }
    }


    const bool debug_draw = true;
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

/// Moves the tetromino, if the bounds are not exceeded. In case the an other tetromino is hit
/// or the bottom is reached, this function returns true
bool Tetromino::move(const glm::vec2& offset, const ofColor* field, int fieldHeight)
{
    glm::vec2 newPos = this->local_position + offset;

    bool returnValue = false;
    bool foundInvalid = false;
    int i = 0;
    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            const bool isFilled = this->type & 1 << i;
            if(isFilled)
            {
                int checkingX = newPos.x + x;
                int checkingY = newPos.y + y;
                
                //x out of bounds
                if(checkingX < 0 || checkingX > 8)
                {
                    foundInvalid = true;
                    break;
                }

                //y out of bounds
                if (checkingY > -1)
                {
                    returnValue = true;
                    foundInvalid = true;
                    break;
                }
            }
            
            i++;
        }
    }


    if (!foundInvalid)
    {
        i = 4;
        glm::vec2 pos = newPos;
        pos.y = -pos.y;
        pos.y -= 2;
        std::cout << "x: " << pos.x << " y: " << pos.y << std::endl;
        
        for (int y = 0; y < 2; ++y)
        {
            for (int x = 0; x < 4; ++x)
            {
                //i = erst [4 - 7] dann [0 - 3]
                const bool isFilled = this->type & 1 << i;
                if(isFilled)
                {
                    //Field busy
                    
                    if (field[((int)pos.x + x) * fieldHeight + ((int)pos.y + y)] != ofColor(0, 0, 0, 0))
                    {
                        returnValue = true;
                        foundInvalid = true;
                        break;
                    }
                }

                i++;
                i %= 8;
            }

            if(returnValue) break;
        }
    }

    
    if(foundInvalid == false)
        this->local_position = newPos;
            
    return returnValue;
}

const glm::vec2& Tetromino::get_position() const
{
    return this->local_position;
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
    tetromino.set_position(TetrisField::width / 2, TetrisField::height - 2);
}
