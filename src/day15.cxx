#include <vector>

#include "common.h"

size_t risk(const std::vector<std::vector<int>> &input)
{
  const size_t xSize = input.size();
  const size_t ySize = input.back().size();

  const std::vector<std::pair<int, int>> dr{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  std::vector<std::vector<size_t>> distance(xSize, std::vector<size_t>(ySize, 0));
  std::vector<std::vector<std::pair<size_t, size_t>>> queue(xSize * 10, std::vector<std::pair<size_t, size_t>>());
  queue[0].emplace_back(0, 0);

  size_t v{};
  while (distance[xSize - 1][ySize - 1] == 0) {
    for (const auto &[x, y] : queue[v]) {
      if (v > distance[x][y]) {
        continue;
      }

      for (const auto &[dx, dy] : dr) {
        if ((x == 0 && dx == -1) || (x == xSize - 1 && dx == 1) || (y == 0 && dy == -1) || (y == ySize - 1 && dy == 1)) {
          continue;
        }

        if (distance[x + dx][y + dy] == 0) {
          const size_t dv = input[x + dx][y + dy];
          distance[x + dx][y + dy] = v + dv;
          queue[v + dv].emplace_back(x + dx, y + dy);
        }
      }
    }
    v++;
  }

  return distance[xSize - 1][ySize - 1];
}

size_t part_one(const std::vector<std::vector<int>> &input)
{
  return risk(input);
}

size_t part_two(const std::vector<std::vector<int>> &input)
{
  // pre-process
  std::vector<std::vector<int>> cache;
  for (size_t i{}; i < 5; i++) {
    for (const std::vector<int> &line : input) {
      std::vector<int> row;
      row.reserve(line.size() * 5);
      for (size_t j{}; j < 5; j++) {
        for (const int v : line) {
          const int n = (v + static_cast<int>(i) + static_cast<int>(j)) % 9;
          row.push_back(n == 0 ? 9 : n);
        }
      }
      cache.emplace_back(std::move(row));
    }
  }

  return risk(cache);
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::vector<std::vector<int>> input;
  for (std::string line; std::getline(input_file, line);) {
    std::vector<int> l;
    l.reserve(line.size());
    for (size_t i{}; i < line.size(); i++) {
      l.push_back(std::stoi(line.substr(i, 1)));
    }
    input.emplace_back(std::move(l));
  }

  const int one = testPart<std::vector<std::vector<int>>, size_t>(part_one, input, 583, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::vector<int>>, size_t>(part_two, input, 2927, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
