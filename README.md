# rdrand-hamming-distance
Compute Hamming distance of random numbers returned by RDRAND instruction of x86 CPUs

## Overview
This project generates consecutive 64-bit hardware random numbers using the x86/x64 `RDRAND` instruction (`_rdrand64_step`) and computes the bitwise Hamming distance (`std::popcount(value ^ prev_value)`) between successive values across multiple batches.

For each batch, the program builds a histogram of Hamming distances (`0` to `64`), prints the distribution, sums all distances, and computes the arithmetic mean, the deviation from the expected value (`32.0`), and the standard deviation (expected theoretical values for uniform 64-bit random numbers: mean `32.0`, standard deviation `4.0`). It also reports the total number of `RDRAND` hardware retries and prints an overall summary across all batches.

## Project Structure
* `rdrand.hpp` / `rdrand.cpp` – RAII-style wrapper class around `_rdrand64_step` with configurable retry limit (default: `10` retries), cumulative retry counter (`retries()`), and `std::runtime_error` exception handling if hardware entropy is exhausted.
* `parser.hpp` / `parser.cpp` – Command-line argument parser (`parser::positive_count`) using `std::from_chars` and `std::optional<std::size_t>` to validate positive integer input without locale overhead or exceptions.
* `rdrand-hamming-distance.cpp` – Main benchmark and statistical evaluation loop running `10` batches and computing per-batch as well as overall histograms, distance sums, arithmetic means, deviations from `32.0`, and standard deviations.
* `rdrand-hamming-distance.slnx` / `rdrand-hamming-distance.vcxproj` – Visual Studio solution and project files (`x64` configurations).

## Requirements
* **Architecture:** 64-bit x86 (`x64`) CPU supporting the `RDRAND` instruction (`_rdrand64_step`).
* **Compiler / Standard:** C++20 or newer (required for `<bit>` / `std::popcount`, `std::from_chars`, `std::optional`, and `std::string_view`).
* **Build Environment:** Visual Studio (`x64` configurations in `rdrand-hamming-distance.slnx` / `rdrand-hamming-distance.vcxproj`) or a C++20-capable compiler supporting `<immintrin.h>` and `-mrdrnd` (GCC/Clang).

## Usage
```bash
rdrand-hamming-distance [number_of_random_numbers]
```

* `number_of_random_numbers` *(optional)*: Positive integer specifying how many 64-bit random numbers are sampled per batch (across `10` batches).
* If omitted or invalid (e.g., `0`, negative, or non-numeric), the program falls back to the default of **`10,000,000`** random numbers per batch (`100,000,000` samples in total).

### Example
```bash
# Run with default (10,000,000 numbers per batch)
./rdrand-hamming-distance

# Run with 1,000,000 numbers per batch
./rdrand-hamming-distance 1000000
```

## Example Output
Sample output from a benchmark run evaluating 10,000,000 consecutive 64-bit `RDRAND` samples in total:

```text
Overall summary:
Total sum: 319978374
Overall mean: 31.9978
Overall diff from 32.0: -0.0021626
Overall StdDev: 3.99956
```

For uniform, independent 64-bit random numbers, the Hamming distance follows a binomial distribution $B(64, 0.5)$ with a theoretical mean of `32.0` and a theoretical standard deviation of `4.0`.
