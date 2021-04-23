#pragma once
#include <glm/vec2.hpp>

#include "BlockType.h"
#include "ofColor.h"

class Tetromino
{
public:
    void render();
    const ofColor& get_color() const;
    void set_color(const ofColor& color);
    void set_block_type(BlockType type);
    void move_down();
    BlockType get_block_type() const;
    static void generate_new_random(Tetromino& tetromino);
private:
    BlockType type = None;
    ofColor color;
    glm::vec2 local_position = {0, 0};
};
