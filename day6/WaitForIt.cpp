#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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

std::vector<int> splitIntoVector(std::string line)
{
    std::string parsed = line.substr(line.find(':') + 1, line.length() - line.find(':') + 1);
    std::istringstream iss(parsed);
    std::vector<int> numbers;
    int num;
    while (iss >> num)
    {
        numbers.push_back(num);
    }

    return numbers;
}

int partOne(std::vector<std::string> &lines)
{
    int product = 1;

    auto times = splitIntoVector(lines[0]);
    auto distances = splitIntoVector(lines[1]);

    for (int i = 0; i < times.size(); i++)
    {
        int time = times[i];
        int distance = distances[i];
        int ways = 0;

        for (int j = 1; j < time - 1; j++)
        {
            int remainingTime = time - j;
            int distance = j * remainingTime;
            if (distance > distances[i])
            {
                ways++;
            }
        }
        product *= ways;
    }

    return product;
}

long long splitIntoNumber(std::string line)
{
    std::string parsed = line.substr(line.find(':') + 1, line.length() - line.find(':') + 1);
    std::istringstream iss(parsed);

    std::string num;
    std::string curr;

    while (iss >> curr)
    {
        num += curr;
    }

    return std::stoll(num);
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;

    long long time = splitIntoNumber(lines[0]);
    long long distance = splitIntoNumber(lines[1]);

    for (long long j = 1; j < time - 1; j++)
    {
        long long remainingTime = time - j;
        long long distanceTravelled = j * remainingTime;
        if (distanceTravelled > distance)
        {
            sum++;
        }
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("WaitForIt.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}
