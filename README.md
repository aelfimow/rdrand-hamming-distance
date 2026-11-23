# rdrand-hamming-distance
Compute Hamming distance of random numbers returned by RDRAND instruction of x86 CPUs

## Overview
This project generates consecutive 64-bit hardware random numbers using the x86/x64 `RDRAND` instruction (`_rdrand64_step`) and computes the bitwise Hamming distance (`std::popcount(value ^ prev_value)`) between successive values across multiple batches.

For each batch, the program builds a histogram of Hamming distances (`0` to `64`), prints the distribution, sums all distances, computes the arithmetic mean (expected theoretical mean for uniform 64-bit random values: `32.0`), and reports the total number of `RDRAND` hardware retries.

## Project Structure
* `rdrand.hpp` / `rdrand.cpp` – RAII-style wrapper class around `_rdrand64_step` with configurable retry limit (default: `10` retries), cumulative retry counter (`retries()`), and `std::runtime_error` exception handling if hardware entropy is exhausted.
* `parser.hpp` / `parser.cpp` – Command-line argument parser (`parser::positive_count`) using `std::from_chars` and `std::optional<std::size_t>` to validate positive integer input without locale overhead or exceptions.
* `rdrand-hamming-distance.cpp` – Main benchmark and statistical evaluation loop running `10` batches and computing per-batch histograms, distance sums, and arithmetic means.
* `rdrand-hamming-distance.slnx` / `rdrand-hamming-distance.vcxproj` – Visual Studio solution and project files.

## Requirements
* **Architecture:** 64-bit x86 (`x64`) CPU supporting the `RDRAND` instruction (`_rdrand64_step`).
* **Compiler / Standard:** C++20 or newer (required for `<bit>` / `std::popcount`, `std::from_chars`, `std::optional`, and `std::string_view`)[cite: 5, 6, 9].
* **Build Environment:** Visual Studio (`x64` configurations in `rdrand-hamming-distance.slnx` / `rdrand-hamming-distance.vcxproj`) or a C++20-capable compiler supporting `<immintrin.h>` and `-mrdrnd` (GCC/Clang)[cite: 2, 11, 12].

## Usage
```bash
rdrand-hamming-distance [number_of_random_numbers]
