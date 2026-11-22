#include "rdrand.hpp"
#include <immintrin.h>
#include <stdexcept>

rdrand::rdrand(size_t max_retries):
	m_max_retries { max_retries },
	m_retries { 0U }
{
	if (m_max_retries == 0U)
	{
		throw std::invalid_argument("Max. retries is invalid");
	}
}

rdrand::~rdrand()
{
}

[[nodiscard]] uint64_t rdrand::operator() () const
{
	uint64_t rand_value = 0U;
	bool valid = false;

	for (size_t i = 0U; i < m_max_retries; ++i)
	{
		valid = (0 != _rdrand64_step(&rand_value));
		if (valid)
		{
			break;
		}

		++m_retries;
	}

	if (not valid)
	{
		throw std::runtime_error("Max. retries reached, rdrand64_step fails");
	}

	return rand_value;
}

[[nodiscard]] size_t rdrand::retries() const
{
	return m_retries;
}