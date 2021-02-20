#include "is_whitespace.hpp"

namespace Utopia
{
    bool is_whitespace(const char c)
    {
        switch (c)
        {
        case ' ':
        case '\t':
            return true;
        }
        return false;
    }
}
