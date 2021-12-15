#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common.h"

uint64_t part_one(const std::unordered_map<std::string, std::unordered_set<std::string>> &inputs)
{
  std::vector<std::vector<std::string>> routes = {{"start"}};
  std::vector<std::vector<std::string>> out;
  while (!routes.empty()) {
    std::vector<std::string> route = routes.back();
    routes.pop_back();

    for (const std::string &c : inputs.at(route.back())) {
      if (!(std::islower(c.front()) != 0 && std::find(route.begin(), route.end(), c) != route.end())) {
        if (c == "end") {
          out.emplace_back(route);
          out.back().emplace_back(c);
        } else {
          routes.emplace_back(route);
          routes.back().emplace_back(c);
        }
      }
    }
  }
  return out.size();
}

uint64_t part_two(const std::unordered_map<std::string, std::unordered_set<std::string>> &inputs)
{
  std::vector<std::vector<std::string>> routes = {{"1", "start"}};
  std::vector<std::vector<std::string>> out;
  while (!routes.empty()) {
    std::vector<std::string> route = routes.back();
    routes.pop_back();

    for (const std::string &c : inputs.at(route.back())) {
      std::vector<std::string> routeStep = route;
      const bool smallDuplicated = std::islower(c.front()) != 0 && std::find(routeStep.begin(), routeStep.end(), c) != routeStep.end();
      const bool valid = routeStep.front() == "1" || !smallDuplicated;
      if (smallDuplicated) {
        routeStep.front() = "0";
      }
      if (valid) {
        if (c == "end") {
          out.emplace_back(routeStep);
          out.back().emplace_back(c);
        } else {
          routes.emplace_back(routeStep);
          routes.back().emplace_back(c);
        }
      }
    }
  }
  return out.size();
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::unordered_map<std::string, std::unordered_set<std::string>> connections;
  for (std::string line; std::getline(input_file, line);) {
    std::vector<std::string> row = split(line, "-");
    std::string id1 = row[0];
    std::string id2 = row[1];

    if (id1 != "end" && id2 != "start") {
      auto [it1, in1] = connections.emplace(id1, std::unordered_set<std::string>());
      it1->second.insert(id2);
    }
    if (id2 != "end" && id1 != "start") {
      auto [it2, in2] = connections.emplace(id2, std::unordered_set<std::string>());
      it2->second.insert(id1);
    }
  }

  const int one = testPart<std::unordered_map<std::string, std::unordered_set<std::string>>, size_t>(part_one, connections, 4378, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::unordered_map<std::string, std::unordered_set<std::string>>, size_t>(part_two, connections, 133621, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
