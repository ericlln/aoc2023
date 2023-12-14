#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
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

struct Mapping
{
    long long destination;
    long long source;
    long long range;
};

std::vector<long long> splitIntoVector(std::string line)
{
    std::istringstream iss(line);
    std::vector<long long> numbers;
    long long num;
    while (iss >> num)
    {
        numbers.push_back(num);
    }

    return numbers;
}

std::vector<long long> getSeeds(std::string line)
{
    std::string parsed = line.substr(line.find(' ') + 1, line.length() - line.find(' ') + 1);
    return splitIntoVector(parsed);
}

void getMappings(std::map<int, std::vector<Mapping>> &allMappings, std::vector<std::string> &lines)
{
    std::vector<Mapping> tempMappings;
    int mappingCount = 0;
    for (int i = 3; i < lines.size(); i++) // Start at first mapping
    {
        if (lines[i].empty())
        {
            allMappings[mappingCount++] = tempMappings;
            tempMappings.clear();
        }

        if (!std::isdigit(lines[i][0]))
        {
            continue;
        }

        auto numbers = splitIntoVector(lines[i]);
        tempMappings.push_back({numbers[0], numbers[1], numbers[2]});
    }
}

int partOne(std::vector<std::string> &lines)
{
    int sum = 0;

    std::vector<long long> seeds = getSeeds(lines[0]);
    std::map<int, std::vector<Mapping>> allMappings;
    getMappings(allMappings, lines);

    for (long long &seed : seeds)
    {
        for (const auto &pair : allMappings)
        {
            int key = pair.first;
            const std::vector<Mapping> &mappings = pair.second;

            for (const auto &mapping : mappings)
            {
                if (mapping.source <= seed && seed < (mapping.source + mapping.range))
                {
                    seed = mapping.destination + (seed - mapping.source);
                    break;
                }
            }
        }
    }

    return *std::min_element(seeds.begin(), seeds.end());
}

// For each seed range, calculate resulting range (ranges if seed range is split between two different mappings)
// for any mappings in the first stage that overlap with the seed range, and move on to the next mapping stage
// with the new ranges, until the last mapping has been done.
std::vector<std::pair<long long, long long>> getNextRange(int num, std::vector<std::pair<long long, long long>> currRange, std::map<int, std::vector<Mapping>> &allMappings)
{
    if (num > 7) // 7 mapping stages in total
    {
        return currRange;
    }

    std::vector<Mapping> &mappings = allMappings[num];
    std::vector<std::pair<long long, long long>> temp;

    for (const auto &seed : currRange)
    {
        bool found = false;

        for (const auto &mapping : mappings)
        {
            long long overlapLeft = std::max(mapping.source, seed.first);
            long long overlapRight = std::min(mapping.source + mapping.range, seed.second - 1);

            if (overlapLeft < overlapRight)
            {
                temp.push_back({overlapLeft - mapping.source + mapping.destination, overlapRight - mapping.source + mapping.destination});

                found = true;
            }
        }

        if (!found)
        {
            temp.push_back(seed);
        }
    }

    // Don't need recursion but more organized like this
    return getNextRange(num + 1, temp, allMappings);
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;

    std::map<int, std::vector<Mapping>> allMappings;
    getMappings(allMappings, lines);

    std::vector<long long> inputs = getSeeds(lines[0]);
    std::vector<std::pair<long long, long long>> seeds;
    for (int i = 0; i < inputs.size(); i += 2)
    {
        seeds.push_back(std::make_pair(inputs[i], inputs[i] + inputs[i + 1]));
    }

    std::vector<std::pair<long long, long long>> finalLocations = getNextRange(0, seeds, allMappings);

    long long min = LLONG_MAX;

    for (const auto &pair : finalLocations) // Get minimum starting location
    {
        if (pair.first < min)
        {
            min = pair.first;
        }
    }

    return min;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("IfYouGiveASeedAFertilizer.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}
