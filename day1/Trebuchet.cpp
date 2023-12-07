#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

bool readInput(const std::string &name, std::vector<std::string> &lines)
{
    std::ifstream input(name);

    if (!input)
    {
        return false;
    }

    std::string line;
    while (std::getline(input, line))
    {
        lines.push_back(line);
    }

    input.close();
    return true;
}

int partOne(std::vector<std::string> &lines)
{
    int sum = 0;
    constexpr std::string_view digits = "123456789";

    for (const auto &line : lines)
    {
        int first = line.at(line.find_first_of(digits)) - '0';
        int last = line.at(line.find_last_of(digits)) - '0';
        sum += first * 10 + last;
    }
    return sum;
}

int getFirstNumber(std::string line, std::map<std::string, int> &digits)
{
    while (!line.empty())
    {
        for (auto const &[string, value] : digits)
        {
            if (line.starts_with(string))
            {
                return value;
            }
        }
        line.erase(0, 1);
    }
    return 0;
}

int getLastNumber(std::string line, std::map<std::string, int> &digits)
{
    while (!line.empty())
    {
        for (auto const &[string, value] : digits)
        {
            if (line.ends_with(string))
            {
                return value;
            }
        }
        line.pop_back();
    }
    return 0;
}

int partTwo(std::vector<std::string> &lines)
{
    std::map<std::string, int> digits;
    digits = {{"1", 1},
              {"2", 2},
              {"3", 3},
              {"4", 4},
              {"5", 5},
              {"6", 6},
              {"7", 7},
              {"8", 8},
              {"9", 9},
              {"one", 1},
              {"two", 2},
              {"three", 3},
              {"four", 4},
              {"five", 5},
              {"six", 6},
              {"seven", 7},
              {"eight", 8},
              {"nine", 9}};

    int sum = 0;
    for (const auto &line : lines)
    {
        sum += getFirstNumber(line, digits) * 10 + getLastNumber(line, digits);
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("Trebuchet.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}