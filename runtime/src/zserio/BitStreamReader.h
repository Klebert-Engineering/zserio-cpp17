#ifndef ZSERIO_BIT_STREAM_READER_H_INC
#define ZSERIO_BIT_STREAM_READER_H_INC

#include <algorithm>
#include <cstddef>
#include <type_traits>

#include "zserio/BitBuffer.h"
#include "zserio/Bytes.h"
#include "zserio/RebindAlloc.h"
#include "zserio/String.h"
#include "zserio/Types.h"
#include "zserio/Expected.h"

namespace zserio
{

/**
 * Reader class which allows to read various data from the bit stream.
 */
class BitStreamReader
{
public:
    /** Type for bit position. */
    using BitPosType = size_t;

    /**
     * Context of the reader defining its state.
     */
    struct ReaderContext
    {
        /**
         * Constructor.
         *
         * \param readBuffer Span to the buffer to read.
         * \param readBufferBitSize Size of the buffer in bits.
         */
        explicit ReaderContext(Span<const uint8_t> readBuffer, size_t readBufferBitSize);

        /**
         * Destructor.
         */
        ~ReaderContext() = default;

        /**
         * Copying and moving is disallowed!
         * \{
         */
        ReaderContext(const ReaderContext&) = delete;
        ReaderContext& operator=(const ReaderContext&) = delete;

        ReaderContext(const ReaderContext&&) = delete;
        ReaderContext& operator=(const ReaderContext&&) = delete;
        /**
         * \}
         */

        Span<const uint8_t> buffer; /**< Buffer to read from. */
        const BitPosType bufferBitSize; /**< Size of the buffer in bits. */

        uintptr_t cache; /**< Bit cache to optimize bit reading. */
        uint8_t cacheNumBits; /**< Num bits available in the bit cache. */

        BitPosType bitIndex; /**< Current bit index. */
    };

    /**
     * Constructor from raw buffer.
     *
     * \param buffer Pointer to the buffer to read.
     * \param bufferByteSize Size of the buffer in bytes.
     */
    explicit BitStreamReader(const uint8_t* buffer, size_t bufferByteSize);

    /**
     * Constructor from buffer passed as a Span.
     *
     * \param buffer Buffer to read.
     */
    explicit BitStreamReader(Span<const uint8_t> buffer);

    /**
     * Constructor from buffer passed as a Span with exact bit size.
     *
     * \param buffer Buffer to read.
     * \param bufferBitSize Size of the buffer in bits.
     */
    explicit BitStreamReader(Span<const uint8_t> buffer, size_t bufferBitSize);

    /**
     * Constructor from raw buffer with exact bit size.
     *
     * \param buffer Pointer to buffer to read.
     * \param bufferBitSize Size of the buffer in bits.
     */
    explicit BitStreamReader(const uint8_t* buffer, size_t bufferBitSize, BitsTag);

    /**
     * Constructor from bit buffer.
     *
     * \param bitBuffer Bit buffer to read from.
     */
    template <typename ALLOC>
    explicit BitStreamReader(const BasicBitBuffer<ALLOC>& bitBuffer) :
            BitStreamReader(bitBuffer.getData(), bitBuffer.getBitSize())
    {}

    /**
     * Destructor.
     */
    ~BitStreamReader() = default;

    /**
     * Reads unsigned bits up to 32-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return expected<uint32_t> containing error if reading fails.
     */
    expected<uint32_t> readUnsignedBits32(uint8_t numBits = 32);

    /**
     * Reads unsigned bits up to 64-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return expected<uint64_t> containing error if reading fails.
     */
    expected<uint64_t> readUnsignedBits64(uint8_t numBits = 64);

    /**
     * Reads signed bits up to 32-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return expected<int32_t> containing error if reading fails.
     */
    expected<int32_t> readSignedBits32(uint8_t numBits = 32);

    /**
     * Reads signed bits up to 64-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return expected<int64_t> containing error if reading fails.
     */
    expected<int64_t> readSignedBits64(uint8_t numBits = 64);

    /**
     * Reads bool as a single bit.
     *
     * \return expected<Bool> containing error if reading fails.
     */
    expected<Bool> readBool();

    /**
     * Reads signed variable integer up to 16 bits.
     *
     * \return expected<VarInt16> containing error if reading fails.
     */
    expected<VarInt16> readVarInt16();

    /**
     * Reads signed variable integer up to 32 bits.
     *
     * \return expected<VarInt32> containing error if reading fails.
     */
    expected<VarInt32> readVarInt32();

    /**
     * Reads signed variable integer up to 32 bits.
     *
     * \return expected<VarInt64> containing error if reading fails.
     */
    expected<VarInt64> readVarInt64();

    /**
     * Reads signed variable integer up to 72 bits.
     *
     * \return expected<VarInt> containing error if reading fails.
     */
    expected<VarInt> readVarInt();

    /**
     * Read unsigned variable integer up to 16 bits.
     *
     * \return expected<VarUInt16> containing error if reading fails.
     */
    expected<VarUInt16> readVarUInt16();

