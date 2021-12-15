#include <vector>

#include "common.h"

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
  const std::filesystem::path file = fileName(argc, argv);

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

  const int one = testPart<std::vector<int>, int>(part_one, inputs, 1466, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<int>, int>(part_two, inputs, 1491, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
