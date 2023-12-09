#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <set>

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

bool isSpecial(char c)
{
    return !(std::isdigit(c) || c == '.');
}

struct Number
{
    int y;
    int x;
    std::string num;
};

void getSpecialSymbolsAndNumbers(std::vector<std::string> &lines, std::vector<std::pair<int, int>> &special, std::vector<Number> &numbers)
{
    std::string num;

    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].length(); j++)
        {
            char c = lines[i][j];
            if (isSpecial(c))
            {
                special.push_back(std::pair<int, int>(i, j));
            }
            else if (std::isdigit(c))
            {
                num += c;
                if (!std::isdigit(lines[i][j + 1]))
                {
                    int adjustedIndex = j - num.length() + 1;
                    numbers.push_back({i, adjustedIndex, num});
                    num = "";
                }
            }
        }
    }
}

int partOne(std::vector<std::string> &lines)
{
    int sum = 0;

    std::vector<std::pair<int, int>> special;
    std::vector<Number> numbers;

    getSpecialSymbolsAndNumbers(lines, special, numbers);

    for (const auto &[i, j] : special) // each special symbol
    {
        for (const auto &[y, x, num] : numbers) // each first index of a number
        {
            for (int k = 0; k < num.length(); k++) // each character of the number
            {
                if (std::abs(j - (x + k)) <= 1)
                {
                    if (std::abs(i - y) <= 1)
                    {
                        sum += stoi(num); // symbol found in 3x3 area around symbol
                        break;
                    }
                }
            }
        }
    }

    return sum;
}

bool isStar(char c)
{
    return c == '*';
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;

    std::vector<std::pair<int, int>> special;
    std::vector<Number> numbers;

    getSpecialSymbolsAndNumbers(lines, special, numbers);

    for (const auto &[i, j] : special) // each special symbol
    {
        int tempNum = 0;

        for (const auto &[y, x, num] : numbers) // each first index of a number
        {
            for (int k = 0; k < num.length(); k++) // each character of the number
            {
                if (std::abs(j - (x + k)) <= 1)
                {
                    if (std::abs(i - y) <= 1)
                    {
                        if (tempNum == 0)
                        {
                            tempNum = stoi(num); // one symbol found in 3x3 area around symbol, keep for later
                            break;
                        }
                        else
                        {
                            sum += tempNum * stoi(num); // both symbols found, add to sum
                            tempNum = 0;
                        }
                    }
                }
            }
        }
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("GearRatios.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}