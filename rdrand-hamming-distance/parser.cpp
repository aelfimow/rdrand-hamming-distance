#include "parser.hpp"

#include <charconv>
#include <optional>
#include <system_error>

std::optional<std::size_t> parser::positive_count(std::string_view const& arg) const noexcept
{
    std::size_t value = 0U;
    auto const [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), value);

    if (ec != std::errc{} || ptr != (arg.data() + arg.size()) || value == 0U)
    {
        return std::nullopt;
    }

    return value;
}