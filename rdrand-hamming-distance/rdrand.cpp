#include "rdrand.hpp"
#include <immintrin.h>
#include <stdexcept>

rdrand::rdrand() :
	m_max_retries{ 10U }
{
}

rdrand::~rdrand()
{
}

uint64_t rdrand::operator() () const
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
	}

	if (not valid)
	{
		throw std::runtime_error("Max. retries reached, rdrand64_step fails");
	}

	return rand_value;
}
