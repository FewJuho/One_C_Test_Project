#include <iostream>
#include <string>
#include <algorithm>

const double JWPREFIXCRITERIA = 0.75;
double jaro_distance(const std::string& str1, const std::string& str2) {
  int len1 = str1.length();
  int len2 = str2.length();

  if (str1 == str2) {
    return 1;
  }

  if (len1 == 0 && len2 == 0)
    return 1.0;

  int match_distance = std::max(len1, len2) / 2 - 1;
  bool* str1_matches = new bool[len1]();
  bool* str2_matches = new bool[len2]();

  int matches = 0;
  int transpositions = 0;

  for (int i = 0; i < len1; ++i) {
    int start = std::max(0, i - match_distance);
    int end = std::min(i + match_distance + 1, len2);

    for (int j = start; j < end; ++j) {
      if (!str2_matches[j] && str1[i] == str2[j]) {
        str1_matches[i] = true;
        str2_matches[j] = true;
        ++matches;
        break;
      }
    }
  }

  int k = 0;
  for (int i = 0; i < len1; ++i) {
    if (str1_matches[i]) {
      while (!str2_matches[k])
        ++k;
      if (str1[i] != str2[k])
        ++transpositions;
      ++k;
    }
  }

  delete[] str1_matches;
  delete[] str2_matches;

  if (matches == 0)
    return 0;

  double jd = (static_cast<double>(matches) / len1 +
      static_cast<double>(matches) / len2 +
      (static_cast<double>(matches - transpositions) / matches)) / 3.0;

  return jd;
}

double jaro_winkler_distance(const std::string& str1, const std::string& str2, double scaling_factor = 0.1) {
  double jd = jaro_distance(str1, str2);

  if (jd > JWPREFIXCRITERIA) {
    int prefix_length = 0;
    while (prefix_length < std::min(str1.length(), str2.length()) && str1[prefix_length] == str2[prefix_length])
      ++prefix_length;

    jd += prefix_length * scaling_factor * (1.0 - jd);
  }

  return jd;
}

double get_file_congruence(std::string str1, std::string str2) {

  double sum_jaro_wink = 0;
  std::string max_str = (std::max(str1.length(), str2.length()) == str1.length()) ? str1: str2;
  std::string min_str = (std::max(str1.length(), str2.length()) == str1.length()) ? str2: str1;
  int count_iters = static_cast<int>(max_str.length() - min_str.length());


  for (int i = 0; i <= count_iters; ++i) {
    auto str_iter = max_str.substr(i, min_str.length());
    std::cout << str_iter << ' ' << min_str << '\n';
    sum_jaro_wink += jaro_winkler_distance(min_str, str_iter);
  }
  
  return sum_jaro_wink / static_cast<double>((count_iters + 1));
}