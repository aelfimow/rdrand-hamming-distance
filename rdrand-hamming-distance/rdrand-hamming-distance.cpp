#include "parser.hpp"
#include "rdrand.hpp"

#include <array>
#include <bit>
#include <cmath>
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
    constexpr long double expected_mean = 32.0L;

    parser const cli_parser{ };
    std::size_t const n_rand_numbers = (argc >= 2) ?
        cli_parser.positive_count(argv[1]).value_or(default_rand_numbers) :
        default_rand_numbers;

    rdrand const rng{ };
    std::uint64_t prev_value = rng();

    constexpr int max_distance = std::numeric_limits<std::uint64_t>::digits;
    static_assert(max_distance == 64, "Expected 64-bit unsigned integer");

    constexpr std::size_t total_batches = 10U;
    using Histogram = std::array<std::array<std::uint64_t, static_cast<std::size_t>(max_distance) + 1U>, total_batches>;
    Histogram histogram{ };

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

    std::array<std::uint64_t, static_cast<std::size_t>(max_distance) + 1U> total_histogram{ };
    std::uint64_t overall_distances_sum = 0U;
    std::uint64_t overall_total_distances = 0U;

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
            total_histogram[d] += count;

            std::cout << d << ": " << count << '\n';
        }

        overall_distances_sum += distances_sum;
        overall_total_distances += total_distances;

        std::cout << "Sum: " << distances_sum << '\n';

        if (total_distances == n_rand_numbers)
        {
            long double const arithm_mean =
                static_cast<long double>(distances_sum) / static_cast<long double>(total_distances);
            long double const diff_from_expected = arithm_mean - expected_mean;

            long double sq_diff_sum = 0.0L;
            for (std::size_t d = 0U; d < histogram[batch].size(); ++d)
            {
                long double const diff = static_cast<long double>(d) - arithm_mean;
                sq_diff_sum += (diff * diff) * static_cast<long double>(histogram[batch][d]);
            }

            long double const std_dev = std::sqrt(sq_diff_sum / static_cast<long double>(total_distances));

            std::cout << "Mean: " << arithm_mean << '\n';
            std::cout << "Diff from 32.0: " << diff_from_expected << '\n';
            std::cout << "StdDev: " << std_dev << '\n';
        }
        else
        {
            std::cout << "Mean: n/a\n";
            std::cout << "Diff from 32.0: n/a\n";
            std::cout << "StdDev: n/a\n";
        }
    }

    std::uint64_t const expected_overall_distances = static_cast<std::uint64_t>(total_batches) * n_rand_numbers;
    std::cout << "\nOverall summary:\n";
    std::cout << "Total sum: " << overall_distances_sum << '\n';

    if (overall_total_distances == expected_overall_distances)
    {
        long double const overall_mean =
            static_cast<long double>(overall_distances_sum) / static_cast<long double>(overall_total_distances);
        long double const overall_diff_from_expected = overall_mean - expected_mean;

        long double overall_sq_diff_sum = 0.0L;
        for (std::size_t d = 0U; d < total_histogram.size(); ++d)
        {
            long double const diff = static_cast<long double>(d) - overall_mean;
            overall_sq_diff_sum += (diff * diff) * static_cast<long double>(total_histogram[d]);
        }

        long double const overall_std_dev =
            std::sqrt(overall_sq_diff_sum / static_cast<long double>(overall_total_distances));

        std::cout << "Overall mean: " << overall_mean << '\n';
        std::cout << "Overall diff from 32.0: " << overall_diff_from_expected << '\n';
        std::cout << "Overall StdDev: " << overall_std_dev << '\n';
    }
    else
    {
        std::cout << "Overall mean: n/a\n";
        std::cout << "Overall diff from 32.0: n/a\n";
        std::cout << "Overall StdDev: n/a\n";
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