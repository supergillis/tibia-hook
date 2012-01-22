#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>
#include <iostream>

class Position
{
	friend std::ostream& operator<<(std::ostream& out, const Position& position);
	public:
		Position(uint16_t = 0, uint16_t = 0, uint8_t = 0);
		uint16_t x() const { return _x; }
		uint16_t y() const { return _y; }
		uint8_t z() const { return _z; }
	protected:
		uint16_t _x;
		uint16_t _y;
		uint8_t _z;
};

#endif
