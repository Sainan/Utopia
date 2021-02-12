#include "SourceLocation.hpp"

namespace Utopia
{
    SourceLocation::SourceLocation(std::string&& name)
        : name(std::move(name))
    {
    }

    SourceLocation::operator std::string() const
    {
        return std::string(name).append(" on line ").append(std::to_string(line)).append(", colon ").append(std::to_string(colon));
    }
}
