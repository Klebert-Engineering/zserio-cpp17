
#ifndef ZSERIO_EXPECTED_H_INC
#define ZSERIO_EXPECTED_H_INC

#include <expected/expected.hpp>

namespace zserio
{

/**
 * Alias for tl::expected to make it easier to use in the Zserio codebase.
 */
template<typename Value, typename Error = std::string>
using expected = tl::expected<Value, Error>;

} // namespace zserio

#endif // ZSERIO_EXPECTED_H_INC