    /**
     * Read unsigned variable integer up to 32 bits.
     *
     * \return expected<VarUInt32> containing error if reading fails.
     */
    expected<VarUInt32> readVarUInt32();

    /**
     * Read unsigned variable integer up to 64 bits.
     *
     * \return expected<VarUInt64> containing error if reading fails.
     */
    expected<VarUInt64> readVarUInt64();

    /**
     * Read unsigned variable integer up to 72 bits.
     *
     * \return expected<VarUInt> containing error if reading fails.
     */
    expected<VarUInt> readVarUInt();

    /**
     * Read variable size integer up to 40 bits.
     *
     * \return expected<VarSize> containing error if reading fails.
     */
    expected<VarSize> readVarSize();

    /**
     * Reads 16-bit float.
     *
     * \return expected<Float16> containing error if reading fails.
     */
    expected<Float16> readFloat16();

    /**
     * Reads 32-bit float.
     *
     * \return expected<Float32> containing error if reading fails.
     */
    expected<Float32> readFloat32();

    /**
     * Reads 64-bit float double.
     *
     * \return expected<Float64> containing error if reading fails.
     */
    expected<Float64> readFloat64();

    /**
     * Reads bytes.
     *
     * \param alloc Allocator to use.
     *
     * \return expected<BasicBytes<ALLOC>> containing error if reading fails.
     */
    template <typename ALLOC = std::allocator<uint8_t>>
    expected<BasicBytes<ALLOC>> readBytes(const ALLOC& alloc = ALLOC())
    {
        auto lenResult = readVarSize();
        if (!lenResult)
        {
            return tl::unexpected(lenResult.error());
        }

        const size_t len = static_cast<size_t>(*lenResult);
        const BitPosType beginBitPosition = getBitPosition();
        if ((beginBitPosition & 0x07U) != 0)
        {
            // we are not aligned to byte
            Vector<uint8_t, ALLOC> value{alloc};
            value.reserve(len);
            for (size_t i = 0; i < len; ++i)
            {
                auto byteResult = readByte();
                if (!byteResult)
                {
                    return tl::unexpected(byteResult.error());
                }
                value.push_back(*byteResult);
            }
            return value;
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + len * 8);
            Span<const uint8_t>::iterator beginIt = m_context.buffer.begin() + beginBitPosition / 8;
            return BasicBytes<ALLOC>(beginIt, beginIt + len, alloc);
        }
    }

    /**
     * Reads an UTF-8 string.
     *
     * \param alloc Allocator to use.
     *
     * \return expected<BasicString<ALLOC>> containing error if reading fails.
     */
    template <typename ALLOC = std::allocator<char>>
    expected<BasicString<ALLOC>> readString(const ALLOC& alloc = ALLOC())
    {
        auto lenResult = readVarSize();
        if (!lenResult)
        {
            return tl::unexpected(lenResult.error());
        }

        const size_t len = static_cast<size_t>(*lenResult);
        const BitPosType beginBitPosition = getBitPosition();
        if ((beginBitPosition & 0x07U) != 0)
        {
            // we are not aligned to byte
            BasicString<ALLOC> value{alloc};
            value.reserve(len);
            for (size_t i = 0; i < len; ++i)
            {
                auto byteResult = readByte();
                if (!byteResult)
                {
                    return tl::unexpected(byteResult.error());
                }
                const char readCharacter = std::char_traits<char>::to_char_type(
                        static_cast<std::char_traits<char>::int_type>(*byteResult));
                value.push_back(readCharacter);
            }
            return value;
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + len * 8);
            Span<const uint8_t>::iterator beginIt = m_context.buffer.begin() + beginBitPosition / 8;
            return BasicString<ALLOC>(beginIt, beginIt + len, alloc);
        }
    }

    /**
     * Reads a bit buffer.
     *
     * \param alloc Allocator to use.
     *
     * \return expected<BasicBitBuffer<RebindAlloc<ALLOC, uint8_t>>> containing error if reading fails.
     */
    template <typename ALLOC = std::allocator<uint8_t>>
    expected<BasicBitBuffer<RebindAlloc<ALLOC, uint8_t>>> readBitBuffer(const ALLOC& allocator = ALLOC())
    {

        auto bitSizeResult = readVarSize();
        if (!bitSizeResult)
        {
            return tl::unexpected(bitSizeResult.error());
        }
        const size_t bitSize = static_cast<size_t>(*bitSizeResult);

        const size_t numBytesToRead = bitSize / 8;
        const uint8_t numRestBits = static_cast<uint8_t>(bitSize - numBytesToRead * 8);
        BasicBitBuffer<RebindAlloc<ALLOC, uint8_t>> bitBuffer(bitSize, allocator);
        Span<uint8_t> buffer = bitBuffer.getData();
        const BitPosType beginBitPosition = getBitPosition();
        const Span<uint8_t>::iterator itEnd = buffer.begin() + numBytesToRead;
        if ((beginBitPosition & 0x07U) != 0)
        {
            // we are not aligned to byte
            for (Span<uint8_t>::iterator it = buffer.begin(); it != itEnd; ++it)
            {
                auto byteResult = readUnsignedBits32(8);
                if (!byteResult)
                {
                    return tl::unexpected(byteResult.error());
                }
                *it = static_cast<uint8_t>(*byteResult);
            }
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + numBytesToRead * 8);
            Span<const uint8_t>::const_iterator sourceIt = m_context.buffer.begin() + beginBitPosition / 8;
            (void)std::copy(sourceIt, sourceIt + numBytesToRead, buffer.begin());
        }

        if (numRestBits > 0)
        {
            auto restBitsResult = readUnsignedBits32(numRestBits);
            if (!restBitsResult)
            {
                return tl::unexpected(restBitsResult.error());
            }
            *itEnd = static_cast<uint8_t>(*restBitsResult << (8U - numRestBits));
        }

        return bitBuffer;
    }

    /**
     * Gets current bit position.
     *
     * \return Current bit position.
     */
    BitPosType getBitPosition() const
    {
        return m_context.bitIndex;
    }

    /**
     * Sets current bit position. Use with caution!
     *
     * \param position New bit position.
     */
    void setBitPosition(BitPosType position);

    /**
     * Moves current bit position to perform the requested bit alignment.
     *
     * \param alignment Size of the alignment in bits.
     */
    void alignTo(size_t alignment);

    /**
     * Gets size of the underlying buffer in bits.
     *
     * \return Buffer bit size.
     */
    size_t getBufferBitSize() const
    {
        return m_context.bufferBitSize;
    }

