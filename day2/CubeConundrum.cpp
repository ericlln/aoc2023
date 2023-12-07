#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

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

int checkGames(auto &tokens, int id)
{
    for (int i = 1; i < tokens.size(); i++)
    {
        std::string count;

        // 9 re, 10 b, etc.
        std::string substring = tokens[i].substr(1, 4);

        for (char c : substring)
        {
            if (std::isdigit(c))
            {
                count += c;
            }
            else
            {
                switch (c)
                {
                case 'r':
                    if (stoi(count) > 12)
                    {
                        return 0;
                    }
                    break;
                case 'g':
                    if (stoi(count) > 13)
                    {
                        return 0;
                    }
                    break;
                case 'b':
                    if (stoi(count) > 14)
                    {
                        return 0;
                    }
                    break;
                default:
                    continue;
                    break;
                }
            }
        }
    }

    return id;
}

// 12 red, 13 green, 14 blue
int partOne(std::vector<std::string> &lines)
{
    int sum = 0;

    for (const auto &line : lines)
    {
        // Split by :;, delimitors
        std::regex re("[\\:;,]");
        std::sregex_token_iterator first{line.begin(), line.end(), re, -1}, last;
        std::vector<std::string> tokens{first, last};

        // Get game ID
        std::string idString;
        for (char c : tokens[0].substr(5, tokens[0].length())) // Remove "Game "
        {
            if (std::isdigit(c))
            {
                idString += c;
            }
        }
        int id = stoi(idString);

        sum += checkGames(tokens, id);
    }

    return sum;
}

int getPower(auto &tokens)
{
    int maxR = -1;
    int maxG = -1;
    int maxB = -1;
    // Don't need game ID anymore
    for (int i = 1; i < tokens.size(); i++)
    {
        // 9 re, 10 b, etc.
        std::string substring = tokens[i].substr(1, 4);
        std::string count;

        for (char c : substring)
        {
            if (std::isdigit(c))
            {
                count += c;
            }
            else
            {
                switch (c)
                {
                case 'r':
                    maxR = std::max(stoi(count), maxR);
                    break;
                case 'g':
                    maxG = std::max(stoi(count), maxG);
                    break;
                case 'b':
                    maxB = std::max(stoi(count), maxB);
                    break;
                default:
                    continue;
                    break;
                }
                count = "0";
            }
        }
    }
    return maxR * maxG * maxB;
}

int partTwo(std::vector<std::string> &lines)
{
    int sum = 0;

    for (const auto &line : lines)
    {
        // Split by :;, delimitors
        std::regex re("[\\:;,]");
        std::sregex_token_iterator first{line.begin(), line.end(), re, -1}, last;
        std::vector<std::string> tokens{first, last};

        sum += getPower(tokens);
    }

    return sum;
}

int main()
{
    std::vector<std::string> lines;
    if (readInput("CubeConundrum.txt", lines))
    {
        std::cout << partOne(lines) << "\n";
        std::cout << partTwo(lines);
        return 0;
    }
    return -1;
}