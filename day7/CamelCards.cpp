#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

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

const std::unordered_map<std::string, int> labels = {{"A", 14},
                                                     {"K", 13},
                                                     {"Q", 12},
                                                     {"J", 11},
                                                     {"T", 10},
                                                     {"9", 9},
                                                     {"8", 8},
                                                     {"7", 7},
                                                     {"6", 6},
                                                     {"5", 5},
                                                     {"4", 4},
                                                     {"3", 3},
                                                     {"2", 2}};

// No two hands are completely the same
bool compareLabels(const std::vector<int> &a, const std::vector<int> &b, int i)
{
    return a[i] == b[i] ? compareLabels(a, b, i + 1) : a[i] > b[i];
}

// five > four > full house > three > two pair > one pair > high
bool comparePairs(const std::pair<std::vector<int>, std::vector<int>> &a,
                  const std::pair<std::vector<int>, std::vector<int>> &b)
{
    int maxA = a.first[0];
    int maxB = b.first[0];

    if (maxA == 2 && maxB == 2 || maxA == 3 && maxB == 3)
    {
        int pairCountA = a.first[1];
        int pairCountB = b.first[1];
        if (pairCountA != pairCountB)
        {
            return pairCountA > pairCountB;
        }
    }

    if (maxA == maxB)
    {
        return compareLabels(a.second, b.second, 0);
    }

    return maxA > maxB;
}

std::vector<std::pair<std::vector<int>, std::vector<int>>> sortHands(std::vector<std::vector<int>> &hands)
{
    std::vector<std::pair<std::vector<int>, std::vector<int>>> handsWithCount;

    for (std::vector<int> hand : hands)
    {
        std::unordered_map<int, int> elements;
        for (int i = 0; i < 5; i++)
        {
            elements[hand[i]]++;
        }

        std::vector<int> counts;
        for (const auto &element : elements)
        {
            counts.push_back(element.second);
        }

        std::sort(counts.rbegin(), counts.rend());
        handsWithCount.push_back({counts, hand});
    }

    std::sort(handsWithCount.begin(), handsWithCount.end(), comparePairs);

    return handsWithCount;
}

std::vector<std::vector<int>> getHands(std::vector<std::string> &lines)
{
    std::vector<std::vector<int>> hands;
    for (std::string &line : lines)
    {
        std::string handString = line.substr(0, line.find(" "));
        std::string bidString = line.substr(line.find(" ") + 1, line.length() - line.find(" ") + 1);
        std::vector<int> hand;

        while (!handString.empty())
        {
            for (auto const &[string, value] : labels)
            {
                if (handString.starts_with(string))
                {
                    hand.push_back(value);
                }
            }
            handString.erase(0, 1);
        }

        hand.push_back(stoi(bidString)); // store bid as last element of vector (index 5)
        hands.push_back(hand);
    }
    return hands;
}

int partOne(std::vector<std::string> &lines)
{
    int sum = 0;

    std::vector<std::vector<int>> hands = getHands(lines);
    std::vector<std::pair<std::vector<int>, std::vector<int>>> handsWithCount = sortHands(hands);
    int rank = handsWithCount.size();

    for (int i = 0; i < handsWithCount.size(); i++)
    {
        sum += rank * handsWithCount[i].second[5];
        rank--;
    }

    return sum;
}

const std::unordered_map<std::string, int> labelsWithJoker = {{"A", 14},
                                                              {"K", 13},
                                                              {"Q", 12},
                                                              {"J", 0},
                                                              {"T", 10},
                                                              {"9", 9},
                                                              {"8", 8},
                                                              {"7", 7},
                                                              {"6", 6},
                                                              {"5", 5},
                                                              {"4", 4},
                                                              {"3", 3},
                                                              {"2", 2}};

std::vector<std::pair<std::vector<int>, std::vector<int>>> sortHandsWithJoker(std::vector<std::vector<int>> &hands)
{
    std::vector<std::pair<std::vector<int>, std::vector<int>>> handsWithCount;

    for (std::vector<int> hand : hands)
    {
        std::unordered_map<int, int> elements;
        for (int i = 0; i < 5; i++)
        {
            elements[hand[i]]++;
        }

        int jacks = elements[0]; // jacks have a key of 0
        elements.erase(0);

        std::vector<int> counts;
        for (const auto &element : elements)
        {
            counts.push_back(element.second);
        }
        std::sort(counts.rbegin(), counts.rend());

        if (jacks == 5)
        {
            counts = {5}; // 5 jacks
        }

        if (jacks != 5)
        {
            counts[0] += jacks;
        }

        handsWithCount.push_back({counts, hand});
    }

    std::sort(handsWithCount.begin(), handsWithCount.end(), comparePairs);

    return handsWithCount;
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;

    std::vector<std::vector<int>> hands;

    for (std::string &line : lines)
    {
        std::string handString = line.substr(0, line.find(" "));
        std::string bidString = line.substr(line.find(" ") + 1, line.length() - line.find(" ") + 1);
        std::vector<int> hand;

        while (!handString.empty())
        {
            for (auto const &[string, value] : labelsWithJoker)
            {
                if (handString.starts_with(string))
                {
                    hand.push_back(value);
                }
            }
            handString.erase(0, 1);
        }

        hand.push_back(stoi(bidString)); // store bid as last element of vector (index 5)
        hands.push_back(hand);
    }

    std::vector<std::pair<std::vector<int>, std::vector<int>>> handsWithCount = sortHandsWithJoker(hands);
    int rank = handsWithCount.size();

    for (int i = 0; i < handsWithCount.size(); i++)
    {
        sum += rank * handsWithCount[i].second[5];
        rank--;
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("CamelCards.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}
