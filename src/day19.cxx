#include <algorithm>
#include <set>
#include <sstream>
#include <vector>

#include "common.h"

struct Point {
  Point(int x_, int y_, int z_)
      : x(x_), y(y_), z(z_) {}
  int x{};
  int y{};
  int z{};

  [[nodiscard]] std::string repr() const
  {
    std::stringstream s;
    s << x << "," << y << "," << z;
    return s.str();
  }

  bool operator==(const Point &p) const
  {
    return x == p.x && y == p.y && z == p.z;
  }

  bool operator!=(const Point &p) const
  {
    return x != p.x || y != p.y || z != p.z;
  }

  bool operator<(const Point &p) const
  {
    if (x != p.x) {
      return x < p.x;
    }
    if (y != p.y) {
      return y < p.y;
    }
    return z < p.z;
  }
};

struct Sensor {
  Sensor() : coordinate(INT_MAX, INT_MAX, INT_MAX) {}
  std::vector<Point> points;
  Point coordinate;

  bool operator<(const Sensor &s) const
  {
    return coordinate < s.coordinate;
  }
};

void rotate(std::vector<Point> &result,
            size_t mode)
{
  int tmp{};
  for (Point &p : result) {
    switch (mode) {
    case 0:
      break;
    case 1:
      tmp = p.y;
      p.y = p.z;
      p.z = -tmp;
      break;
    case 2:
      p.y = -p.y;
      p.z = -p.z;
      break;
    case 3:
      tmp = p.y;
      p.y = -p.z;
      p.z = tmp;
      break;
    case 4:
      p.x = -p.x;
      p.z = -p.z;
      break;
    case 5:
      p.x = -p.x;
      tmp = p.y;
      p.y = p.z;
      p.z = tmp;
      break;
    case 6:
      p.x = -p.x;
      p.y = -p.y;
      break;
    case 7:
      p.x = -p.x;
      tmp = p.y;
      p.y = -p.z;
      p.z = -tmp;
      break;
    case 8:
      tmp = p.x;
      p.x = p.y;
      p.y = -tmp;
      break;
    case 9:
      tmp = p.x;
      p.x = p.y;
      p.y = p.z;
      p.z = tmp;
      break;
    case 10:
      tmp = p.x;
      p.x = p.y;
      p.y = tmp;
      p.z = -p.z;
      break;
    case 11:
      tmp = p.x;
      p.x = p.y;
      p.y = -p.z;
      p.z = -tmp;
      break;
    case 12:
      tmp = p.x;
      p.x = -p.y;
      p.y = -tmp;
      p.z = -p.z;
      break;
    case 13:
      tmp = p.x;
      p.x = -p.y;
      p.y = p.z;
      p.z = -tmp;
      break;
    case 14:
      tmp = p.x;
      p.x = -p.y;
      p.y = tmp;
      break;
    case 15:
      tmp = p.x;
      p.x = -p.y;
      p.y = -p.z;
      p.z = tmp;
      break;
    case 16:
      tmp = p.x;
      p.x = p.z;
      p.z = p.y;
      p.y = tmp;
      break;
    case 17:
      tmp = p.x;
      p.x = p.z;
      p.z = -tmp;
      break;
    case 18:
      tmp = p.x;
      p.x = p.z;
      p.z = -p.y;
      p.y = -tmp;
      break;
    case 19:
      tmp = p.x;
      p.x = p.z;
      p.z = tmp;
      p.y = -p.y;
      break;
    case 20:
      tmp = p.x;
      p.x = -p.z;
      p.z = -p.y;
      p.y = tmp;
      break;
    case 21:
      tmp = p.x;
      p.x = -p.z;
      p.z = tmp;
      break;
    case 22:
      tmp = p.x;
      p.x = -p.z;
      p.z = p.y;
      p.y = -tmp;
      break;
    case 23:
      tmp = p.x;
      p.x = -p.z;
      p.z = -tmp;
      p.y = -p.y;
      break;
    }
  }
}

std::pair<size_t, size_t> maxFreq(const std::vector<Point> &points)
{
  size_t max{};
  size_t freq{1};
  size_t index{};
  for (size_t i{}; i < points.size() - 1; i++) {
    if (points[i] == points[i + 1]) {
      freq++;
      if (freq > max) {
        max = freq;
        index = i;
      }
    } else {
      freq = 1;
    }
  }

  return {max, index};
}

bool overlap(const Sensor &reference,
             Sensor &sensor)
{
  for (size_t r{}; r < 24; r++) {
    std::vector<Point> current{sensor.points};
    rotate(current, r);

    for (const Point &p : reference.points) {
      for (const Point &c : current) {
        Point d(p.x - c.x, p.y - c.y, p.z - c.z);
        size_t count{};
        for (const Point &cn : current) {
          Point n(cn.x + d.x, cn.y + d.y, cn.z + d.z);
          if (std::find(reference.points.begin(), reference.points.end(), n) != reference.points.end()) {
            count++;
          }
        }
        if (count == 12) {
          sensor.points.clear();
          sensor.coordinate = Point(d.x, d.y, d.z);
          for (const Point &s : current) {
            sensor.points.emplace_back(s.x + d.x, s.y + d.y, s.z + d.z);
          }
          return true;
        }
      }
    }
  }
  return false;
}

void solution(const std::vector<Sensor> &inputs)
{
  std::vector<Sensor> data{inputs};
  data[0].coordinate = Point(0, 0, 0);
  while (data[data.size() - 1].coordinate == Point(INT_MAX, INT_MAX, INT_MAX)) {
    for (size_t i{}; i < inputs.size(); i++) {
      if (data[i].coordinate == Point(INT_MAX, INT_MAX, INT_MAX)) {
        continue;
      }
      bool found{};
      for (size_t j{i + 1}; j < inputs.size(); j++) {
        if (data[j].coordinate != Point(INT_MAX, INT_MAX, INT_MAX)) {
          continue;
        }

        if (overlap(data[i], data[j])) {
          found = true;
          break;
        }
      }
      if (found) {
        break;
      }
    }
    std::sort(data.begin(), data.end());
  }

  std::set<Point> points;
  for (const Sensor &s : data) {
    std::copy(s.points.begin(), s.points.end(), std::inserter(points, points.begin()));
  }

  int max{};
  for (const Sensor &a : data) {
    for (const Sensor &b : data) {
      max = std::max(max, std::abs(a.coordinate.x - b.coordinate.x) + std::abs(a.coordinate.y - b.coordinate.y) + std::abs(a.coordinate.z - b.coordinate.z));
    }
  }

  std::cout << points.size() << " " << max << std::endl;
}

size_t part_two(const std::vector<Sensor> &inputs)
{
  return inputs.size();
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::vector<Sensor> inputs;
  Sensor current;
  for (std::string line; std::getline(input_file, line);) {
    if (line.empty()) {
      continue;
    }

    if (line.find("scanner") != std::string::npos) {
      if (!current.points.empty()) {
        inputs.push_back(current);
        current.points.clear();
      }
      continue;
    }

    std::vector<int> coordinates = splitNumber(line, ",");
    current.points.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
  }
  if (!current.points.empty()) {
    inputs.push_back(current);
  }

  solution(inputs);
  // 362 12204

  return 0;
}
