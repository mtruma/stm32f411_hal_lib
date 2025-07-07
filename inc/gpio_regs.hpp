#ifndef _GPIOREGS_HPP_
#define _GPIOREGS_HPP_

#include "register_base.hpp"

#include <cstdint>
#include <stdint.h>
#include <assert.h>
#include <type_traits>

/**
 * @brief GPIO-related types and masks for easier register manipulation
 */
namespace gpio
{
    /// @brief Tag type representing the MODER (GPIO mode configuration).
    struct MODER_Tag {};

    /// @brief Tag type representing the OTYPER (output type).
    struct OTYPER_Tag {};

    /// @brief Tag type representing the OSPEEDR (output speed).
    struct OSPEEDR_Tag {};

    /// @brief Tag type representing the PUPDR (pull-up/pull-down).
    struct PUPDR_Tag {};

    /// @brief Tag type representing the IDR (input data).
    struct IDR_Tag {};

    /// @brief Tag type representing the ODR (output data).
    struct ODR_Tag {};

    /// @brief Tag type representing the BSRR (bit set/reset).
    struct BSRR_Tag {};

    /// @brief Tag type representing the LCKR (lock configuration).
    struct LCKR_Tag {};

    /// @brief Tag type representing the AFRL (alternate function low).
    struct AFRL_Tag {};

    /// @brief Tag type representing the AFRH (alternate function high).
    struct AFRH_Tag {};

    /**
     * @brief Enum representing available GPIO ports and their base addresses.
     */
    enum class Port : uint32_t
    {
        A = 0x40020000UL, ///< GPIO port A address
        B = 0x40024000UL, ///< GPIO port B address
        C = 0x40028000UL, ///< GPIO port C address
        D = 0x4002C000UL, ///< GPIO port D address
        E = 0x40030000UL, ///< GPIO port E address
        H = 0x4003C000UL  ///< GPIO port H address
    };

    /// @brief Enum representing GPIO pins (0–15).
    enum class Pins : uint8_t
    {
        P0 = 0,
        P1 = 1,
        P2 = 2,
        P3 = 3,
        P4 = 4,
        P5 = 5,
        P6 = 6,
        P7 = 7,
        P8 = 8,
        P9 = 9,
        P10 = 10,
        P11 = 11,
        P12 = 12,
        P13 = 13,
        P14 = 14,
        P15 = 15,
    };

    /**
     * @brief GPIO pin modes.
     */
    enum class Mode : uint8_t
    {
        Input    = 0U,
        Output   = 1U,
        AltFunc  = 2U,
        Analog   = 3U
    };

    /**
     * @brief GPIO output types.
     */
    enum class OutputType : uint8_t
    {
        PushPull  = 0U,
        OpenDrain = 1U
    };

    /**
     * @brief GPIO output speeds.
     */
    enum class OutputSpeed : uint8_t
    {
        Low     = 0U,
        Medium  = 1U,
        Fast    = 2U,
        High    = 3U
    };

    /**
     * @brief GPIO pull-up/pull-down configurations.
     */
    enum class PullType : uint8_t
    {
        NoPull   = 0U,
        PullUp   = 1U,
        PullDown = 2U
    };

    /**
     * @brief Alternate functions.
     */
    enum class AlternateFunc : uint16_t
    {
        AF0  = 0U,
        AF1,
        AF2,
        AF3,
        AF4,
        AF5,
        AF6,
        AF7,
        AF8,
        AF9,
        AF10,
        AF11,
        AF12,
        AF13,
        AF14,
        AF15
    };

    /**
     * @brief Bitfield mask wrapper for a specific pin and configuration value.
     * 
     * @tparam Tag        The register tag.
     * @tparam AccessFlag Access permission (RW, RO, etc.).
     * @tparam ValueType  Type of the configuration value (e.g., Mode, OutputType).
     * @tparam Pin        Pin
     * @tparam PosOffset  Position offset, in some cases you want starting position to be at 16 in reg(ex BSRR)
     */
    template<typename Tag, reg::BitFieldAccessFlag AccessFlag, uint8_t Width, typename ValueType, Pins Pin, uint8_t PosOffset = 0>
    struct PinMask : RegisterMask<Tag, AccessFlag, Width, static_cast<uint8_t>(Pin) * Width + PosOffset> 
    {
        /**
         * @brief Constructs a mask for a specific pin with a value.
         * 
         * @param val value to apply for that pin.
         */
        constexpr PinMask(ValueType val)
            : RegisterMask<Tag, AccessFlag, Width, static_cast<uint8_t>(Pin) * Width> {static_cast<uint32_t>(val)} 
            {
                if constexpr (std::is_same_v<Tag, gpio::AFRL_Tag>)
                    static_assert(static_cast<uint8_t>(Pin) < 8, "Alternate function LOW register accepts 0-7 pins only!");
                else if constexpr (std::is_same_v<Tag, gpio::AFRH_Tag>)
                    static_assert(static_cast<uint8_t>(Pin) >= 8, "Alternate function HIGH register accepts 8-15 pins only!");
            }

