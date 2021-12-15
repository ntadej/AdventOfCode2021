#include <unordered_set>
#include <vector>

#include "common.h"

int part_one(const std::vector<std::string> &inputs)
{
  std::vector<size_t> countOne(inputs.front().size(), 0);
  std::vector<size_t> countZero(inputs.front().size(), 0);
  for (const std::string &binary : inputs) {
    for (size_t i{}; i < binary.size(); i++) {
      if (binary[i] == '1') {
        countOne[i]++;
      } else {
        countZero[i]++;
      }
    }
  }

  std::string gamma{inputs.front()};
  std::string epsilon{inputs.front()};
  for (size_t i{}; i < gamma.size(); i++) {
    gamma[i] = countOne[i] > countZero[i] ? '1' : '0';
    epsilon[i] = countOne[i] > countZero[i] ? '0' : '1';
  }

  return std::stoi(gamma, nullptr, 2) * std::stoi(epsilon, nullptr, 2);
}

int part_two_sensor(const std::vector<std::string> &inputs,
                    char type)
{
  const char antiType = type == '1' ? '0' : '1';
  const size_t size{inputs.front().size()};
  std::unordered_set<size_t> selected{};
  for (size_t j{}; j < inputs.size(); j++) {
    selected.insert(j);
  }

  size_t countOne{};
  size_t countZero{};
  for (size_t i{}; i < size; i++) {
    countOne = 0;
    countZero = 0;
    char bit{};
    for (size_t j : selected) {
      if (inputs[j][i] == '1') {
        countOne++;
      } else {
        countZero++;
      }
    }

    if (countOne == countZero) {
      bit = type;
    } else {
      bit = countOne > countZero ? type : antiType;
    }
    for (auto it{selected.begin()}; it != selected.end();) {
      if (inputs[*it][i] != bit) {
        it = selected.erase(it);
      } else {
        it++;
      }
    }

    if (selected.size() == 1) {
      break;
    }
  }

  return std::stoi(inputs[*selected.begin()], nullptr, 2);
}

int part_two(const std::vector<std::string> &inputs)
{
  return part_two_sensor(inputs, '1') * part_two_sensor(inputs, '0');
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
  do {
    std::string string{};
    input_file >> string;
    inputs.emplace_back(string);
  } while (!input_file.eof());

  const int one = testPart<std::vector<std::string>, int>(part_one, inputs, 693486, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::string>, int>(part_two, inputs, 3379326, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
