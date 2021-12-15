#include <cmath>
#include <set>
#include <vector>

#include "common.h"

size_t part_one(const std::vector<std::vector<std::string>> & /* inputs */,
                const std::vector<std::vector<std::string>> &outputs)
{
  size_t count{};
  for (const std::vector<std::string> &line : outputs) {
    for (const std::string &pattern : line) {
      if (pattern.size() == 2 || pattern.size() == 3 || pattern.size() == 4 || pattern.size() == 7) {
        count++;
      }
    }
  }
  return count;
}

const std::vector<size_t> order{0, 1, 2, 3, 4, 7, 6, 5};

size_t part_two(const std::vector<std::vector<std::string>> &inputs,
                const std::vector<std::vector<std::string>> &outputs)
{
  size_t total{};
  for (size_t i{}; i < inputs.size(); i++) {
    std::vector<std::set<char>> solution(10, std::set<char>());

    std::vector<std::string> inputPatterns{inputs[i]};
    std::sort(inputPatterns.begin(), inputPatterns.end(), [](const std::string &a, const std::string &b) {
      size_t sa = order[a.size()];
      size_t sb = order[b.size()];
      return sa < sb;
    });

    for (auto it{inputPatterns.begin()}; it != inputPatterns.end();) {
      if (it->size() == 2) {
        for (char c : *it) {
          solution[1].insert(c);
        }
        inputPatterns.erase(it);
      } else if (it->size() == 3) {
        for (char c : *it) {
          solution[7].insert(c);
        }
        inputPatterns.erase(it);
      } else if (it->size() == 4) {
        for (char c : *it) {
          solution[4].insert(c);
        }
        inputPatterns.erase(it);
      } else if (it->size() == 7) {
        for (char c : *it) {
          solution[8].insert(c);
        }
        inputPatterns.erase(it);
      } else {
        break;
      }
    }

    std::set<char> fourSeven;
    std::set_union(solution[4].begin(), solution[4].end(), solution[7].begin(), solution[7].end(), std::inserter(fourSeven, fourSeven.begin()));

    std::set<char> eightNotFourSeven;
    std::set_difference(solution[8].begin(), solution[8].end(), fourSeven.begin(), fourSeven.end(), std::inserter(eightNotFourSeven, eightNotFourSeven.begin()));

    for (auto it{inputPatterns.begin()}; it != inputPatterns.end();) {
      std::set<char> current;
      for (char c : *it) {
        current.insert(c);
      }
      if (current.size() == 6) {
        std::set<char> notEight;
        std::set_difference(solution[8].begin(), solution[8].end(), current.begin(), current.end(), std::inserter(notEight, notEight.begin()));
        if (solution[1].find(*notEight.begin()) != solution[1].end()) {
          solution[6] = current;
          inputPatterns.erase(it);
        } else if (eightNotFourSeven.find(*notEight.begin()) != eightNotFourSeven.end()) {
          solution[9] = current;
          inputPatterns.erase(it);
        } else {
          solution[0] = current;
          inputPatterns.erase(it);
        }
      } else {
        ++it;
      }
    }

    for (auto it{inputPatterns.begin()}; it != inputPatterns.end();) {
      std::set<char> current;
      for (char c : *it) {
        current.insert(c);
      }
      if (current.size() == 5) {
        std::set<char> notNine;
        std::set_difference(solution[9].begin(), solution[9].end(), current.begin(), current.end(), std::inserter(notNine, notNine.begin()));
        if (notNine.size() == 2) {
          solution[2] = current;
          inputPatterns.erase(it);
        } else if (solution[1].find(*notNine.begin()) != solution[1].end()) {
          solution[5] = current;
          inputPatterns.erase(it);
        } else {
          solution[3] = current;
          inputPatterns.erase(it);
        }
      } else {
        ++it;
      }
    }

    size_t num{};
    for (size_t p{}; p < outputs[i].size(); p++) {
      std::set<char> current;
      for (char c : outputs[i][p]) {
        current.insert(c);
      }
      for (size_t n{}; n < solution.size(); n++) {
        if (current == solution[n]) {
          num += n * static_cast<size_t>(std::pow(10, outputs[i].size() - p - 1));
        }
      }
    }
    total += num;
  }
  return total;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }
  std::vector<std::vector<std::string>> inputs;
  std::vector<std::vector<std::string>> outputs;
  for (std::string line; std::getline(input_file, line);) {
    std::vector<std::string> lineSplit = split(line, "|");
    inputs.emplace_back(split(lineSplit[0], " "));
    outputs.emplace_back(split(lineSplit[1], " "));
  }

  const int one = testPart<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>, size_t>(part_one, inputs, outputs, 554, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>, size_t>(part_two, inputs, outputs, 990964, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
