#ifndef COLOR_HPP
#define COLOR_HPP
#include <ostream>
enum class Color { EMPTY, BLACK, RED};
std::ostream& operator<<(std::ostream& os, Color color);
#endif