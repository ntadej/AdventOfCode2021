#include <unordered_map>

#include "common.h"

size_t part_one(const std::string &input,
                const std::unordered_map<std::string, char> &rules)
{
  std::string current = input;
  std::unordered_map<char, size_t> counter{};
  for (char c : current) {
    auto [it, inserted] = counter.emplace(c, 0);
    it->second++;
  }

  for (size_t step{0}; step < 10; step++) {
    std::string next = current;
    size_t insertions{};
    for (size_t i{0}; i < current.size() - 1; i++) {
      auto result = rules.find(current.substr(i, 2));
      if (result != rules.end()) {
        next.insert(i + insertions + 1, 1, result->second);
        insertions++;

        auto [it, inserted] = counter.emplace(result->second, 0);
        it->second++;
      }
    }
    current = next;
  }

  size_t min{UINT64_MAX};
  size_t max{};
  for (auto [c, count] : counter) {
    if (count > max) {
      max = count;
    }
    if (count < min) {
      min = count;
    }
  }

  return max - min;
}

size_t part_two(const std::string &input,
                const std::unordered_map<std::string, char> &rules)
{
  std::unordered_map<char, size_t> counter{};
  for (char c : input) {
    auto [it, inserted] = counter.emplace(c, 0);
    it->second++;
  }

  std::unordered_map<std::string, size_t> pairs{};
  for (size_t i{0}; i < input.size() - 1; i++) {
    auto [it, inserted] = pairs.emplace(input.substr(i, 2), 0);
    it->second++;
  }

  for (size_t step{0}; step < 40; step++) {
    const std::unordered_map<std::string, size_t> current{pairs};
    for (const auto &pair : current) {
      if (pair.second == 0) {
        continue;
      }

      auto result = rules.find(pair.first);
      if (result != rules.end()) {
        pairs[pair.first] -= pair.second;

        auto [it, inserted] = counter.emplace(result->second, 0);
        it->second += pair.second;

        auto [it1, inserted1] = pairs.emplace(std::string() + pair.first[0] + result->second, 0);
        it1->second += pair.second;
        auto [it2, inserted2] = pairs.emplace(std::string() + result->second + pair.first[1], 0);
        it2->second += pair.second;
      }
    }
  }

  size_t min{UINT64_MAX};
  size_t max{};
  for (auto [c, count] : counter) {
    if (count > max) {
      max = count;
    }
    if (count < min) {
      min = count;
    }
  }

  return max - min;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::string input;
  std::getline(input_file, input);
  std::unordered_map<std::string, char> rules;
  for (std::string line; std::getline(input_file, line);) {
    if (line.empty()) {
      continue;
    }

    std::vector<std::string> s = split(line, " -> ");
    rules.emplace(s[0], s[1][0]);
  }

  const int one = testPart<std::string, std::unordered_map<std::string, char>, size_t>(part_one, input, rules, 3906, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::string, std::unordered_map<std::string, char>, size_t>(part_two, input, rules, 4441317262452, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
