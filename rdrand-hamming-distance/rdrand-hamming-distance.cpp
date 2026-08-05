#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <bit>

#include "rdrand.hpp"

int main(int argc, char* argv[])
try
{
#if 0
    if (argc != 2)
    {
        std::cerr << "Error: number of random numbers missing" << std::endl;
        throw std::invalid_argument("Missing argument");
    }
#endif

    size_t n_rand_numbers = 20U;

#if 0
    {
        std::stringstream ss{ argv[1] };
        ss >> n_rand_numbers;
    }
#endif

    rdrand rng{ };

    std::vector<uint64_t> rand_numbers(n_rand_numbers);

    for (auto &value : rand_numbers)
    {
        value = rng();
    }

    for (auto const &value : rand_numbers)
    {
        std::cout << std::hex << value << std::endl;
    }

    std::vector<int> distances{ };
    bool skip_first = true;
    uint64_t prev_value = 0U;

    for (auto const& value: rand_numbers)
    {
        if (skip_first)
        {
            prev_value = value;
            skip_first = false;
        }
        else
        {
            uint64_t const diff = (value xor prev_value);
            int const distance = std::popcount(diff);
            distances.push_back(distance);
            prev_value = value;
        }
    }

    for (auto const& distance: distances)
    {
        std::cout << std::dec << distance << std::endl;
    }

    return EXIT_SUCCESS;
}
catch (std::exception& exc)
{
    std::cerr << "Exception: " << exc.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Error, exception" << std::endl;
    return EXIT_FAILURE;
}