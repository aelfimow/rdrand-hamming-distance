#include "parser.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <optional>
#include <system_error>

std::optional<std::size_t> parser::positive_count(std::string_view const& arg) const noexcept
{
    std::size_t value = 0U;
    auto const [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), value);

    bool const parse_ok = (ec == std::errc{});
    bool const full_string_consumed = (ptr == (arg.data() + arg.size()));
    bool const is_positive = (value > 0U);

    std::array<bool, 3U> const flags
    {
        not parse_ok,
        not full_string_consumed,
        not is_positive
    };

    if (std::any_of(flags.cbegin(), flags.cend(), [](bool flag) { return flag; }))
    {
        return std::nullopt;
    }

    return value;
}