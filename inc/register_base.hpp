#ifndef _REGISTER_BASE_HPP_
#define _REGISTER_BASE_HPP_

#include "./status_codes.hpp"

#include <cstdint>
#include <stdint.h>
#include <assert.h>
#include <type_traits>

namespace reg
{
    /**
     * @brief Enum representing bitfield access permissions.
     */
    enum class BitFieldAccessFlag : uint8_t 
    {
        RO, ///< Read-only
        WO, ///< Write-only
        RW,  ///< Read/Write
        RC_W0 // Read and Write 0 only
    };
}

/**
 * @brief Represents a strongly-typed bitfield mask for a specific hardware register.
 *
 * This mask encodes width, position, and access control metadata at compile time, enabling
 * compile-time safety for read/write operations, and allowing composition of multiple bitfields.
 *
 * @tparam Tag         A type uniquely identifying the hardware register (must define Tag::id).
 * @tparam AccessFlag  Indicates the access permissions of the field (RO, WO, or RW).
 * @tparam Width       Width of the bitfield in bits.
 * @tparam Position    Bit position (LSB index) of the field within the register.
 * @tparam IsComposite Set to true if the mask represents a combination of multiple bitfields.
 */
template<typename Tag, reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position = 0, typename ValueType = uint32_t, bool IsComposite = false>
struct RegisterMask
{
    /// @brief Final bitmask value (already shifted into position).
    uint32_t value{};

    /// @brief Bitfield access type (RO, WO, or RW).
    static constexpr reg::BitFieldAccessFlag access_flag{AccessFlag};

    /// @brief Whether this is a composite mask created by combining other masks.
    static constexpr bool is_composite = IsComposite;

    /**
     * @brief Constructs a register mask from an unshifted field value.
     *
     * The value is masked and shifted into position based on template parameters.
     *
     * @param val The unshifted field value to insert into the bitfield.
     */
    constexpr RegisterMask(ValueType val)
        : value{(static_cast<uint32_t>(val) & ((Width < 32) ? ((1U << Width) - 1) : ~0U)) << Position} {}

    /**
     * @brief Constructs a register mask with default state
     *
     * The value is masked and shifted into position based on template parameters.
     *
     * @param val The unshifted field value to insert into the bitfield.
     */
     constexpr RegisterMask()
     : value{(((Width < 32) ? ((1U << Width) - 1) : ~0UL)) << Position} {}

    /*template<typename T, std::enable_if_t<std::is_enum_v<T> || std::is_integral_v<T>, int> = 0>
    constexpr RegisterMask(T val)
        : RegisterMask(static_cast<uint32_t>(val)) {}*/

    /**
     * @brief Bitwise OR between two RegisterMasks with matching Tag and AccessFlag.
     *
     * This creates a composite mask (`IsComposite = true`) with `Width = 32` and `Position = 0`.
     * Combining is allowed only if both masks reference the same register and access type.
     *
     * @param other The other mask to combine with.
     * @return New composite RegisterMask.
     */
    template<typename OtherTag, reg::BitFieldAccessFlag OtherAccessFlag, uint32_t OtherWidth, uint32_t OtherPosition, typename OtherValueType, bool OtherIsComposite>
    constexpr RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true> operator|(const RegisterMask<OtherTag, OtherAccessFlag, OtherWidth, OtherPosition, OtherValueType, OtherIsComposite>& other) const
    {
        static_assert(std::is_same_v<Tag, OtherTag>, "Mismatched tags");
        static_assert(AccessFlag == OtherAccessFlag, "AccessFlag mismatch in bitmask combination");
        return RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true>{value | other.value};
    }

