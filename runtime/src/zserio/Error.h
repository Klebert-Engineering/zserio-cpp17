
#ifndef ZSERIO_ERROR_H_INC
#define ZSERIO_ERROR_H_INC

#include <string>
#include <variant>

#include "zserio/Expected.h"

namespace zserio
{

/**
 * Base class for all Zserio errors.
 */
struct ZserioError
{
    virtual ~ZserioError() = default;
    virtual std::string toString() const = 0;
};

/**
 * Error representing a constraint violation.
 */
struct ConstraintError : public ZserioError
{
    std::string fieldName;
    std::string constraint;

    ConstraintError(const std::string& fieldName, const std::string& constraint)
        : fieldName(fieldName), constraint(constraint) {}

    std::string toString() const override
    {
        return "Constraint error: field '" + fieldName + "' violates constraint '" + constraint + "'";
    }
};

/**
 * Error representing an out of range value.
 */
struct OutOfRangeError : public ZserioError
{
    std::string fieldName;
    std::string value;
    std::string range;

    OutOfRangeError(const std::string& fieldName, const std::string& value, const std::string& range)
        : fieldName(fieldName), value(value), range(range) {}

    std::string toString() const override
    {
        return "Out of range error: field '" + fieldName + "' with value '" + value +
               "' is out of range '" + range + "'";
    }
};

/**
 * Error representing a missed optional field.
 */
struct MissedOptionalError : public ZserioError
{
    std::string fieldName;

    explicit MissedOptionalError(const std::string& fieldName)
        : fieldName(fieldName) {}

    std::string toString() const override
    {
        return "Missed optional error: field '" + fieldName + "' is missed";
    }
};

/**
 * Error representing an unexpected optional field.
 */
struct UnexpectedOptionalError : public ZserioError
{
    std::string fieldName;

    explicit UnexpectedOptionalError(const std::string& fieldName)
        : fieldName(fieldName) {}

    std::string toString() const override
    {
        return "Unexpected optional error: field '" + fieldName + "' is unexpected";
    }
};

/**
 * Error representing a choice case error.
 */
struct ChoiceCaseError : public ZserioError
{
    std::string choiceName;
    std::string caseName;

    ChoiceCaseError(const std::string& choiceName, const std::string& caseName)
        : choiceName(choiceName), caseName(caseName) {}

    std::string toString() const override
    {
        return "Choice case error: choice '" + choiceName + "' has invalid case '" + caseName + "'";
    }
};

/**
 * Error representing a union case error.
 */
struct UnionCaseError : public ZserioError
{
    std::string unionName;
    std::string caseName;

    UnionCaseError(const std::string& unionName, const std::string& caseName)
        : unionName(unionName), caseName(caseName) {}

    std::string toString() const override
    {
        return "Union case error: union '" + unionName + "' has invalid case '" + caseName + "'";
    }
};

/**
 * Error representing a service error.
 */
struct ServiceError : public ZserioError
{
    std::string message;

    explicit ServiceError(const std::string& message)
        : message(message) {}

    std::string toString() const override
    {
        return "Service error: " + message;
    }
};

/**
 * Error representing a validation error.
 */
struct ValidationError : public ZserioError
{
    std::string message;

    explicit ValidationError(const std::string& message)
        : message(message) {}

    std::string toString() const override
    {
        return "Validation error: " + message;
    }
};

/**
 * Error representing a general runtime error.
 */
struct RuntimeError : public ZserioError
{
    std::string message;

    explicit RuntimeError(const std::string& message)
        : message(message) {}

    std::string toString() const override
    {
        return "Runtime error: " + message;
    }
};

/**
 * Alias for the variant type that can hold any Zserio error.
 */
using ZserioErrorVariant = std::variant<
    ConstraintError,
    OutOfRangeError,
    MissedOptionalError,
    UnexpectedOptionalError,
    ChoiceCaseError,
    UnionCaseError,
    ServiceError,
    ValidationError,
    RuntimeError
>;

/**
 * Convert a ZserioErrorVariant to a string representation.
 */
inline std::string toString(const ZserioErrorVariant& error)
{
    return std::visit([](const auto& e) -> std::string { return e.toString(); }, error);
}

} // namespace zserio

#endif // ZSERIO_ERROR_H_INC
