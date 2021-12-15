#include <numeric>
#include <vector>

#include "common.h"

uint64_t part_one(const std::vector<int> &inputs)
{
  std::vector<int> positions{inputs};
  std::sort(positions.begin(), positions.end());

  const int median = positions[positions.size() / 2];
  return std::accumulate(positions.begin(), positions.end(), 0, [median](int a, int b) {
    return a + std::abs(b - median);
  });
}

uint64_t part_two(const std::vector<int> &inputs)
{
  std::vector<int> positions{inputs};
  std::sort(positions.begin(), positions.end());

  auto accumulator = [positions](int solution) {
    return std::accumulate(positions.begin(), positions.end(), 0, [solution](int a, int b) {
      const int n = std::abs(b - solution);
      return a + (n * (n + 1) / 2);
    });
  };

  const int median = positions[positions.size() / 2];
  int solution{median};
  while (accumulator(solution + 1) < accumulator(solution)) {
    solution++;
  }
  if (solution == median) {
    while (accumulator(solution - 1) < accumulator(solution)) {
      solution--;
    }
  }

  return accumulator(solution);
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

  const int one = testPart<std::vector<int>, uint64_t>(part_one, inputs, 333755, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<int>, uint64_t>(part_two, inputs, 94017638, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
