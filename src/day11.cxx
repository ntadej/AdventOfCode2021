#include <vector>

#include "common.h"

constexpr size_t steps{100};
constexpr size_t size{10};

uint64_t energyUp(std::vector<std::vector<int>> &grid,
                  size_t i,
                  size_t j);

uint64_t flash(std::vector<std::vector<int>> &grid,
               size_t i,
               size_t j)
{
  uint64_t count{1};
  grid[i][j] = 0;

  if (i > 0 && j > 0) {
    count += energyUp(grid, i - 1, j - 1);
  }
  if (i < size - 1 && j < size - 1) {
    count += energyUp(grid, i + 1, j + 1);
  }
  if (i > 0) {
    count += energyUp(grid, i - 1, j);
  }
  if (j > 0) {
    count += energyUp(grid, i, j - 1);
  }
  if (i < size - 1) {
    count += energyUp(grid, i + 1, j);
  }
  if (j < size - 1) {
    count += energyUp(grid, i, j + 1);
  }
  if (i > 0 && j < size - 1) {
    count += energyUp(grid, i - 1, j + 1);
  }
  if (i < size - 1 && j > 0) {
    count += energyUp(grid, i + 1, j - 1);
  }
  return count;
}

uint64_t energyUp(std::vector<std::vector<int>> &grid,
                  size_t i,
                  size_t j)
{
  if (grid[i][j] == 0) {
    return 0;
  }

  grid[i][j]++;

  if (grid[i][j] > 9) {
    return flash(grid, i, j);
  }

  return 0;
}

uint64_t part_one(const std::vector<std::vector<int>> &inputs)
{
  uint64_t count{};
  std::vector<std::vector<int>> grid{inputs};

  for (size_t s{}; s < steps; s++) {
    // increase by 1
    for (size_t i{}; i < size; i++) {
      for (size_t j{}; j < size; j++) {
        grid[i][j]++;
      }
    }
    // flash
    for (size_t i{}; i < size; i++) {
      for (size_t j{}; j < size; j++) {
        if (grid[i][j] > 9) {
          count += flash(grid, i, j);
        }
      }
    }
    // // debug
    // for (size_t i{}; i < size; i++) {
    //   for (size_t j{}; j < size; j++) {
    //     std::cout << grid[i][j];
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << std::endl;
  }
  return count;
}

uint64_t part_two(const std::vector<std::vector<int>> &inputs)
{
  std::vector<std::vector<int>> grid{inputs};
  for (size_t s{}; s < UINT64_MAX; s++) {
    uint64_t count{};
    // increase by 1
    for (size_t i{}; i < size; i++) {
      for (size_t j{}; j < size; j++) {
        grid[i][j]++;
      }
    }
    // flash
    for (size_t i{}; i < size; i++) {
      for (size_t j{}; j < size; j++) {
        if (grid[i][j] > 9) {
          count += flash(grid, i, j);
        }
      }
    }
    // // debug
    // for (size_t i{}; i < size; i++) {
    //   for (size_t j{}; j < size; j++) {
    //     std::cout << grid[i][j];
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << std::endl;
    if (count == size * size) {
      return s + 1;
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }
  std::vector<std::vector<int>> inputs;
  for (std::string line; std::getline(input_file, line);) {
    std::vector<int> row;
    for (char c : line) {
      row.emplace_back(c - '0');
    }
    inputs.emplace_back(std::move(row));
  }

  const int one = testPart<std::vector<std::vector<int>>, uint64_t>(part_one, inputs, 1721, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::vector<int>>, uint64_t>(part_two, inputs, 298, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
