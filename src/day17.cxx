#include <vector>

#include "common.h"

template <typename T>
int sgn(T val)
{
  return (T(0) < val) - (val < T(0));
}

int part_one(const std::pair<std::pair<int, int>, std::pair<int, int>> &area)
{
  int yMax{};
  for (int vy0{area.second.first}; vy0 < -area.second.first; vy0++) {
    int y{};
    int vy{vy0};
    int cMax{};

    while (true) {
      y += vy;
      vy -= 1;
      cMax = std::max(cMax, y);

      if (area.second.first <= y && y <= area.second.second) {
        yMax = std::max(cMax, yMax);
        break;
      }

      if (y < area.second.first && vy < 0) {
        break;
      }
    }
  }
  return yMax;
}

size_t part_two(const std::pair<std::pair<int, int>, std::pair<int, int>> &area)
{
  size_t count{};
  for (int vx0{-area.first.second}; vx0 <= area.first.second; vx0++) {
    for (int vy0{area.second.first}; vy0 <= -area.second.first; vy0++) {
      int x{};
      int y{};
      int vx{vx0};
      int vy{vy0};

      while (true) {
        x += vx;
        y += vy;
        vx -= sgn(vx);
        vy -= 1;

        if (area.first.first <= x && x <= area.first.second && area.second.first <= y && y <= area.second.second) {
          count++;
          break;
        }

        if (y < area.second.first && vy < 0) {
          break;
        }
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
  input.erase(0, 15);
  std::vector<std::string> inputSplit = split(input, ", y=");
  std::vector<int> xArea = splitNumber(inputSplit[0], "..");
  std::vector<int> yArea = splitNumber(inputSplit[1], "..");
  std::pair<std::pair<int, int>, std::pair<int, int>> area = {{xArea[0], xArea[1]}, {yArea[0], yArea[1]}};

  const int one = testPart<std::pair<std::pair<int, int>, std::pair<int, int>>, int>(part_one, area, 12246, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::pair<std::pair<int, int>, std::pair<int, int>>, size_t>(part_two, area, 3528, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
