#include <vector>

#include "common.h"

char decode(size_t step,
            int i,
            int j,
            const std::vector<std::vector<char>> &picture,
            const std::string &pattern)
{
  std::vector<char> chars;
  int X = static_cast<int>(picture.size());
  int Y = static_cast<int>(picture.back().size());

  for (int I{i - 3}; I < i; I++) {
    for (int J{j - 3}; J < j; J++) {
      if (I < 0 || J < 0 || I >= X || J >= Y) {
        chars.push_back(step % 2 != 0 && pattern[0] == '#' ? '1' : '0');
        continue;
      }
      chars.push_back(picture[I][J] == '#' ? '1' : '0');
    }
  }

  std::string string(chars.begin(), chars.end());
  int index = std::stoi(string, nullptr, 2);

  return pattern[index];
}

size_t part_one(const std::string &pattern,
                const std::vector<std::vector<char>> &picture)
{
  std::vector<std::vector<char>> result{picture};
  for (size_t s{}; s < 2; s++) {
    std::vector<std::vector<char>> current(result.size() + 4, std::vector<char>(result.back().size() + 4, '.'));
    for (size_t i{}; i < current.size(); i++) {
      for (size_t j{}; j < current.size(); j++) {
        current[i][j] = decode(s, static_cast<int>(i), static_cast<int>(j), result, pattern);
      }
    }
    result = current;
  }

  size_t count{};
  for (size_t i{}; i < result.size(); i++) {
    for (size_t j{}; j < result.size(); j++) {
      if (result[i][j] == '#') {
        count++;
      }
      std::cout << result[i][j];
    }
    std::cout << std::endl;
  }

  return count;
}

size_t part_two(const std::string &pattern,
                const std::vector<std::vector<char>> &picture)
{
  std::vector<std::vector<char>> result{picture};
  for (size_t s{}; s < 50; s++) {
    std::vector<std::vector<char>> current(result.size() + 4, std::vector<char>(result.back().size() + 4, '.'));
    for (size_t i{}; i < current.size(); i++) {
      for (size_t j{}; j < current.size(); j++) {
        current[i][j] = decode(s, static_cast<int>(i), static_cast<int>(j), result, pattern);
      }
    }
    result = current;
  }

  size_t count{};
  for (size_t i{}; i < result.size(); i++) {
    for (size_t j{}; j < result.size(); j++) {
      if (result[i][j] == '#') {
        count++;
      }
    }
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

  std::string input;
  std::getline(input_file, input);
  std::vector<std::vector<char>> picture;
  for (std::string line; std::getline(input_file, line);) {
    if (line.empty()) {
      continue;
    }

    std::vector<char> row;
    for (char c : line) {
      row.push_back(c);
    }
    picture.push_back(row);
  }

  const int one = testPart<std::string, std::vector<std::vector<char>>, size_t>(part_one, input, picture, 5229, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::string, std::vector<std::vector<char>>, size_t>(part_two, input, picture, 17009, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
