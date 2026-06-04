// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <cstddef>
#include <vector>

class PMTree {
 public:
  explicit PMTree(const std::vector<char>& symbols);
  ~PMTree();

  friend std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
  friend std::vector<char> getPerm1(const PMTree& tree, int num);
  friend std::vector<char> getPerm2(const PMTree& tree, int num);

 private:
  struct Node {
    char value;
    std::vector<Node*> children;
    size_t permCount;

    explicit Node(char val) : value(val), permCount(0) {}
  };

  Node* root;

  static void buildSubtree(Node* node, std::vector<char> remaining);
  static void deleteSubtree(Node* node);
  static void collectPerms(const Node* node,
                           std::vector<char>& current,
                           std::vector<std::vector<char>>& out);
};

#endif  // INCLUDE_TREE_H_
