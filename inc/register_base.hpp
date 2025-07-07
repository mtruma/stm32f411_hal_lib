#ifndef _REGISTER_BASE_HPP_
#define _REGISTER_BASE_HPP_

#include "./status_codes.hpp"

#include <cstdint>
#include <stdint.h>
#include <assert.h>
#include <type_traits>

/**
 * @brief Base register specific types.
 */
namespace reg
{
    /**
     * @brief Enum representing bitfield access permissions.
     */
    enum class BitFieldAccessFlag : uint8_t 
    {
        RO,   ///< Read-only
        WO,   ///< Write-only
        RW,   ///< Read/Write
        RC_W0 ///< Read and Write 0 only
    };
}

/**
 * @brief Base RegisterMask that is used to construct other masks.
 *
 *
 * @tparam Tag         A type uniquely identifying the hardware register.
 * @tparam AccessFlag  Access permissions of the field (RO, WO, or RW).
 * @tparam Width       Width of the bitfield in bits.
 * @tparam Position    Bit position (LSB index) of the field within the register.
 * @tparam ValueType   Value type, since enum classes are used as a value but can also be raw uint type.
 * @tparam IsComposite Set to true if the mask represents a combination of multiple bitfields.
 */
template<typename Tag, reg::BitFieldAccessFlag AccessFlag, uint32_t Width, uint32_t Position = 0, typename ValueType = uint32_t, bool IsComposite = false>
struct RegisterMask
{
    /// @brief Final bitmask value.
    uint32_t value{};

    /// @brief Bitfield access type (RO, WO, RW...).
    static constexpr reg::BitFieldAccessFlag access_flag{AccessFlag};

    /// @brief Whether this is a composite mask created by combining other masks.
    static constexpr bool is_composite = IsComposite;

    /**
     * @brief Constructs a register mask from the value.
     *
     * The value is masked and shifted into position based on template parameters.
     *
     * @param val Value from which to construct mask.
     */
    constexpr RegisterMask(ValueType val)
        : value{(static_cast<uint32_t>(val) & ((Width < 32) ? ((1U << Width) - 1) : ~0U)) << Position} {}

    /**
     * @brief Constructs a register mask with default state
     *
     * The value is masked and shifted into position based on template parameters.
     *
     */
     constexpr RegisterMask()
     : value{(((Width < 32) ? ((1U << Width) - 1) : ~0UL)) << Position} {}

    /**
     * @brief Combine two RegisterMask objects using bitwise OR.
     *
     * This function merges two RegisterMask instances into a single composite mask.
     * The result is a new mask with:
     *   - Composite flag set to true (`IsComposite = true`)
     *   - Full register width (`Width = 32`)
     *   - Position set to 0
     *   - Value type set to `uint32_t`
     *
     * Constraints:
     *   - Both masks must reference the same register tag (`Tag`) and have the same access flag (`AccessFlag`)
     *   - Intended for forming composite masks to be applied to full registers.
     *
     * @tparam OtherTag            Tag type of the other mask
     * @tparam OtherAccessFlag     Access flag of the other mask
     * @tparam OtherWidth          Width of the other mask
     * @tparam OtherPosition       Bit position of the other mask
     * @tparam OtherValueType      Value type of the other mask
     * @tparam OtherIsComposite    Composite status of the other mask
     * 
     * @param other Another RegisterMask to combine with.
     * @return A new composite RegisterMask combining both masks via bitwise OR.
     */
    template<typename OtherTag, reg::BitFieldAccessFlag OtherAccessFlag, uint32_t OtherWidth, uint32_t OtherPosition, typename OtherValueType, bool OtherIsComposite>
    constexpr RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true> operator|(const RegisterMask<OtherTag, OtherAccessFlag, OtherWidth, OtherPosition, OtherValueType, OtherIsComposite>& other) const
    {
        static_assert(std::is_same_v<Tag, OtherTag>, "Mismatched tags");
        static_assert(AccessFlag == OtherAccessFlag, "AccessFlag mismatch in bitmask combination");
        return RegisterMask<Tag, AccessFlag, 32, 0, uint32_t, true>{value | other.value};
    }

    /**
     * @brief Combine two RegisterMask objects using bitwise AND.
     *
     * This function performs a bitwise AND between two RegisterMask instances and
     * returns a new composite mask with the following properties:
     *   - Composite flag set to true (`IsComposite = true`)
     *   - Full register width (`Width = 32`)
     *   - Bit position set to 0
     *   - Value type set to `uint32_t`
     *
     * Constraints:
     *   - Both masks must reference the same register tag (`Tag`) and have the same access flag (`AccessFlag`)
     *   - Intended for forming composite masks to be applied to full registers.
     *
     * @tparam OtherTag            Tag type of the other mask
     * @tparam OtherAccessFlag     Access flag of the other mask
     * @tparam OtherWidth          Width of the other mask
     * @tparam OtherPosition       Bit position of the other mask
     * @tparam OtherValueType      Value type of the other mask
     * @tparam OtherIsComposite    Composite status of the other mask
     *
     * @param other Another RegisterMask to AND with
     * @return A new composite RegisterMask representing the intersection
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
     * @return The masked value
     */
    constexpr operator uint32_t() const 
    {
        return value;
    }
};

/**
 * @brief Base Register class for building concrete registers.
 *
 * All operations are access-controlled using the `RegisterMask`.
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
         * @param mask The `RegisterMask` to set.
         * @return `StatusCode`.
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
         * @param mask The `RegisterMask` to clear.
         * @return `StatusCode`.
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
         * @param mask The `RegisterMask` to write and overwrite the register.
         * @return `StatusCode`.
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
         * For composite masks, returns the raw value masked.
         * For non-composite fields it returns actual value.
         *
         * @param mask The `RegisterMask` that represent which bit fields to read.
         * @return Currently returns only uint32_t value.
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

        /**
         * @brief Returns the address of the associated hardware register.
         *
         * Useful for debugging, unit tests, or interacting with the peripheral at a lower level.
         *
         * @return The absolute register address as a `uint32_t`.
         */
        static inline uint32_t get_addr() 
        {
            return (Addr);
        }
};
 

#endif // _REGISTER_BASE_HPP_
