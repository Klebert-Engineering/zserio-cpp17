

#ifndef ZSERIO_ERROR_UTIL_H_INC
#define ZSERIO_ERROR_UTIL_H_INC

#include "zserio/Error.h"

namespace zserio
{

/**
 * Create a ConstraintError.
 */
inline expected<void> makeConstraintError(std::string fieldName, std::string constraint)
{
    return tl::unexpected(ConstraintError(std::move(fieldName), std::move(constraint)));
}

/**
 * Create an OutOfRangeError.
 */
inline expected<void> makeOutOfRangeError(std::string fieldName, std::string value, std::string range)
{
    return tl::unexpected(OutOfRangeError(std::move(fieldName), std::move(value), std::move(range)));
}

/**
 * Create a MissedOptionalError.
 */
inline expected<void> makeMissedOptionalError(std::string fieldName)
{
    return tl::unexpected(MissedOptionalError(std::move(fieldName)));
}

/**
 * Create an UnexpectedOptionalError.
 */
inline expected<void> makeUnexpectedOptionalError(std::string fieldName)
{
    return tl::unexpected(UnexpectedOptionalError(std::move(fieldName)));
}

/**
 * Create a ChoiceCaseError.
 */
inline expected<void> makeChoiceCaseError(std::string choiceName, std::string caseName)
{
    return tl::unexpected(ChoiceCaseError(std::move(choiceName), std::move(caseName)));
}

/**
 * Create a UnionCaseError.
 */
inline expected<void> makeUnionCaseError(std::string unionName, std::string caseName)
{
    return tl::unexpected(UnionCaseError(std::move(unionName), std::move(caseName)));
}

/**
 * Create a ServiceError.
 */
inline expected<void> makeServiceError(std::string message)
{
    return tl::unexpected(ServiceError(std::move(message)));
}

/**
 * Create a ValidationError.
 */
inline expected<void> makeValidationError(std::string message)
{
    return tl::unexpected(ValidationError(std::move(message)));
}

/**
 * Create a RuntimeError.
 */
inline expected<void> makeRuntimeError(std::string message)
{
    return tl::unexpected(RuntimeError(std::move(message)));
}

} // namespace zserio

#endif // ZSERIO_ERROR_UTIL_H_INC

