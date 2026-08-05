#ifdef RDRAND_HPP
#error Already included.
#else
#define RDRAND_HPP

#include <cstdint>

class rdrand
{
public:
	rdrand();
	~rdrand();

	uint64_t operator() () const;

private:
	size_t const m_max_retries;
};

#endif