#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int part_one(const std::vector<int> &inputs)
{
  int previous{inputs.front()};
  int count{};
  for (int number : inputs) {
    if (number > previous) {
      count++;
    }
    previous = number;
  }
  return count;
}

int part_two(const std::vector<int> &inputs)
{
  int previous{inputs[0] + inputs[1] + inputs[2]};
  int count{};
  for (size_t i{3}; i < inputs.size(); i++) {
    if (previous + inputs[i] - inputs[i - 3] > previous) {
      count++;
    }
    previous = previous + inputs[i] - inputs[i - 3];
  }
  return count;
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
  std::vector<int> inputs;
  do {
    int number{};
    input_file >> number;
    inputs.emplace_back(number);
  } while (!input_file.eof());

  int target_one{1466};
  int result_one = part_one(inputs);
  bool ok_one = target_one == result_one;
  std::cout << "Part 1: " << target_one << (ok_one ? " == " : " != ") << result_one << std::endl;
  if (!ok_one) {
    return 1;
  }

  int target_two{1491};
  int result_two = part_two(inputs);
  bool ok_two = target_two == result_two;
  std::cout << "Part 2: " << target_two << (ok_two ? " == " : " != ") << result_two << std::endl;
  if (!ok_two) {
    return 2;
  }

  return 0;
}
