// Copyright 2022 NNTU-CS
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "tree.h"

std::vector<char> makeSymbols(int n) {
  std::vector<char> sym;
  sym.reserve(n);
  for (int i = 1; i <= n; ++i) {
    if (i <= 9)
      sym.push_back(static_cast<char>('0' + i));
    else
      sym.push_back(static_cast<char>('A' + (i - 10)));
  }
  return sym;
}

uint64_t factorial(int n) {
  uint64_t res = 1;
  for (int i = 2; i <= n; ++i) res *= i;
  return res;
}

template <typename Func>
double measureTimeMs(Func&& func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
  std::cout << "========== ПРИМЕРЫ ==========\n";
  std::vector<char> in = {'1', '2', '3'};
  PMTree tree(in);

  std::vector<std::vector<char>> all = getAllPerms(tree);
  std::cout << "Все перестановки для {1,2,3}: ";
  for (const auto& perm : all) {
    for (char c : perm) std::cout << c;
    std::cout << " ";
  }
  std::cout << "\n";

  auto p1 = getPerm1(tree, 2);
  std::cout << "getPerm1(tree, 2) = ";
  for (char c : p1) std::cout << c;
  std::cout << "\n";

  auto p2 = getPerm2(tree, 3);
  std::cout << "getPerm2(tree, 3) = ";
  for (char c : p2) std::cout << c;
  std::cout << "\n";

  auto invalid = getPerm2(tree, 100);
  std::cout << "Запрос несуществующей перестановки №100: "
            << (invalid.empty() ? "пустой вектор (корректно)" : "ОШИБКА")
            << "\n\n";

  std::cout << "========== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ==========\n";
  const int maxN = 10;
  const int repeats = 5;
  std::mt19937 rng(42);

  std::ofstream csv("experiment_data.csv");
  csv << "n,getAllPerms_ms,getPerm1_ms,getPerm2_ms\n";

  for (int n = 1; n <= maxN; ++n) {
    std::vector<char> symbols = makeSymbols(n);
    PMTree tree(symbols);
    uint64_t totalPerms = factorial(n);

    double timeAll = measureTimeMs([&tree]() { getAllPerms(tree); });

    double time1 = 0.0;
    for (int r = 0; r < repeats; ++r) {
      int randNum = std::uniform_int_distribution<int>(1,
                         static_cast<int>(totalPerms))(rng);
      time1 += measureTimeMs([&tree, randNum]() { getPerm1(tree, randNum); });
    }
    time1 /= repeats;

    double time2 = 0.0;
    for (int r = 0; r < repeats; ++r) {
      int randNum = std::uniform_int_distribution<int>(1,
                         static_cast<int>(totalPerms))(rng);
      time2 += measureTimeMs([&tree, randNum]() { getPerm2(tree, randNum); });
    }
    time2 /= repeats;

    std::cout << "n=" << n
              << " | getAllPerms: " << timeAll << " ms"
              << " | getPerm1: " << time1 << " ms"
              << " | getPerm2: " << time2 << " ms\n";
    csv << n << "," << timeAll << "," << time1 << "," << time2 << "\n";
  }
  csv.close();
  std::cout << "\nРезультаты эксперимента сохранены в experiment_data.csv\n";
  std::cout << "Для построения графика выполните python plot.py\n";

  return 0;
}
