#ifndef UTILS_H
#define UTILS_H

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

#endif