#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

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
  std::filesystem::path file = std::string(argv[0]) + ".txt";
  file = file.filename();
  if (argc >= 2) {
    file = argv[1];
  }

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

  int target_one{2027977};
  int result_one = part_one(inputs);
  bool ok_one = target_one == result_one;
  std::cout << "Part 1: " << target_one << (ok_one ? " == " : " != ") << result_one << std::endl;
  if (!ok_one) {
    return 1;
  }

  int target_two{1903644897};
  int result_two = part_two(inputs);
  bool ok_two = target_two == result_two;
  std::cout << "Part 2: " << target_two << (ok_two ? " == " : " != ") << result_two << std::endl;
  if (!ok_two) {
    return 2;
  }

  return 0;
}
