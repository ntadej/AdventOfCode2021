#include <set>
#include <vector>

#include "common.h"

size_t part_one(const std::set<std::pair<int, int>> &input,
                const std::vector<std::pair<int, int>> &folds)
{
  std::set<std::pair<int, int>> dots = input;
  std::pair<int, int> fold = folds.front();

  for (auto it = dots.begin(); it != dots.end();) {
    if (fold.first > 0 && it->first > fold.first) {
      std::pair<int, int> dot = *it;
      dot.first = 2 * fold.first - dot.first;
      it = dots.erase(it);
      dots.insert(std::move(dot));
    } else if (fold.second > 0 && it->second > fold.second) {
      std::pair<int, int> dot = *it;
      dot.second = 2 * fold.second - dot.second;
      it = dots.erase(it);
      dots.insert(std::move(dot));
    } else {
      ++it;
    }
  }

  return dots.size();
}

size_t part_two(const std::set<std::pair<int, int>> &input,
                const std::vector<std::pair<int, int>> &folds)
{
  std::set<std::pair<int, int>> dots = input;

  for (const std::pair<int, int> &fold : folds) {
    for (auto it = dots.begin(); it != dots.end();) {
      if (fold.first > 0 && it->first > fold.first) {
        std::pair<int, int> dot = *it;
        dot.first = 2 * fold.first - dot.first;
        it = dots.erase(it);
        dots.insert(std::move(dot));
      } else if (fold.second > 0 && it->second > fold.second) {
        std::pair<int, int> dot = *it;
        dot.second = 2 * fold.second - dot.second;
        it = dots.erase(it);
        dots.insert(std::move(dot));
      } else {
        ++it;
      }
    }
  }

  size_t s = std::max(dots.rbegin()->first, dots.rbegin()->second) + 1;
  std::vector<std::vector<char>> grid = std::vector<std::vector<char>>(s, std::vector<char>(s, '.'));
  for (const std::pair<int, int> &dot : dots) {
    grid[dot.first][dot.second] = '#';
  }

  for (size_t i{}; i < s; i++) {
    for (size_t j{}; j < s; j++) {
      std::cout << grid[j][i];
    }
    std::cout << std::endl;
  }

  return s;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::set<std::pair<int, int>> dots;
  std::vector<std::pair<int, int>> folds;
  for (std::string line; std::getline(input_file, line);) {
    if (line.empty()) {
      continue;
    }

    if (line.find("fold") != std::string::npos) {
      std::vector<std::string> foldSplit = split(line, "=");
      if (foldSplit[0] == "fold along x") {
        folds.emplace_back(std::stoi(foldSplit[1]), 0);
      } else {
        folds.emplace_back(0, std::stoi(foldSplit[1]));
      }
    } else {
      std::vector<std::string> dotSplit = split(line, ",");
      dots.insert(std::make_pair(std::stoi(dotSplit[0]), std::stoi(dotSplit[1])));
    }
  }

  const int one = testPart<std::set<std::pair<int, int>>, std::vector<std::pair<int, int>>, size_t>(part_one, dots, folds, 602, 1);
  if (one != 0) {
    return one;
  }

  std::cout << std::endl;

  part_two(dots, folds);

  return 0;
}
