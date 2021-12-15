#ifndef COMMON_H
#define COMMON_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

inline std::filesystem::path fileName(int argc, char **argv)
{
  std::filesystem::path file = std::string(argv[0]) + ".txt"; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  file = file.filename();
  if (argc >= 2) {
    file = argv[1]; //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
  return file;
}

inline std::vector<std::string> split(const std::string &string,
                                      const std::string &delimiter)
{
  std::string::size_type b = 0;
  std::vector<std::string> result;

  while ((b = string.find_first_not_of(delimiter, b)) != std::string::npos) {
    std::string::size_type e = string.find_first_of(delimiter, b);
    result.push_back(string.substr(b, e - b));
    b = e;
  }
  return result;
}

inline std::vector<int> splitNumber(const std::string &string,
                                    const std::string &delimiter)
{
  std::string::size_type b = 0;
  std::vector<int> result;

  while ((b = string.find_first_not_of(delimiter, b)) != std::string::npos) {
    std::string::size_type e = string.find_first_of(delimiter, b);
    result.push_back(std::stoi(string.substr(b, e - b)));
    b = e;
  }
  return result;
}

template <typename T, typename U>
int testPart(std::function<U(const T &)> function,
             const T &inputs,
             U target,
             int part)
{
  using namespace std::string_literals;

  const U result = function(inputs);
  const bool ok = target == result;
  const std::string op = ok ? " == "s : " != "s;
  std::cout << "Part " << part << ": " << target << op << result << std::endl;
  if (!ok) {
    return part;
  }
  return 0;
}

template <typename T, typename U, typename V>
int testPart(std::function<V(const T &, const U &)> function,
             const T &inputs1,
             const U &inputs2,
             V target,
             int part)
{
  using namespace std::string_literals;

  const V result = function(inputs1, inputs2);
  const bool ok = target == result;
  const std::string op = ok ? " == "s : " != "s;
  std::cout << "Part " << part << ": " << target << op << result << std::endl;
  if (!ok) {
    return part;
  }
  return 0;
}

namespace std
{

template <typename T1, typename T2>
struct less<std::pair<T1, T2>> {
  bool operator()(const std::pair<T1, T2> &l, const std::pair<T1, T2> &r) const
  {
    if (l.first == r.first) {
      return l.second > r.second;
    }

    return l.first < r.first;
  }
};

} // namespace std

#endif
