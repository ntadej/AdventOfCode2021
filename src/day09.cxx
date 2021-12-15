#include <algorithm>
#include <vector>

#include "common.h"

uint64_t part_one(const std::vector<std::vector<int>> &inputs)
{
  uint64_t count{};
  for (size_t i{}; i < inputs.size(); i++) {
    for (size_t j{}; j < inputs[i].size(); j++) {
      if ((i == 0 || inputs[i][j] < inputs[i - 1][j])
          && (i == inputs.size() - 1 || inputs[i][j] < inputs[i + 1][j])
          && (j == 0 || inputs[i][j] < inputs[i][j - 1])
          && (j == inputs[i].size() - 1 || inputs[i][j] < inputs[i][j + 1])) {
        count += inputs[i][j] + 1;
      }
    }
  }
  return count;
}

uint64_t basin(std::vector<std::vector<int>> &field,
               size_t i,
               size_t j)
{
  if (field[i][j] == 9) {
    return 0;
  }

  field[i][j] = 9;

  uint64_t size{1};
  if (i > 0) {
    size += basin(field, i - 1, j);
  }
  if (j > 0) {
    size += basin(field, i, j - 1);
  }
  if (i != field.size() - 1) {
    size += basin(field, i + 1, j);
  }
  if (j != field[i].size() - 1) {
    size += basin(field, i, j + 1);
  }
  return size;
}

uint64_t part_two(const std::vector<std::vector<int>> &inputs)
{
  std::vector<std::pair<size_t, size_t>> tubes;
  for (size_t i{}; i < inputs.size(); i++) {
    for (size_t j{}; j < inputs[i].size(); j++) {
      if ((i == 0 || inputs[i][j] < inputs[i - 1][j])
          && (i == inputs.size() - 1 || inputs[i][j] < inputs[i + 1][j])
          && (j == 0 || inputs[i][j] < inputs[i][j - 1])
          && (j == inputs[i].size() - 1 || inputs[i][j] < inputs[i][j + 1])) {
        tubes.emplace_back(i, j);
      }
    }
  }

  std::vector<uint64_t> sizes;
  for (const std::pair<size_t, size_t> &pair : tubes) {
    std::vector<std::vector<int>> field = inputs;
    sizes.emplace_back(basin(field, pair.first, pair.second));
  }
  std::sort(sizes.begin(), sizes.end(), std::greater<>());

  return sizes[0] * sizes[1] * sizes[2];
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

  const int one = testPart<std::vector<std::vector<int>>, uint64_t>(part_one, inputs, 436, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::vector<int>>, uint64_t>(part_two, inputs, 1317792, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
