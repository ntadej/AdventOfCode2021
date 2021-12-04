#include <fstream>
#include <iostream>
#include <vector>

#include "common.h"

int part_one(const std::vector<std::pair<std::string, int>> &inputs)
{
  int position{};
  int depth{};
  for (const std::pair<std::string, int> &step : inputs) {
    if (step.first == "forward") {
      position += step.second;
    } else if (step.first == "down") {
      depth += step.second;
    } else if (step.first == "up") {
      depth -= step.second;
      if (depth < 0) {
        depth = 0;
      }
    }
  }
  return position * depth;
}

int part_two(const std::vector<std::pair<std::string, int>> &inputs)
{
  int position{};
  int aim{};
  int depth{};
  for (const std::pair<std::string, int> &step : inputs) {
    if (step.first == "forward") {
      position += step.second;
      depth += aim * step.second;
      if (depth < 0) {
        depth = 0;
      }
    } else if (step.first == "down") {
      aim += step.second;
    } else if (step.first == "up") {
      aim -= step.second;
    }
  }
  return position * depth;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::vector<std::pair<std::string, int>> inputs;
  do {
    std::string string{};
    int number{};
    input_file >> string >> number;
    inputs.emplace_back(string, number);
  } while (!input_file.eof());

  const int one = testPart<std::pair<std::string, int>>(part_one, inputs, 2027977, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::pair<std::string, int>>(part_two, inputs, 1903644897, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
