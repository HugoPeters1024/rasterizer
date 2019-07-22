#ifndef UTILS_H
#define UTILS_H

#define PI 3.141592536

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

#endif
