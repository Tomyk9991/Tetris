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


    const bool debug_draw = false;
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

MoveResult Tetromino::isValidMove(const glm::vec2& offset, const ofColor* field, int fieldHeight) const
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

void Tetromino::rotate_tetromino(Tetromino& tetromino, ofColor* field)
{
    static const BlockType intMapping[28]
    {
        Longi,
        L,
        LInv,
        Quad,
        Stair,
        T,
        StairInv,

        Longi90,
        L90,
        LInv90,
        Quad90,
        Stair90,
        T90,
        StairInv90,

        Longi180,
        L180,
        LInv180,
        Quad180,
        Stair180,
        T180,
        StairInv180,

        Longi270,
        L270,
        LInv270,
        Quad270,
        Stair270,
        T270,
        StairInv270
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

        {Longi180, 14},
        {L180, 15},
        {LInv180, 16},
        {Quad180, 17},
        {Stair180, 18},
        {T180, 19},
        {StairInv180, 20},

        {Longi270, 21},
        {L270, 22},
        {LInv270, 23},
        {Quad270, 24},
        {Stair270, 25},
        {T270, 26},
        {StairInv270, 27},
    };

    BlockType oldBlockType = tetromino.get_block_type();
    int currentIndex = blockMapping.at(oldBlockType);

    currentIndex += 7;
    currentIndex = currentIndex % 28;

    tetromino.set_block_type(intMapping[currentIndex]);
    const MoveResult& result = tetromino.isValidMove(glm::vec2(0, 0), field);


    if (result.foundInvalid == true)
    {
        tetromino.set_block_type(oldBlockType);
    }
}

void Tetromino::generate_new_random(Tetromino& tetromino)
{
    static BlockType intMapping[7]
    {
        Longi, L, LInv, Quad, Stair, T, Stair90
    };

    BlockType randomBlock = static_cast<BlockType>(intMapping[rand() % 7]);
    ofColor randomColor = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));

    tetromino.set_block_type(randomBlock);
    tetromino.set_color(randomColor);
    tetromino.set_position(TetrisField::width / 2 - 2, TetrisField::height);
}
