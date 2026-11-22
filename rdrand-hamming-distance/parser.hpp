#ifdef PARSER_HPP
#error Already included.
#else
#define PARSER_HPP

#include <cstddef>
#include <optional>
#include <string_view>

class parser
{
public:
    parser() = default;
    ~parser() = default;

    [[nodiscard]] std::optional<std::size_t> positive_count(std::string_view const& arg) const noexcept;
};

#endif