#pragma once
#include <ostream>
#include <glm/vec2.hpp>

struct MoveResult
{
    MoveResult(bool can_move, bool found_invalid, const glm::vec2& new_pos)
        : canMove(can_move),
          foundInvalid(found_invalid),
          newPos(new_pos)
    {
        
    }

    friend std::ostream& operator<<(std::ostream& os, const MoveResult& move_result);

    const bool canMove = false;
    const bool foundInvalid = false;
    const glm::vec2 newPos;
};
