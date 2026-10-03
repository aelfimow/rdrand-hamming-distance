#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <bit>

#include "rdrand.hpp"

int main(int argc, char* argv[])
try
{
    if (argc != 2)
    {
        std::cerr << "Error: number of random numbers missing" << std::endl;
        throw std::invalid_argument("Missing argument");
    }

    size_t n_rand_numbers = 10000000U;

    {
        std::stringstream ss{ argv[1] };
        ss >> n_rand_numbers;
    }

    rdrand rng { };

    uint64_t prev_value = rng();
    uint64_t distances_sum = 0U;

    constexpr int max_distance = 64;
    constexpr size_t total_batches = 10U;
    uint64_t histogram[total_batches][max_distance + 1] = { };

    for (size_t batch = 0U; batch < total_batches; ++batch)
    {
        std::cout << "Computing batch " << batch << std::endl;

        for (size_t i = 0U; i < n_rand_numbers; ++i)
        {
            uint64_t const value = rng();

            // Compute Hamming distance:
            uint64_t const diff = (value xor prev_value);
            int const distance = std::popcount(diff);

            bool const distance_ok = ((distance >= 0) and (distance <= max_distance));
            if (distance_ok)
            {
                histogram[batch][distance] += 1U;
            }
            else
            {
                throw std::runtime_error("Distance out of range");
            }
#if 0
            std::cout << std::hex << prev_value << " " << value << ": " << std::dec << distance << std::endl;
#endif
            prev_value = value;

            distances_sum += distance;
        }
    }

    std::cout << "Sum: " << std::dec << distances_sum << std::endl;
    uint64_t const total_distances = (n_rand_numbers - 1U);

    long double const arithm_mean = (static_cast<long double>(distances_sum) / static_cast<long double>(total_distances));
    std::cout << "Mean: " << arithm_mean << std::endl;

    auto const rng_retries = rng.retries();
    std::cout << "RNG retries: " << rng_retries << std::endl;

    for (size_t batch = 0U; batch < total_batches; ++batch)
    {
        std::cout << "Batch: " << batch << std::endl;
        uint64_t n = 0U;

        for (uint64_t const& h : histogram[batch])
        {
            std::cout << n << ": " << h << std::endl;
            ++n;
        }
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