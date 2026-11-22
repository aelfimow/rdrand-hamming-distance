#ifdef RDRAND_HPP
#error Already included.
#else
#define RDRAND_HPP

#include <cstdint>

class rdrand
{
public:
	explicit rdrand(size_t max_retries = 10U);
	~rdrand();

	[[nodiscard]] uint64_t operator() () const;

	[[nodiscard]] size_t retries() const;

private:
	size_t const m_max_retries;
	mutable size_t m_retries;
};

#endif