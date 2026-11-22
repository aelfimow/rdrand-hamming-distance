#include "parser.hpp"
#include "rdrand.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

int main(int argc, char* argv[])
try
{
    constexpr std::size_t default_rand_numbers = 10'000'000U;

    parser const cli_parser { };
    std::size_t const n_rand_numbers = (argc >= 2) ?
        cli_parser.positive_count(argv[1]).value_or(default_rand_numbers) :
        default_rand_numbers;

    rdrand const rng { };
    std::uint64_t prev_value = rng();

    constexpr int max_distance = std::numeric_limits<std::uint64_t>::digits;
    static_assert(max_distance == 64, "Expected 64-bit unsigned integer");

    constexpr std::size_t total_batches = 10U;
    using Histogram = std::array<std::array<std::uint64_t, static_cast<std::size_t>(max_distance) + 1U>, total_batches>;
    Histogram histogram { };

    for (std::size_t batch = 0U; batch < total_batches; ++batch)
    {
        std::cout << "Computing batch " << batch << " with " << n_rand_numbers << " random numbers\n";

        for (std::size_t i = 0U; i < n_rand_numbers; ++i)
        {
            std::uint64_t const value = rng();

            // Compute Hamming distance between consecutive 64-bit values:
            std::uint64_t const diff = value ^ prev_value;
            int const distance = std::popcount(diff);

            histogram[batch][static_cast<std::size_t>(distance)] += 1U;
            prev_value = value;
        }
    }

    std::cout << "RNG retries: " << rng.retries() << '\n';

    for (std::size_t batch = 0U; batch < total_batches; ++batch)
    {
        std::cout << "\nBatch: " << batch << '\n';
        std::uint64_t distances_sum = 0U;
        std::uint64_t total_distances = 0U;

        for (std::size_t d = 0U; d < histogram[batch].size(); ++d)
        {
            std::uint64_t const count = histogram[batch][d];
            distances_sum += static_cast<std::uint64_t>(d) * count;
            total_distances += count;

            std::cout << d << ": " << count << '\n';
        }

        std::cout << "Sum: " << distances_sum << '\n';

        if (total_distances == n_rand_numbers)
        {
            long double const arithm_mean =
                static_cast<long double>(distances_sum) / static_cast<long double>(total_distances);
            std::cout << "Mean: " << arithm_mean << '\n';
        }
        else
        {
            std::cout << "Mean: n/a\n";
        }
    }

    std::cout.flush();

    return EXIT_SUCCESS;
}
catch (std::exception const& exc)
{
    std::cerr << "Exception: " << exc.what() << '\n';
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Error: unknown exception\n";
    return EXIT_FAILURE;
}