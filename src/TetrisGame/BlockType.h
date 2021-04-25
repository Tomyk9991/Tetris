#pragma once

enum BlockType
{
    Longi = 0b0000111100000000,
    L = 0b1000111000000000,
    LInv = 0b0001011100000000,
    Quad = 0b0011001100000000,
    Stair = 0b0011011000000000,
    T = 0b0010011100000000,
    StairInv = 0b0110001100000000,

    Longi90 = 0b1000100010001000,
    L90 = 0b0011000100010000,
    LInv90 = 0b001000100011,
    Quad90 = 0b0011001100000000,
    Stair90 = 0b0010011000010000,
    T90 = 0b0010001100100000,
    StairInv90 = 0b0001000100100010,

    None = 0
};
