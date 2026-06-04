// Copyright 2022 NNTU-CS
#include "tree.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

std::vector<char> makeSymbols(int n) {
  std::vector<char> sym;
  sym.reserve(n);
  for (int i = 1; i <= n; ++i) {
    if (i <= 9) {
      sym.push_back(static_cast<char>('0' + i));
    } else {
      sym.push_back(static_cast<char>('A' + (i - 10)));
    }
  }
  return sym;
}

unsigned long long factorial(int n) {
  unsigned long long res = 1;
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
  std::cout << "=== Demo ===" << std::endl;
  std::vector<char> in = {'1','2','3'};
  PMTree tree(in);

  auto all = getAllPerms(tree);
  std::cout << "All permutations for {1,2,3}:\n";
  for (const auto& perm : all) {
    for (char c : perm) std::cout << c;
    std::cout << " ";
  }
  std::cout << "\n";

  auto p1 = getPerm1(tree, 2);
  std::cout << "getPerm1(tree,2) = ";
  for (char c : p1) std::cout << c;
  std::cout << "\n";

  auto p2 = getPerm2(tree, 3);
  std::cout << "getPerm2(tree,3) = ";
  for (char c : p2) std::cout << c;
  std::cout << "\n\n";

  const int maxN = 10;
  const int repeats = 5;
  std::mt19937 rng(42);

  std::ofstream dataFile("experiment_data.txt");
  dataFile << "# n time_getAllPerms_ms time_getPerm1_ms time_getPerm2_ms\n";

  for (int n = 1; n <= maxN; ++n) {
    std::vector<char> symbols = makeSymbols(n);
    PMTree tree(symbols);
    unsigned long long totalPerms = factorial(n);

    double timeAll = measureTimeMs([&tree]() { getAllPerms(tree); });

    double time1 = 0.0;
    for (int r = 0; r < repeats; ++r) {
      int randomNum = std::uniform_int_distribution<int>(1, static_cast<int>(totalPerms))(rng);
      time1 += measureTimeMs([&tree, randomNum]() { getPerm1(tree, randomNum); });
    }
    time1 /= repeats;

    double time2 = 0.0;
    for (int r = 0; r < repeats; ++r) {
      int randomNum = std::uniform_int_distribution<int>(1, static_cast<int>(totalPerms))(rng);
      time2 += measureTimeMs([&tree, randomNum]() { getPerm2(tree, randomNum); });
    }
    time2 /= repeats;

    std::cout << "n=" << n
              << " | getAllPerms: " << timeAll << " ms"
              << " | getPerm1: " << time1 << " ms"
              << " | getPerm2: " << time2 << " ms\n";

    dataFile << n << " " << timeAll << " " << time1 << " " << time2 << "\n";
  }
  dataFile.close();

  std::ofstream gnuScript("plot_script.gp");
  gnuScript << "set terminal png size 800,600 enhanced font 'Arial,12'\n";
  gnuScript << "set output 'result/plot.png'\n";
  gnuScript << "set logscale y\n";
  gnuScript << "set xlabel 'n (размер алфавита)'\n";
  gnuScript << "set ylabel 'время, мс'\n";
  gnuScript << "set title 'Зависимость времени работы функций от n'\n";
  gnuScript << "set grid\n";
  gnuScript << "plot 'experiment_data.txt' using 1:2 with linespoints title 'getAllPerms', \\\n";
  gnuScript << "     'experiment_data.txt' using 1:3 with linespoints title 'getPerm1', \\\n";
  gnuScript << "     'experiment_data.txt' using 1:4 with linespoints title 'getPerm2'\n";
  gnuScript.close();

  system("mkdir -p result");
  int ret = system("gnuplot plot_script.gp");
  if (ret == 0) {
    std::cout << "\nГрафик сохранён в result/plot.png\n";
  } else {
    std::cerr << "\nОшибка: не удалось запустить gnuplot. Убедитесь, что gnuplot установлен.\n";
    std::cerr << "Файл с данными: experiment_data.txt\n";
  }

  return 0;
}