private:
    expected<uint8_t> readByte();

    ReaderContext m_context;
};

namespace detail
{

inline void read(BitStreamReader& reader, Bool& value)
{
    value = reader.readBool();
}

template <BitSize BIT_SIZE, bool IS_SIGNED>
void read(BitStreamReader& reader, FixedIntWrapper<BIT_SIZE, IS_SIGNED>& value)
{
    using ValueType = typename FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType;
    // TODO[Mi-L@]: implement on the reader to get rid of the numBits check
    if constexpr (sizeof(ValueType) <= 4)
    {
        if constexpr (std::is_signed_v<ValueType>)
        {
            value = static_cast<ValueType>(reader.readSignedBits32(BIT_SIZE));
        }
        else
        {
            value = static_cast<ValueType>(reader.readUnsignedBits32(BIT_SIZE));
        }
    }
    else
    {
        if constexpr (std::is_signed_v<ValueType>)
        {
            value = static_cast<ValueType>(reader.readSignedBits64(BIT_SIZE));
        }
        else
        {
            value = static_cast<ValueType>(reader.readUnsignedBits64(BIT_SIZE));
        }
    }
}

template <typename T>
void read(BitStreamReader& reader, DynIntWrapper<T>& value, uint8_t numBits)
{
    if constexpr (sizeof(T) <= 4)
    {
        if constexpr (std::is_signed_v<T>)
        {
            value = static_cast<T>(reader.readSignedBits32(numBits));
        }
        else
        {
            value = static_cast<T>(reader.readUnsignedBits32(numBits));
        }
    }
    else
    {
        if constexpr (std::is_signed_v<T>)
        {
            value = static_cast<T>(reader.readSignedBits64(numBits));
        }
        else
        {
            value = static_cast<T>(reader.readUnsignedBits64(numBits));
        }
    }
}

inline void read(BitStreamReader& reader, VarInt16& value)
{
    value = reader.readVarInt16();
}

inline void read(BitStreamReader& reader, VarInt32& value)
{
    value = reader.readVarInt32();
}

inline void read(BitStreamReader& reader, VarInt64& value)
{
    value = reader.readVarInt64();
}

inline void read(BitStreamReader& reader, VarInt& value)
{
    value = reader.readVarInt();
}

inline void read(BitStreamReader& reader, VarUInt16& value)
{
    value = reader.readVarUInt16();
}

inline void read(BitStreamReader& reader, VarUInt32& value)
{
    value = reader.readVarUInt32();
}

inline void read(BitStreamReader& reader, VarUInt64& value)
{
    value = reader.readVarUInt64();
}

inline void read(BitStreamReader& reader, VarUInt& value)
{
    value = reader.readVarUInt();
}

inline void read(BitStreamReader& reader, VarSize& value)
{
    value = reader.readVarSize();
}

inline void read(BitStreamReader& reader, Float16& value)
{
    value = reader.readFloat16();
}

inline void read(BitStreamReader& reader, Float32& value)
{
    value = reader.readFloat32();
}

inline void read(BitStreamReader& reader, Float64& value)
{
    value = reader.readFloat64();
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, BasicBytes<ALLOC>& value)
{
    value = reader.readBytes(value.get_allocator());
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, zserio::BasicString<ALLOC>& value)
{
    value = reader.readString(value.get_allocator());
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, BasicBitBuffer<ALLOC>& value)
{
    value = reader.readBitBuffer(value.get_allocator());
}

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_BIT_STREAM_READER_H_INC
