#ifndef POSITION_H
#define POSITION_H
    
struct Position
{
    // A square on the board
    // x = column (0 = 'a' ... 7 = 'h'),
    // y = row (0 = rank 1 ... 7 = rank 8). ex: b1 = {1, 0}.
    // Board stores pieces as squares[y][x].
    int x;
    int y;

    bool operator==(const Position &other) const
    {
        return (x == other.x && y == other.y);
    }
     bool operator!=(const Position &other) const
    {
        return !(*this == other);
    }
     bool validPos() const//has to be in the bounds of the board
    {
        return (x >= 0 && y >=0 && x < 8 && y < 8);
    }
};

#endif