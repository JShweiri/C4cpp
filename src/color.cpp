#include "color.hpp"

std::ostream& operator<<(std::ostream& os, Color color) {
  switch (color) {
    case Color::EMPTY:
      os << "-";
      break;
    case Color::BLACK:
      os << "X";
      break;
    case Color::RED:
      os << "O";
      break;
  }
  return os;
}