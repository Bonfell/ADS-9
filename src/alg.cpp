// Copyright 2022 NNTU-CS
#include <algorithm>
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"

PMTree::PMTree(const std::vector<char>& symbols) {
  root = new Node('\0');
  std::vector<char> sorted = symbols;
  std::sort(sorted.begin(), sorted.end());
  buildSubtree(root, sorted);
}

PMTree::~PMTree() {
  deleteSubtree(root);
}

void PMTree::buildSubtree(Node* node, std::vector<char> remaining) {
  if (remaining.empty()) {
    node->permCount = 1;
    return;
  }

  size_t total = 0;
  for (size_t i = 0; i < remaining.size(); ++i) {
    char ch = remaining[i];
    Node* child = new Node(ch);
    node->children.push_back(child);

    std::vector<char> next = remaining;
    next.erase(next.begin() + i);
    buildSubtree(child, next);

    total += child->permCount;
  }
  node->permCount = total;
}

void PMTree::deleteSubtree(Node* node) {
  if (!node) return;
  for (Node* child : node->children) {
    deleteSubtree(child);
  }
  delete node;
}

static void collectPerms(const PMTree::Node* node,
                         std::vector<char>& current,
                         std::vector<std::vector<char>>& out) {
  if (node->children.empty()) {
    out.push_back(current);
    return;
  }
  for (const PMTree::Node* child : node->children) {
    current.push_back(child->value);
    collectPerms(child, current, out);
    current.pop_back();
  }
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  std::vector<std::vector<char>> result;
  std::vector<char> current;
  collectPerms(tree.root, current, result);
  return result;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  if (num < 1) return {};
  std::vector<std::vector<char>> all = getAllPerms(tree);
  if (static_cast<size_t>(num) > all.size()) return {};
  return all[num - 1];
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  if (num < 1) return {};
  const PMTree::Node* cur = tree.root;
  if (num > static_cast<int>(cur->permCount)) return {};

  std::vector<char> result;
  while (!cur->children.empty()) {
    size_t block = cur->children[0]->permCount;
    size_t idx = static_cast<size_t>(num - 1) / block;
    if (idx >= cur->children.size()) return {};
    cur = cur->children[idx];
    result.push_back(cur->value);
    num = (num - 1) % static_cast<int>(block) + 1;
  }
  return result;
}

