#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>

// map <(n, k, target_sum)>
std::map<std::tuple<int, int, int>, std::vector<int>> __cache;


// auxiliary recursive function
std::vector<int> _target_sum(int n, int k, int target, std::vector<int> allowed);

/* Solve subset sum fixed size problem using lazy recursion.
 *
 * @param n - amount of least numbers from $allowed$, which function can use
 * @param k - required size of subset
 * @param target - target sum
 * @param allowed - constant array of allowed numbers
 */
std::vector<int> target_sum(int n, int k, int target, std::vector<int> allowed);


int main(int argc, char const *argv[]) {
  std::string filename = "input.txt";
  if (argc > 1)
    filename = argv[1];

  std::ifstream infile (filename, std::ifstream::in);
  if (!infile) {
    std::cout << "File not found" << '\n';
    return 0;
  }
  int k, n, target;
  std::vector<int> allowed;

  infile >> k;
  infile >> target;
  while (1) {
    int tmp;
    infile >> tmp;
    if (infile.eof())
      break;
    n++;
    allowed.push_back(tmp);
  }

  // sort in order to be able to use binary search
  std::sort(allowed.begin(), allowed.end());

  auto res = target_sum(n, k, target, allowed);

  for (auto el : res) {
    std::cout << el << '\n';
  }

  return 0;
}

std::vector<int> target_sum(int n, int k, int target, std::vector<int> allowed){

  // caching which provides lazy recursion
  auto pars = std::make_tuple(n, k, target);
  auto cached = __cache.find(pars);
  if (cached == __cache.end()) {
    auto res = _target_sum(n, k, target, allowed);
    __cache[pars] = res;
    return res;
  }
  return cached->second;
};


std::vector<int> _target_sum(int n, int k, int target, std::vector<int> allowed){

  std::vector<int> res;

  // border conditions
  if (k < 1 || k > n) {
    return res;
  } else if (k == 1) {

    // optimization using binary search for the last required number
    if (std::binary_search(allowed.begin(), allowed.begin() + n, target)) {
      res.push_back(target);
    }
    return res;
  }

  // try find same sum without the last number
  res = target_sum(n-1, k, target, allowed);
  if (!res.empty())
    return res;

  // try without the last number, including it in
  // the result sum
  res = target_sum(n-1, k-1, target - allowed[n-1], allowed);
  if (res.empty())
    return res;
  else {
    res.push_back(allowed[n-1]);
    return res;
  }
}
