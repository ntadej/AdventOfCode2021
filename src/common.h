#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>

template <typename T>
int testPart(std::function<int(const std::vector<T> &)> function,
             const std::vector<T> &inputs,
             int target,
             int part)
{
  int result = function(inputs);
  bool ok = target == result;
  const char *op = ok == true ? " == " : " != ";
  std::cout << "Part " << part << ": " << target << op << result << std::endl;
  if (!ok) {
    return part;
  }
  return 0;
}

template <typename T, typename U>
int testPart(std::function<int(const std::vector<T> &, const std::vector<U> &)> function,
             const std::vector<T> &parameters,
             const std::vector<U> &inputs,
             int target,
             int part)
{
  int result = function(parameters, inputs);
  bool ok = target == result;
  const char *op = ok == true ? " == " : " != ";
  std::cout << "Part " << part << ": " << target << op << result << std::endl;
  if (!ok) {
    return part;
  }
  return 0;
}

#endif
