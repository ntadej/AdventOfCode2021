#include <memory>
#include <vector>

#include "common.h"

struct Snailfish {
  int left{-1};
  int right{-1};
  Snailfish *parent{};
  std::unique_ptr<Snailfish> leftSnailfish{};
  std::unique_ptr<Snailfish> rightSnailfish{};

  [[nodiscard]] std::string repr() const;
  [[nodiscard]] int level() const;
  void reduce();

  [[nodiscard]] bool explode();
  [[nodiscard]] bool split();

  void addToParentLeft(int n);
  void addToParentRight(int n);
  void addToChildLeft(int n);
  void addToChildRight(int n);

  [[nodiscard]] uint64_t magnitude() const;

  [[nodiscard]] std::unique_ptr<Snailfish> copy() const;
};

std::string Snailfish::repr() const
{
  std::string out = "[";
  if (leftSnailfish != nullptr) {
    out.append(leftSnailfish->repr());
  } else {
    out.append(std::to_string(left));
  }
  out.append(",");
  if (rightSnailfish != nullptr) {
    out.append(rightSnailfish->repr());
  } else {
    out.append(std::to_string(right));
  }
  out.append("]");
  return out;
}

int Snailfish::level() const
{
  if (parent != nullptr) {
    return parent->level() + 1;
  }

  return 1;
}

void Snailfish::reduce()
{
  while (true) {
    if (explode()) {
      continue;
    }
    if (split()) {
      continue;
    }
    break;
  }
}

bool Snailfish::explode()
{
  if (leftSnailfish) {
    if (leftSnailfish->level() > 4) {
      leftSnailfish->addToParentLeft(leftSnailfish->left);
      leftSnailfish->addToParentRight(leftSnailfish->right);
      leftSnailfish.reset();
      left = 0;
      return true;
    }
    if (leftSnailfish->explode()) {
      return true;
    }
  }

  if (rightSnailfish) {
    if (rightSnailfish->level() > 4) {
      rightSnailfish->addToParentLeft(rightSnailfish->left);
      rightSnailfish->addToParentRight(rightSnailfish->right);
      rightSnailfish.reset();
      right = 0;
      return true;
    }
    if (rightSnailfish->explode()) {
      return true;
    }
  }

  return false;
}

bool Snailfish::split()
{
  if (left > 9) {
    int n = left;
    left = -1;
    leftSnailfish = std::make_unique<Snailfish>();
    leftSnailfish->parent = this;
    leftSnailfish->left = n / 2;
    leftSnailfish->right = n % 2 == 0 ? n / 2 : n / 2 + 1;
    if (leftSnailfish->level() > 4) {
      (void)explode();
    }
    return true;
  }

  if (leftSnailfish != nullptr && leftSnailfish->split()) {
    return true;
  }

  if (right > 9) {
    int n = right;
    right = -1;
    rightSnailfish = std::make_unique<Snailfish>();
    rightSnailfish->parent = this;
    rightSnailfish->left = n / 2;
    rightSnailfish->right = n % 2 == 0 ? n / 2 : n / 2 + 1;
    if (rightSnailfish->level() > 4) {
      (void)explode();
    }
    return true;
  }

  if (rightSnailfish != nullptr && rightSnailfish->split()) {
    return true;
  }

  return false;
}

void Snailfish::addToParentLeft(int n)
{
  if (parent == nullptr) {
    return;
  }

  if (parent->leftSnailfish == nullptr) {
    parent->left += n;
  } else if (parent->rightSnailfish.get() == this) {
    parent->leftSnailfish->addToChildRight(n);
  } else if (parent->leftSnailfish.get() == this) {
    parent->addToParentLeft(n);
  }
}

void Snailfish::addToParentRight(int n)
{
  if (parent == nullptr) {
    return;
  }

  if (parent->rightSnailfish == nullptr) {
    parent->right += n;
  } else if (parent->leftSnailfish.get() == this) {
    parent->rightSnailfish->addToChildLeft(n);
  } else if (parent->rightSnailfish.get() == this) {
    parent->addToParentRight(n);
  }
}

void Snailfish::addToChildLeft(int n)
{
  if (leftSnailfish == nullptr) {
    left += n;
  } else {
    leftSnailfish->addToChildLeft(n);
  }
}