    /**
     * @brief Bitwise AND between two RegisterMasks with matching Tag and AccessFlag.
     *
     * Creates a composite mask (`IsComposite = true`) with `Width = 32` and `Position = 0`.
     *
     * @param other The other mask to combine with.
     * @return New composite RegisterMask.
     */
    template<typename OtherTag, reg::BitFieldAccessFlag OtherAccessFlag, uint32_t OtherWidth, uint32_t OtherPosition,  typename OtherValueType, bool OtherIsComposite>
    constexpr RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true> operator&(const RegisterMask<OtherTag, OtherAccessFlag, OtherWidth, OtherPosition, OtherValueType, OtherIsComposite>& other) const
    {
        static_assert(std::is_same_v<Tag, OtherTag>, "Mismatched tags");
        static_assert(AccessFlag == OtherAccessFlag, "AccessFlag mismatch in bitmask combination");
        return RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true>{value & other.value};
    }

    /**
     * @brief Implicit conversion to uint32_t for direct register operations.
     *
     * @return The masked and shifted bitfield value.
     */
    constexpr operator uint32_t() const 
    {
        return value;
    }
};

/**
 * @brief Represents a hardware register and provides safe accessors for field-level manipulation.
 *
 * All operations are type-safe and access-controlled using the `RegisterMask` system.
 *
 * @tparam Tag     A type uniquely identifying the register (used to match masks).
 * @tparam Address Physical address of the hardware register.
 */
template<typename Tag, uint32_t Addr>
class Register
{
    public:
        Register() = delete;
    
        /**
        * @brief Sets bits in the register (bitwise OR with mask).
        *
        * @param mask The bitfield mask to set.
        */
        template<reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position, typename ValueType, bool IsComposite>
        static inline StatusCode set(RegisterMask<Tag, AccessFlag, Width, Position, ValueType, IsComposite> mask) 
        {
            static_assert(AccessFlag != reg::BitFieldAccessFlag::RO, "Trying to set a read-only field");
            *reinterpret_cast<volatile uint32_t*>(Addr) |= mask.value;

            return StatusCode::Ok;
        }
    
        /**
        * @brief Clears bits in the register (bitwise AND with inverted mask).
        *
        * @param mask The bitfield mask to clear.
        */
        template<reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position, typename ValueType, bool IsComposite>
        static inline StatusCode clear(RegisterMask<Tag, AccessFlag, Width, Position, ValueType, IsComposite> mask) 
        {
            static_assert(AccessFlag != reg::BitFieldAccessFlag::RO, "Trying to clear a read-only field");
            *reinterpret_cast<volatile uint32_t*>(Addr) &= ~mask.value;

            return StatusCode::Ok;
        }
    
        /**
        * @brief Overwrites the register with the given mask value.
        *
        * @param mask The full value to write into the register.
        */
        template<reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position, typename ValueType, bool IsComposite>
        static inline StatusCode write(RegisterMask<Tag, AccessFlag, Width, Position, ValueType, IsComposite> mask) 
        {
            static_assert(AccessFlag != reg::BitFieldAccessFlag::RO, "Trying to write a read-only field");
            *reinterpret_cast<volatile uint32_t*>(Addr) = mask.value;

            return StatusCode::Ok;
        }
    
        /**
        * @brief Reads the value of the specified field from the register.
        *
        * For composite masks, returns the raw value masked (no shift).
        * For non-composite fields, the value is shifted right into LSB position.
        *
        * @param mask The bitfield mask specifying the bits to read.
        * @return Extracted field value (shifted if single field, unshifted if composite).
        */
        template<reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position, typename ValueType, bool IsComposite>
        static inline uint32_t read(RegisterMask<Tag, AccessFlag, Width, Position, ValueType, IsComposite> mask) 
        {
            static_assert(AccessFlag != reg::BitFieldAccessFlag::WO, "Trying to read a write-only field");
            uint32_t raw = *reinterpret_cast<volatile uint32_t*>(Addr) & mask.value;
            if constexpr (IsComposite)
                return raw;  // No shift for composite masks
            else
                return raw >> Position; // Shift for single-field masks
        }

        static inline uint32_t get_addr() 
        {
            return (Addr);
        }
};
 

#endif // _REGISTER_BASE_HPP_
