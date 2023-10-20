#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "jaro-winkler.h"


std::string read_file(const std::string& filepath) {
  std::ifstream file(filepath, std::ios::binary | std::ios::ate);
  std::ifstream::pos_type fileSize = file.tellg();
  std::string buffer(fileSize, '\0');
  file.seekg(0);
  file.read(&buffer[0], fileSize);

  return buffer;
}

std::vector<std::string> get_files_list(const std::string& dir) {
  std::vector<std::string> files;

  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    if (std::filesystem::is_regular_file(entry)) {
      files.push_back(entry.path().string());
    }
  }

  return files;
}

void compare_directories(const std::string& dir1, const std::string& dir2, double TonysPercent) {
  std::vector<std::string> files1 = get_files_list(dir1);
  std::vector<std::string> files2 = get_files_list(dir2);

  for (const auto& file_1 : files1) {
    bool foundIdentical = false;
    bool foundSimilar = false;

    for (const auto& file_2 : files2) {
      std::string buffer1 = read_file(file_1);
      std::string buffer2 = read_file(file_2);

      if (buffer1 == buffer2) {
        std::cout << "Идентичные файлы: " << file_1 << " и " << file_2 << '\n';
        foundIdentical = true;
        break;
      } else {

        double similarity = get_file_congruence(buffer1, buffer2);

        if (similarity >= TonysPercent) {
          std::cout << "Похожие файлы: " << file_1 << " и " << file_2 << " c % сходства = " << similarity << '\n';
          foundSimilar = true;
          break;
        }
      }
    }

    if (!foundIdentical && !foundSimilar) {
      std::cout << "Файл есть только в директории 1: " << file_1 << '\n';
    }
  }

  for (const auto& file2 : files2) {
    bool foundSmth = false;

    for (const auto& file1 : files1) {
      if (file2 == file1) {
        foundSmth = true;
        break;
      }
    }

    if (!foundSmth) {
      std::cout << "Файл есть только в директории 2: " << file2 << '\n';
    }
  }
}

int main() {
  std::string dir1;
  std::string dir2;

  std::cout << "Введите путь к первой директории" << '\n';
  std::cin >> dir1;

  if (!std::filesystem::exists(dir1)) {
    std::cerr << "Директория не существует: " << dir1 << '\n';
    return 1;
  }

  std::cout << "Введите путь ко второй директории" << '\n';
  std::cin >> dir2;


  double TonysPercent;
  std::cin >> TonysPercent;

  compare_directories(dir1, dir2, TonysPercent);

  return 0;
}