void Snailfish::addToChildRight(int n)
{
  if (rightSnailfish == nullptr) {
    right += n;
  } else {
    rightSnailfish->addToChildRight(n);
  }
}

uint64_t Snailfish::magnitude() const
{
  uint64_t m{};
  if (leftSnailfish != nullptr) {
    m += 3 * leftSnailfish->magnitude();
  } else {
    m += static_cast<uint64_t>(3 * left);
  }

  if (rightSnailfish != nullptr) {
    m += 2 * rightSnailfish->magnitude();
  } else {
    m += static_cast<uint64_t>(2 * right);
  }

  return m;
}

std::unique_ptr<Snailfish> Snailfish::copy() const
{
  auto out = std::make_unique<Snailfish>();

  if (leftSnailfish != nullptr) {
    out->leftSnailfish = leftSnailfish->copy();
    out->leftSnailfish->parent = out.get();
  } else {
    out->left = left;
  }

  if (rightSnailfish != nullptr) {
    out->rightSnailfish = rightSnailfish->copy();
    out->rightSnailfish->parent = out.get();
  } else {
    out->right = right;
  }

  return out;
}

std::unique_ptr<Snailfish> add(std::unique_ptr<Snailfish> a, std::unique_ptr<Snailfish> b)
{
  auto out = std::make_unique<Snailfish>();
  out->leftSnailfish = std::move(a);
  out->leftSnailfish->parent = out.get();
  out->rightSnailfish = std::move(b);
  out->rightSnailfish->parent = out.get();
  return out;
}

uint64_t part_one(const std::vector<std::unique_ptr<Snailfish>> &inputs)
{
  std::unique_ptr<Snailfish> out = inputs.front()->copy();
  for (size_t i{1}; i < inputs.size(); i++) {
    out = add(std::move(out), inputs[i]->copy());
    out->reduce();
  }
  return out->magnitude();
}

uint64_t part_two(const std::vector<std::unique_ptr<Snailfish>> &inputs)
{
  uint64_t magnitude{};
  for (size_t i{}; i < inputs.size(); i++) {
    for (size_t j{}; j < inputs.size(); j++) {
      if (i == j) {
        continue;
      }

      std::unique_ptr<Snailfish> out = add(inputs[i]->copy(), inputs[j]->copy());
      out->reduce();
      magnitude = std::max(magnitude, out->magnitude());
    }
  }
  return magnitude;
}

int main(int argc, char **argv)
{
  const std::filesystem::path file = fileName(argc, argv);

  std::ifstream input_file(file);
  if (!input_file.is_open()) {
    std::cerr << "Input file " << file << " can not be opened!" << std::endl;
    return 10;
  }

  std::vector<std::unique_ptr<Snailfish>> inputs;
  for (std::string line; std::getline(input_file, line);) {
    auto main = std::make_unique<Snailfish>();
    std::vector<Snailfish *> children;
    children.push_back(main.get());

    for (char c : line.substr(1, line.size() - 2)) {
      if (c == ',') {
        continue;
      }

      Snailfish *current = children.back();
      if (c == '[') {
        Snailfish *nested{};
        if (current->leftSnailfish == nullptr && current->left == -1) {
          current->leftSnailfish = std::make_unique<Snailfish>();
          nested = current->leftSnailfish.get();
        } else {
          current->rightSnailfish = std::make_unique<Snailfish>();
          nested = current->rightSnailfish.get();
        }
        nested->parent = current;
        children.push_back(nested);
        continue;
      }
      if (std::isdigit(c) != 0) {
        int n = c - 48;
        if (current->leftSnailfish == nullptr && current->left == -1) {
          current->left = n;
        } else {
          current->right = n;
        }
        continue;
      }
      if (c == ']') {
        children.pop_back();
      }
    }

    inputs.emplace_back(std::move(main));
  }

  const int one = testPart<std::vector<std::unique_ptr<Snailfish>>, uint64_t>(part_one, inputs, 3359, 1);
  if (one != 0) {
    return one;
  }

  const int two = testPart<std::vector<std::unique_ptr<Snailfish>>, uint64_t>(part_two, inputs, 4616, 2);
  if (two != 0) {
    return two;
  }

  return 0;
}
