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
    for (int y = 0; y < 4; ++y)
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
    if (debug_draw)
    {
        ofSetColor(ofColor::red);

        ofDrawLine(gx, gy, gx + TetrisField::boxWidth * 4, gy);
        ofDrawLine(gx + TetrisField::boxWidth * 4, gy, gx + TetrisField::boxWidth * 4, gy + TetrisField::boxHeight * 4);
        ofDrawLine(gx + TetrisField::boxWidth * 4, gy + TetrisField::boxHeight * 4, gx,
                   gy + TetrisField::boxHeight * 4);
        ofDrawLine(gx, gy + TetrisField::boxHeight * 4, gx, gy);
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

const MoveResult& Tetromino::isValidMove(const glm::vec2& offset, const ofColor* field, int fieldHeight) const
{
    glm::vec2 newPos = this->local_position + offset;

    bool returnValue = false;
    bool foundInvalid = false;
    int i = 0;
    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            const bool isFilled = this->type & 1 << i;
            if (isFilled)
            {
                int checkingX = newPos.x + x;
                int checkingY = newPos.y + y;

                //x out of bounds
                if (checkingX < 0 || checkingX > 8)
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
        i = 0;
        glm::vec2 pos = newPos;
        pos.y = -pos.y;
        pos.y -= 4;

        for (int y = 3; y >= 0; --y)
        {
            for (int x = 0; x < 4; ++x)
            {
                const bool isFilled = this->type & 1 << i;
                if (isFilled)
                {
                    //Field busy
                    if (field[(static_cast<int>(pos.x) + x) * fieldHeight + (static_cast<int>(pos.y) + y)] != ofColor(
                        0, 0, 0, 0))
                    {
                        returnValue = true;
                        foundInvalid = true;
                        break;
                    }
                }

                i++;
            }

            if (returnValue) break;
        }
    }

    return MoveResult(returnValue, foundInvalid, newPos);
}


/// Moves the tetromino, if the bounds are not exceeded. In case the an other tetromino is hit
/// or the bottom is reached, this function returns true
bool Tetromino::move(const glm::vec2& offset, const ofColor* field, int fieldHeight)
{
    const MoveResult& result = this->isValidMove(offset, field, fieldHeight);


    if (result.foundInvalid == false)
        this->local_position = result.newPos;

    return result.canMove;
}

const glm::vec2& Tetromino::get_position() const
{
    return this->local_position;
}

void Tetromino::rotate_tetromino(Tetromino& tetromino)
{
    static const std::map<int, int> intMapping = {
        {0, Longi},
        {1, L},
        {2, LInv},
        {3, Quad},
        {4, Stair},
        {5, T},
        {6, StairInv},

        {7,  Longi90},
        {8,  L90},
        {9,  LInv90},
        {10, Quad90},
        {11, Stair90},
        {12, T90},
        {13, StairInv90},
    };

    static const std::map<int, int> blockMapping = {
        {Longi, 0},
        {L, 1},
        {LInv, 2},
        {Quad, 3},
        {Stair, 4},
        {T, 5},
        {StairInv, 6},

        {Longi90, 7},
        {L90, 8},
        {LInv90, 9},
        {Quad90, 10},
        {Stair90, 11},
        {T90, 12},
        {StairInv90, 13},
};
    int currentIndex = blockMapping.at(tetromino.get_block_type());

    currentIndex = currentIndex >= 7 ? currentIndex - 7 : currentIndex + 7;

    tetromino.set_block_type(static_cast<BlockType>(intMapping.at(currentIndex)));
}

void Tetromino::generate_new_random(Tetromino& tetromino)
{
    static const std::map<int, int> intMapping = {
        {0, Longi},
        {1, L},
        {2, LInv},
        {3, Quad},
        {4, Stair},
        {5, T},
        {6, StairInv}
    };

    BlockType randomBlock = static_cast<BlockType>(intMapping.at(rand() % 7));
    ofColor randomColor = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));

    tetromino.set_block_type(randomBlock);
    tetromino.set_color(randomColor);
    tetromino.set_position(TetrisField::width / 2, TetrisField::height - 2);
}
