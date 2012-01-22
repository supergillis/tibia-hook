#include "position.h"

std::ostream& operator<<(std::ostream& out, const Position& position)
{
	out << "(" << position.x() << ", " << position.y() << ", " << (int)position.z() << ")";
	return out;
}

Position::Position(uint16_t x, uint16_t y, uint8_t z): _x(x), _y(y), _z(z)
{
}
