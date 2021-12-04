#ifndef COMMON_H
#define COMMON_H

#include <filesystem>
#include <iostream>
#include <span>
#include <vector>

inline std::filesystem::path fileName(int argc, char **argv)
{
  auto args = std::span(argv, size_t(argc));
  std::filesystem::path file = std::string(args[0]) + ".txt";
  file = file.filename();
  if (args.size() >= 2) {
    file = args[1];
  }
  return file;
}

template <typename T>
int testPart(std::function<int(const std::vector<T> &)> function,
             const std::vector<T> &inputs,
             int target,
             int part)
{
  using namespace std::string_literals;

  const int result = function(inputs);
  const bool ok = target == result;
  const std::string op = ok ? " == "s : " != "s;
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
  using namespace std::string_literals;

  const int result = function(parameters, inputs);
  const bool ok = target == result;
  const std::string op = ok ? " == "s : " != "s;
  std::cout << "Part " << part << ": " << target << op << result << std::endl;
  if (!ok) {
    return part;
  }
  return 0;
}

#endif
