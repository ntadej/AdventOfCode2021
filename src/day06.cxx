#include <numeric>
#include <vector>

#include "common.h"

uint64_t solution(size_t days,
                  const std::vector<int> &inputs)
{
  constexpr int bins{9};
  std::vector<uint64_t> states(bins, 0);
  for (int state : inputs) {
    states[state]++;
  }
  for (size_t d{}; d < days; d++) {
    std::rotate(states.begin(), states.begin() + 1, states.end());
    states[6] += states[8];
  }
  return std::accumulate(states.begin(), states.end(), 0ULL);
}

uint64_t part_one(const std::vector<int> &inputs)
{
  return solution(80, inputs);
}

uint64_t part_two(const std::vector<int> &inputs)
{
  return solution(256, inputs);
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
    char comma{};
    input_file >> number;
    inputs.emplace_back(number);
    input_file >> comma;
  } while (!input_file.eof());

  const int one = testPart<std::vector<int>, uint64_t>(part_one, inputs, 394994, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<int>, uint64_t>(part_two, inputs, 1765974267455, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