        /**
         * @brief Constructs a raw mask that covers only the pin's bitfield without a value.
         * 
         * Useful for clear-only operations, for masking out bits, or for reading.
         * 
         * @param pin GPIO pin number.
         */
        constexpr PinMask()
            : RegisterMask<Tag, AccessFlag, Width, static_cast<uint8_t>(Pin) * Width> {((1U << Width) - 1)} {}
    };

    /// @brief GPIO mode mask (MODER register, 2 bits per pin).
    template<Pins Pin>
    using ModeMask = PinMask<gpio::MODER_Tag, reg::BitFieldAccessFlag::RW, 2, gpio::Mode, Pin>;

    /// @brief GPIO output type mask (OTYPER register, 1 bit per pin).
    template<Pins Pin>
    using OutputTypeMask = PinMask<gpio::OTYPER_Tag, reg::BitFieldAccessFlag::RW, 1, gpio::OutputType, Pin>;

    /// @brief GPIO output speed mask (OSPEEDR register, 2 bits per pin).
    template<Pins Pin>
    using OutputSpeedMask = PinMask<gpio::OSPEEDR_Tag, reg::BitFieldAccessFlag::RW, 2, gpio::OutputSpeed, Pin>;

    /// @brief GPIO pull-up/pull-down mask (PUPDR register, 2 bits per pin).
    template<Pins Pin>
    using PullTypeMask = PinMask<gpio::PUPDR_Tag, reg::BitFieldAccessFlag::RW, 2, gpio::PullType, Pin>;

    /// @brief GPIO input data mask (IDR register, 1 bit per pin).
    template<Pins Pin>
    using InputDataMask = PinMask<gpio::IDR_Tag, reg::BitFieldAccessFlag::RO, 1, bool, Pin>;

    /// @brief GPIO output data mask (ODR register, 1 bit per pin).
    template<Pins Pin>
    using OutputDataMask = PinMask<gpio::ODR_Tag, reg::BitFieldAccessFlag::RW, 1, bool, Pin>;

    /// @brief GPIO bit set mask (BSRR register, 1 bit per pin).
    template<Pins Pin>
    using BitSetMask = PinMask<gpio::BSRR_Tag, reg::BitFieldAccessFlag::RW, 1, bool, Pin>;
    /// @brief GPIO bit reset mask (BSRR register, 1 bit per pin).

    template<Pins Pin>
    using BitResetMask = PinMask<gpio::BSRR_Tag, reg::BitFieldAccessFlag::RW, 1, bool, Pin, 16>;

    /*
        TODO: Implement LockMask and Lock register
    */
    //using LockMask        = PinMask<gpio::LCKR_Tag, GPIO::PullType, 2>;

    /// @brief GPIO alternate function low mask (AFRL register, 4 bits per pin).
    template<Pins Pin>
    using AltFuncLowMask  = PinMask<gpio::AFRL_Tag, reg::BitFieldAccessFlag::RW, 4, gpio::AlternateFunc, Pin>;

    /// @brief GPIO alternate function high mask (AFRH register, 4 bits per pin).
    template<Pins Pin>
    using AltFuncHighMask = PinMask<gpio::AFRH_Tag, reg::BitFieldAccessFlag::RW, 4, gpio::AlternateFunc, Pin>;

};

/**
 * @brief GPIO abstraction for a specific port.
 * 
 * Static class.
 * 
 * @tparam Port GPIO port (A, B, C, etc.).
 */
template<gpio::Port Port>
class GpioRegs
{
    private:
        inline static constexpr uint32_t BASE_ADDR = static_cast<uint32_t>(Port);
    public:
        GpioRegs() = delete;

        /// @brief GPIO mode register.
        using ModeReg           = Register<gpio::MODER_Tag,   BASE_ADDR + 0x00>;

        /// @brief Output type register.
        using OutputTypeReg     = Register<gpio::OTYPER_Tag,  BASE_ADDR + 0x04>;

        /// @brief Output speed register.
        using OutputSpeedReg    = Register<gpio::OSPEEDR_Tag, BASE_ADDR + 0x08>;

        /// @brief Pull-up/pull-down register.
        using PullTypeReg       = Register<gpio::PUPDR_Tag,   BASE_ADDR + 0x0C>;

        /// @brief Input data register.
        using InputDataReg      = Register<gpio::IDR_Tag,     BASE_ADDR + 0x10>;

        /// @brief Output data register.
        using OutputDataReg     = Register<gpio::ODR_Tag,     BASE_ADDR + 0x14>;

        /// @brief Bit set/reset register.
        using BitSetResetReg    = Register<gpio::BSRR_Tag,    BASE_ADDR + 0x18>;

        // /// @brief Configuration lock register.
        // using LockReg           = Register<gpio::LCKR_Tag,    LCKR_ADDR + 0x1C>;

        /// @brief Alternate function low register (pins 0–7).
        using AltFuncLowReg     = Register<gpio::AFRL_Tag,    BASE_ADDR + 0x20>;

        /// @brief Alternate function high register (pins 8–15).
        using AltFuncHighReg    = Register<gpio::AFRH_Tag,    BASE_ADDR + 0x24>;
};

#endif
