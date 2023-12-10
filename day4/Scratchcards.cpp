#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <cmath>
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

std::vector<int> splitIntoVector(std::string numbers)
{
    std::vector<int> v;

    int i = 0;
    while (i < numbers.size())
    {
        int spacePos = numbers.find(" ", i);
        if (spacePos < 0)
        {
            spacePos = numbers.size();
        }
        std::string numStr = numbers.substr(i, spacePos - i);
        v.push_back(std::stoi(numStr));
        i = spacePos + 1;
    }
    return v;
}

std::vector<int> getIntersections(std::string line)
{
    std::vector<int> intersections;
    std::regex pattern("\\s{2,}"); // Two or more consecutive whitespaces

    std::string winning = line.substr(line.find(":") + 1, line.find("|") - line.find(":") - 1);
    winning = std::regex_replace(winning, pattern, " ");
    winning = winning.substr(1, winning.length() - 2); //
    std::vector<int> winningNums = splitIntoVector(winning);

    std::string numbers = line.substr(line.find("|") + 1);
    numbers = std::regex_replace(numbers, pattern, " ");
    numbers = numbers.substr(1, numbers.length() - 1); //
    std::vector<int> ourNums = splitIntoVector(numbers);

    std::sort(winningNums.begin(), winningNums.end());
    std::sort(ourNums.begin(), ourNums.end());

    std::set_intersection(winningNums.begin(), winningNums.end(),
                          ourNums.begin(), ourNums.end(),
                          back_inserter(intersections));

    return intersections;
}

int partOne(std::vector<std::string> &lines)
{
    int sum = 0;

    for (const auto &line : lines)
    {
        std::vector<int> intersections = getIntersections(line);
        if (intersections.size() > 0)
        {
            sum += pow(2, intersections.size() - 1);
        }
    }

    return sum;
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;
    std::regex pattern("\\s{2,}"); // Two or more consecutive whitespaces
    std::vector<std::pair<int, int>> cards;

    for (const auto &line : lines)
    {
        std::vector<int> intersections = getIntersections(line);
        cards.push_back(std::make_pair(intersections.size(), 1));
    }

    // first is number of winning nums, second is number of cards
    for (int j = 0; j < cards.size(); j++)
    {
        for (int i = 0; i < cards[j].first; i++)
        {
            cards[j + i + 1].second += cards[j].second;
        }
        sum += cards[j].second;
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("Scratchcard.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}