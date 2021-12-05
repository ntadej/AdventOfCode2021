#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "common.h"

class Line
{
public:
  Line(int x1,
       int y1,
       int x2,
       int y2);

  [[nodiscard]] std::vector<std::pair<int, int>> horizontalSection() const;
  [[nodiscard]] std::vector<std::pair<int, int>> diagonalSection() const;
  [[nodiscard]] int maxCoordinate() const;

private:
  std::pair<int, int> _start;
  std::pair<int, int> _end;
};

Line::Line(int x1,
           int y1, // NOLINT(bugprone-easily-swappable-parameters)
           int x2,
           int y2)
{
  _start = std::make_pair(x1, y1);
  _end = std::make_pair(x2, y2);
}

std::vector<std::pair<int, int>> Line::horizontalSection() const
{
  std::vector<std::pair<int, int>> coordinates;
  if (_start.first == _end.first) {
    int step = _start.second < _end.second ? 1 : -1;
    for (int i{_start.second}; i != _end.second + step; i += step) {
      coordinates.emplace_back(_start.first, i);
    }
  } else if (_start.second == _end.second) {
    int step = _start.first < _end.first ? 1 : -1;
    for (int i{_start.first}; i != _end.first + step; i += step) {
      coordinates.emplace_back(i, _start.second);
    }
  }
  return coordinates;
}

std::vector<std::pair<int, int>> Line::diagonalSection() const
{
  std::vector<std::pair<int, int>> coordinates;
  if (std::abs(_start.first - _end.first) == std::abs(_start.second - _end.second)) {
    int first = _start.first < _end.first ? 1 : -1;
    int second = _start.second < _end.second ? 1 : -1;
    for (int i{_start.first}, j{_start.second}; i != _end.first + first; i += first, j += second) {
      coordinates.emplace_back(i, j);
    }
  }
  return coordinates;
}

int Line::maxCoordinate() const
{
  return std::max(std::max(_start.first, _start.second), std::max(_end.first, _end.second));
}

std::vector<int> stringSplit(const std::string &string,
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

int part_one(const std::vector<int> &parameters,
             const std::vector<Line> &inputs)
{
  std::vector<std::vector<int>> matrix(parameters.front(), std::vector<int>(parameters.front(), 0));
  for (const Line &line : inputs) {
    for (const std::pair<int, int> &coordinate : line.horizontalSection()) {
      matrix[coordinate.first][coordinate.second]++;
    }
  }

  int count{};
  for (int i{}; i < parameters.front(); i++) {
    for (int j{}; j < parameters.front(); j++) {
      if (matrix[i][j] >= 2) {
        count++;
      }
    }
  }
  return count;
}

int part_two(const std::vector<int> &parameters,
             const std::vector<Line> &inputs)
{
  std::vector<std::vector<int>> matrix(parameters.front(), std::vector<int>(parameters.front(), 0));
  for (const Line &line : inputs) {
    for (const std::pair<int, int> &coordinate : line.horizontalSection()) {
      matrix[coordinate.first][coordinate.second]++;
    }
    for (const std::pair<int, int> &coordinate : line.diagonalSection()) {
      matrix[coordinate.first][coordinate.second]++;
    }
  }

  int count{};
  for (int i{}; i < parameters.front(); i++) {
    for (int j{}; j < parameters.front(); j++) {
      if (matrix[i][j] >= 2) {
        count++;
      }
    }
  }
  return count;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::vector<Line> inputs;
  int max{};
  while (!input_file.eof()) {
    std::string buffer;
    std::vector<int> bufferSplit;
    input_file >> buffer;
    if (buffer.empty()) {
      break;
    }
    bufferSplit = stringSplit(buffer, ",");
    int x1 = bufferSplit[0];
    int y1 = bufferSplit[1];
    input_file >> buffer >> buffer;
    bufferSplit = stringSplit(buffer, ",");
    int x2 = bufferSplit[0];
    int y2 = bufferSplit[1];

    inputs.emplace_back(x1, y1, x2, y2);
    max = std::max(max, inputs.back().maxCoordinate());
  }
  std::vector<int> parameters = {max};

  const int one = testPart<int, Line>(part_one, parameters, inputs, 5084, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<int, Line>(part_two, parameters, inputs, 17882, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
