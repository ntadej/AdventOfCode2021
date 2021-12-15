#include <numeric>
#include <unordered_map>
#include <vector>

#include "common.h"

const std::unordered_map<char, uint64_t> values = {
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}};

const std::unordered_map<char, uint64_t> completion = {
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}};

uint64_t part_one(const std::vector<std::string> &inputs)
{
  std::vector<char> illegal;
  for (const std::string &line : inputs) {
    std::vector<char> expected;
    for (char c : line) {
      if (c == '(') {
        expected.emplace_back(')');
      } else if (c == '[') {
        expected.emplace_back(']');
      } else if (c == '{') {
        expected.emplace_back('}');
      } else if (c == '<') {
        expected.emplace_back('>');
      } else if (c == ')' || c == ']' || c == '}' || c == '>') {
        if (expected.back() != c) {
          illegal.emplace_back(c);
          break;
        }
        expected.pop_back();
      }
    }
  }
  return std::accumulate(illegal.begin(), illegal.end(), 0, [](uint64_t a, char b) {
    return a + values.at(b);
  });
}

uint64_t part_two(const std::vector<std::string> &inputs)
{
  std::vector<uint64_t> scores;

  for (const std::string &line : inputs) {
    std::vector<char> expected;
    bool valid{true};
    for (char c : line) {
      if (c == '(') {
        expected.emplace_back(')');
      } else if (c == '[') {
        expected.emplace_back(']');
      } else if (c == '{') {
        expected.emplace_back('}');
      } else if (c == '<') {
        expected.emplace_back('>');
      } else if (c == ')' || c == ']' || c == '}' || c == '>') {
        if (expected.back() != c) {
          valid = false;
          break;
        }
        expected.pop_back();
      }
    }

    if (valid && !expected.empty()) {
      uint64_t score{};
      for (auto it = expected.rbegin(); it != expected.rend(); it++) {
        score = score * 5 + completion.at(*it);
      }
      scores.emplace_back(score);
    }
  }

  std::sort(scores.begin(), scores.end());
  return scores[scores.size() / 2];
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }
  std::vector<std::string> inputs;
  for (std::string line; std::getline(input_file, line);) {
    inputs.emplace_back(line);
  }

  const int one = testPart<std::vector<std::string>, uint64_t>(part_one, inputs, 367227, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::string>, uint64_t>(part_two, inputs, 3583341858, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
