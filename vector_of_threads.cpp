#compile it using "gcc vector_of_threads.cpp -o vector_of_threads -lstdc++ -lpthread" in linux
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>



// A demo for creating two threads
// Run this using one of the following methods:
//  1. With bazel: bazel run src/main:vector_of_threads_main
//  2. With plain g++: g++ -std=c++17 -lpthread src/main/vector_of_threads_main.cc  -I ./
void AccumulateRange(uint64_t &sum, uint64_t start, uint64_t end) {
  sum = 0;
  for (uint64_t i = start; i < end; i++) {
    sum += i;
  }
}

template <class T>
void PrintVector(std::vector<T> input) {
  std::cout << "{ ";
  unsigned int count = 0;
  for (auto n : input) {
    count++;
    std::cout << n;
    if (count < input.size()) std::cout << ", ";
  }
  std::cout << " }" << std::endl;
}
int main() {
  const int number_of_threads = 4;
  uint64_t number_of_elements = 1000 * 1000* 1000;
  uint64_t step = number_of_elements / number_of_threads;
  std::vector<std::thread> threads;
  std::vector<uint64_t> partial_sums(number_of_threads);

  for (uint64_t i = 0; i < number_of_threads; i++) {
    threads.push_back(std::thread(AccumulateRange, std::ref(partial_sums[i]),
                                  i * step, (i + 1) * step));
  }

  for (std::thread &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  uint64_t total =
      std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));

  PrintVector(partial_sums);
  std::cout << "total: " << total << std::endl;

  return 0;
}
