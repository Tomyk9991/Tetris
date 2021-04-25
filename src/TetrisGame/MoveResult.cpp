#include "MoveResult.h"

std::ostream& operator<<(std::ostream& os, const MoveResult& move_result)
{
    os << "Can move: "  << move_result.canMove << " Invalid move: " << move_result.foundInvalid << " New Position: (x: " << move_result.newPos.x << " y: " << move_result.newPos.y << ")";
    return os;
}