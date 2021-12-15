#include <array>
#include <vector>

#include "common.h"

class Board
{
public:
  void fill(std::ifstream &stream);
  [[nodiscard]] int &value(size_t i,
                           size_t j);
  [[nodiscard]] const int &value(size_t i,
                                 size_t j) const;
  void hit(int number);
  [[nodiscard]] bool check() const;
  [[nodiscard]] int evaluate() const;
  [[nodiscard]] constexpr size_t size() const { return 5; }

private:
  std::array<std::array<int, 5>, 5> _values;
};

void Board::fill(std::ifstream &stream)
{
  for (size_t i{}; i < size(); i++) {
    for (size_t j{}; j < size(); j++) {
      stream >> _values.at(i).at(j);
    }
  }
}

int &Board::value(size_t i,
                  size_t j)
{
  return _values.at(i).at(j);
}

const int &Board::value(size_t i,
                        size_t j) const
{
  return _values.at(i).at(j);
}

void Board::hit(int number)
{
  for (size_t i{}; i < size(); i++) {
    for (size_t j{}; j < size(); j++) {
      if (value(i, j) == number) {
        value(i, j) = -1;
      }
    }
  }
}

bool Board::check() const
{
  for (size_t i{}; i < size(); i++) {
    bool valid{true};
    for (size_t j{}; j < size(); j++) {
      valid = valid && value(i, j) == -1;
    }
    if (valid) {
      return true;
    }
  }

  for (size_t j{}; j < size(); j++) {
    bool valid{true};
    for (size_t i{}; i < size(); i++) {
      valid = valid && value(i, j) == -1;
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
  for (size_t i{}; i < size(); i++) {
    for (size_t j{}; j < size(); j++) {
      if (value(i, j) != -1) {
        sum += value(i, j);
      }
    }
  }
  return sum;
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
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::string paramString{};
  input_file >> paramString;
  const std::vector<int> parameters = splitNumber(paramString, ",");
  std::vector<Board> inputs;
  while (!input_file.eof()) {
    Board board{};
    board.fill(input_file);
    if (board.evaluate() != 0) {
      inputs.emplace_back(board);
    }
  }

  const int one = testPart<std::vector<int>, std::vector<Board>, int>(part_one, parameters, inputs, 51776, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<int>, std::vector<Board>, int>(part_two, parameters, inputs, 16830, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
