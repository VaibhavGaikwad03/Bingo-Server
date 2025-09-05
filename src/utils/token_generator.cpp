//
// Created by vaibz on 05/09/25.
//
#include <chrono>
#include <sstream>
#include <random>
#include "../../include/utils/token_generator.h"

std::string generate_token(const size_t length)
{
    static std::random_device rd;
    static std::mt19937_64 gen(
        rd() ^ std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    static std::uniform_int_distribution<int> dist(0, 15);

    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i)
    {
        oss << std::hex << dist(gen);
    }
    return oss.str();
}
