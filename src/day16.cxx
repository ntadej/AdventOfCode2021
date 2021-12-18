#include <cmath>
#include <memory>
#include <sstream>
#include <vector>

#include "common.h"

const char *hexToBinary(char c)
{
  switch (c) {
  case '0':
    return "0000";
  case '1':
    return "0001";
  case '2':
    return "0010";
  case '3':
    return "0011";
  case '4':
    return "0100";
  case '5':
    return "0101";
  case '6':
    return "0110";
  case '7':
    return "0111";
  case '8':
    return "1000";
  case '9':
    return "1001";
  case 'A':
    return "1010";
  case 'B':
    return "1011";
  case 'C':
    return "1100";
  case 'D':
    return "1101";
  case 'E':
    return "1110";
  case 'F':
    return "1111";
  }

  throw std::runtime_error("invalid character");
}

struct Packet {
  int version{};
  int type{};
  uint64_t value{};
  std::vector<std::unique_ptr<Packet>> subpackets;

  [[nodiscard]] std::string repr() const;
  [[nodiscard]] uint64_t versionSum() const;
  [[nodiscard]] uint64_t evaluate() const;
};

std::string Packet::repr() const
{
  std::stringstream s;
  s << "version: " << version << ", type: " << type << ", ";
  if (type == 4) {
    s << "value: " << value;
  } else {
    s << "subpackets: ";
    for (const std::unique_ptr<Packet> &p : subpackets) {
      s << "(" << p->repr() << ") ";
    }
  }
  return s.str();
}

uint64_t Packet::versionSum() const
{
  uint64_t sum{};
  sum += version;
  for (const std::unique_ptr<Packet> &p : subpackets) {
    sum += p->versionSum();
  }
  return sum;
}

uint64_t Packet::evaluate() const
{
  if (type == 4) {
    return value;
  }

  if (type == 0) {
    // sum
    uint64_t sum{};
    for (const std::unique_ptr<Packet> &p : subpackets) {
      sum += p->evaluate();
    }
    return sum;
  }
  if (type == 1) {
    // product
    uint64_t prod{UINT64_MAX};
    for (const std::unique_ptr<Packet> &p : subpackets) {
      if (prod == UINT64_MAX) {
        prod = p->evaluate();
      } else {
        prod *= p->evaluate();
      }
    }
    return prod;
  }
  if (type == 2) {
    // min
    uint64_t min{UINT64_MAX};
    for (const std::unique_ptr<Packet> &p : subpackets) {
      min = std::min(min, p->evaluate());
    }
    return min;
  }
  if (type == 3) {
    // max
    uint64_t max{};
    for (const std::unique_ptr<Packet> &p : subpackets) {
      max = std::max(max, p->evaluate());
    }
    return max;
  }
  if (type == 5) {
    // greater than
    return subpackets[0]->evaluate() > subpackets[1]->evaluate() ? 1 : 0;
  }
  if (type == 6) {
    // less than
    return subpackets[0]->evaluate() < subpackets[1]->evaluate() ? 1 : 0;
  }
  if (type == 7) {
    // equals
    return subpackets[0]->evaluate() == subpackets[1]->evaluate() ? 1 : 0;
  }

  return 0;
}

std::pair<std::unique_ptr<Packet>, size_t> parse(const std::string &binaryString)
{
  size_t offset{};

  auto packet = std::make_unique<Packet>();
  packet->version = std::stoi(binaryString.substr(offset, 3), nullptr, 2);
  offset += 3;
  packet->type = std::stoi(binaryString.substr(offset, 3), nullptr, 2);
  offset += 3;

  if (packet->type == 4) {
    // numeric literal
    std::string digits;
    while (true) {
      digits.append(binaryString.substr(offset + 1, 4));
      offset += 5;
      if (binaryString[offset - 5] == '0') {
        break;
      }
    }
    packet->value = std::stol(digits, nullptr, 2);
  } else {
    // operator
    offset++;
    if (binaryString[offset - 1] == '1') {
      size_t count = std::stoi(binaryString.substr(offset, 11), nullptr, 2);
      offset += 11;
      for (size_t c{}; c < count; c++) {
        auto [p, o] = parse(binaryString.substr(offset));
        packet->subpackets.push_back(std::move(p));
        offset += o;
      }
    } else {
      size_t size = std::stoi(binaryString.substr(offset, 15), nullptr, 2);
      offset += 15;
      size_t buffer{};
      while (buffer < size) {
        auto [p, o] = parse(binaryString.substr(offset));
        packet->subpackets.push_back(std::move(p));
        buffer += o;
        offset += o;
      }
    }
  }

  return {std::move(packet), offset};
}

uint64_t part_one(const std::string &binaryString)
{
  return parse(binaryString).first->versionSum();
}

uint64_t part_two(const std::string &binaryString)
{
  return parse(binaryString).first->evaluate();
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
  std::string binaryString;
  for (char c : input) {
    binaryString.append(hexToBinary(c));
  }

  const int one = testPart<std::string, uint64_t>(part_one, binaryString, 871, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::string, uint64_t>(part_two, binaryString, 68703010504, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
