#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "common.h"

class Board
{
public:
  void fill(std::ifstream &stream);
  void hit(int number);
  bool check() const;
  int evaluate() const;

private:
  std::array<std::array<int, 5>, 5> _values;
};

void Board::fill(std::ifstream &stream)
{
  for (size_t i{}; i < _values.size(); i++) {
    for (size_t j{}; j < _values[i].size(); j++) {
      stream >> _values[i][j];
    }
  }
}

void Board::hit(int number)
{
  for (size_t i{}; i < _values.size(); i++) {
    for (size_t j{}; j < _values[i].size(); j++) {
      if (_values[i][j] == number) {
        _values[i][j] = -1;
      }
    }
  }
}

bool Board::check() const
{
  for (size_t i{}; i < _values.size(); i++) {
    bool valid{true};
    for (size_t j{}; j < _values[i].size(); j++) {
      valid = valid && _values[i][j] == -1;
    }
    if (valid) {
      return true;
    }
  }

  for (size_t j{}; j < _values[0].size(); j++) {
    bool valid{true};
    for (size_t i{}; i < _values.size(); i++) {
      valid = valid && _values[i][j] == -1;
    }
    if (valid) {
      return true;
    }
  }

  return false;
}

int Board::evaluate() const
{
  int sum{};
  for (size_t i{}; i < _values.size(); i++) {
    for (size_t j{}; j < _values[i].size(); j++) {
      if (_values[i][j] != -1) {
        sum += _values[i][j];
      }
    }
  }
  return sum;
}

std::vector<int> stringSplit(const std::string &string,
                             const std::string &delimiter)
{
  std::string::size_type b = 0;
  std::vector<int> result;

  while ((b = string.find_first_not_of(delimiter, b)) != std::string::npos) {
    std::string::size_type e = string.find_first_of(delimiter, b);
    result.push_back(std::stoi(string.substr(b, e - b)));
    b = e;
  }
  return result;
}

int part_one(const std::vector<int> &parameters,
             const std::vector<Board> &inputs)
{
  std::vector<Board> boards = inputs;
  for (int number : parameters) {
    for (Board &board : boards) {
      board.hit(number);
      if (board.check()) {
        return number * board.evaluate();
      }
    }
  }
  return 0;
}

int part_two(const std::vector<int> &parameters,
             const std::vector<Board> &inputs)
{
  std::vector<Board> boards = inputs;
  for (int number : parameters) {
    for (auto it{boards.begin()}; it != boards.end();) {
      it->hit(number);
      if (it->check()) {
        if (boards.size() == 1) {
          return number * it->evaluate();
        }
        it = boards.erase(it);
      } else {
        it++;
      }
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  std::filesystem::path file = std::string(argv[0]) + ".txt";
  file = file.filename();
  if (argc >= 2) {
    file = argv[1];
  }

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::string paramString{};
  input_file >> paramString;
  std::vector<int> parameters = stringSplit(paramString, ",");
  std::vector<Board> inputs;
  do {
    Board board;
    board.fill(input_file);
    inputs.emplace_back(board);
  } while (!input_file.eof());

  int one = testPart<int, Board>(part_one, parameters, inputs, 51776, 1);
  if (one) {
    return one;
  }

  int two = testPart<int, Board>(part_two, parameters, inputs, 693486, 2);
  if (two) {
    return two;
  }

  return 0;
}
