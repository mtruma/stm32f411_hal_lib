#ifndef _RCCREGS_HPP_
#define _RCCREGS_HPP_

#include "./register_base.hpp"

#include <stdint.h>
#include <assert.h>
#include <type_traits>

namespace rcc
{
    struct CR_Tag {};

    struct PLLCFGR_Tag {};

    struct CFGR_Tag {};

    struct AHB1RSTR_Tag {};

    struct AHB2RSTR_Tag {};

    struct APB1RSTR_Tag {};

    struct APB2RSTR_Tag {};

    struct AHB1ENR_Tag {};

    struct AHB2ENR_Tag {};

    struct APB1ENR_Tag {};

    struct APB2ENR_Tag {};

    enum class PllReady : bool
    {
        Unlocked = false,
        Locked   = true
    };

    enum class PllI2sReady : bool
    {
        Unlocked = false,
        Locked   = true
    };

    enum class PllP : uint32_t
    {
        Div_2 = 0U,
        Div_4,
        Div_6,
        Div_8
    };

    enum class PllSource : uint8_t
    {
        Hsi = 0U,
        Hse
    };

    enum class SysClkSwitch : uint8_t
    {
        Hsi = 0U,
        Hse,
        Pll
    };

    enum class AHBPrescaler : uint8_t
    {
        Pre_0 = 0U,
        Pre_2 = 8U,
        Pre_4,
        Pre_8,
        Pre_16,
        Pre_64,
        Pre_128,
        Pre_256,
        Pre_512
    };

    enum class APB1Prescaler : uint8_t
    {
        Pre_0 = 0U,
        Pre_2 = 4U,
        Pre_4,
        Pre_8,
        Pre_16
    };

    enum class APB2Prescaler : uint8_t
    {
        Pre_0 = 1U,
        Pre_2 = 4U,
        Pre_4,
        Pre_8,
        Pre_16
    };

    enum class McuClkOut1 : uint8_t
    {
        Hsi = 0U,
        Lse,
        Hse,
        Pll
    };

    enum class McuClkOut2 : uint8_t
    {
        SystemClk = 0U,
        PllI2S,
        Hse,
        Pll
    };

    enum class I2SClkSel : uint8_t
    {
        PLLI2S = 0U,
        External
    };
    
    enum class McuClkOutPre : uint8_t
    {
        Pre_0 = 1U,
        Pre_2 = 4U,
        Pre_3,
        Pre_4,
        Pre_5
    };

    // Control register
    using HsiOnMask            = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using HsiReadyMask         = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RO, 1, 1,  bool>;
    using HsiTrimMask          = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 4, 3,  uint8_t>;
    using HsiCalMask           = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RO, 7, 8,  uint8_t>;
    using HseOnMask            = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 16, bool>;
    using HseReadyMask         = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RO, 1, 17, bool>;
    using HseBypassMask        = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 18, bool>;
    using ClkSecuritySysOnMask = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 19, bool>;
    using PllOnMask            = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 24, bool>;
    using PllReadyMask         = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RO, 1, 25, PllReady>;
    using PllI2sOnMask         = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 1, 26, bool>;
    using PllI2sReadyMask      = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RO, 1, 27, PllI2sReady>;

    // PLL configuration register
    using PllMMask             = RegisterMask<PLLCFGR_Tag, reg::BitFieldAccessFlag::RW, 6, 0,  uint32_t>;
    using PllNMask             = RegisterMask<PLLCFGR_Tag, reg::BitFieldAccessFlag::RW, 9, 6,  uint32_t>;
    using PllPMask             = RegisterMask<PLLCFGR_Tag, reg::BitFieldAccessFlag::RW, 2, 16, PllP>;
    using PllSrcMask           = RegisterMask<PLLCFGR_Tag, reg::BitFieldAccessFlag::RW, 1, 22, PllSource>;
    using PllQMask             = RegisterMask<PLLCFGR_Tag, reg::BitFieldAccessFlag::RW, 4, 24, uint32_t>;

    // Configuration register
    using SysClkSwitchMask     = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 2, 0,  SysClkSwitch>;
    using SysClkSwitchStatMask = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RO, 2, 2,  SysClkSwitch>;
    using AHBPrescalerMask     = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 4, 4,  AHBPrescaler>;
    using APB1PrescalerMask    = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 3, 10, APB1Prescaler>;
    using APB2PrescalerMask    = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 3, 13, APB2Prescaler>;
    using RTCPrescalerMask     = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 5, 16, uint8_t>;
    using McuClkOut1Mask       = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 2, 21, McuClkOut1>;
    using I2SClkSelMask        = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 1, 23, I2SClkSel>;
    using McuClkOut1PreMask    = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 3, 24, McuClkOutPre>;
    using McuClkOut2PreMask    = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 3, 27, McuClkOutPre>;
    using McuClkOut2Mask       = RegisterMask<CFGR_Tag, reg::BitFieldAccessFlag::RW, 2, 30, McuClkOut2>;

    // AHB1 reset register
    using GpioAResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using GpioBResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 1,  bool>;
    using GpioCResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 2,  bool>;
    using GpioDResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 3,  bool>;
    using GpioEResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 4,  bool>;
    using GpioHResetMask = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 7,  bool>;
    using CRCResetMask   = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 12, bool>;
    using DMA1ResetMask  = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 21, bool>;
    using DMA2ResetMask  = RegisterMask<AHB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 22, bool>;

    // AHB2 reset register
    using OTGFSResetMask = RegisterMask<AHB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 7,  bool>;

    // APB1 reset register
    using Tim2ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using Tim3ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 1,  bool>;
    using Tim4ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 2,  bool>;
    using Tim5ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 3,  bool>;
    using WindowWatchdogRstMask = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 11, bool>;
    using Spi2ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 14, bool>;
    using Spi3ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 15, bool>;
    using Usart2ResetMask       = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 17, bool>;
    using I2C1ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 21, bool>;
    using I2C2ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 22, bool>;
    using I2C3ResetMask         = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 23, bool>;
    using PowerResetMask        = RegisterMask<APB1RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 28, bool>;

    // APB2 reset register
    using Tim1ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using Usart1ResetMask       = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 4,  bool>;
    using Usart6ResetMask       = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 5,  bool>;
    using Adc1ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 8,  bool>;
    using SdioResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 11, bool>;
    using Spi1ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 12, bool>;
    using Spi4ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 13, bool>;
    using SysConfigCtrlRstMask  = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 14, bool>;
    using Tim9ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 16, bool>;
    using Tim10ResetMask        = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 17, bool>;
    using Tim11ResetMask        = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 18, bool>;
    using Spi5ResetMask         = RegisterMask<APB2RSTR_Tag, reg::BitFieldAccessFlag::RW, 1, 20, bool>;

    // AHB1 enable register
    using GpioAEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using GpioBEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 1,  bool>;
    using GpioCEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 2,  bool>;
    using GpioDEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 3,  bool>;
    using GpioEEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 4,  bool>;
    using GpioHEnableMask = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 7,  bool>;
    using CRCEnableMask   = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 12, bool>;
    using DMA1EnableMask  = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 21, bool>;
    using DMA2EnableMask  = RegisterMask<AHB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 22, bool>;

    // AHB2 enable register
    using OTGFSEnableMask = RegisterMask<AHB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 7,  bool>;

    // APB1 enable register
    using Tim2EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using Tim3EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 1,  bool>;
    using Tim4EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 2,  bool>;
    using Tim5EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 3,  bool>;
    using WindowWatchdogEnMask   = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 11, bool>;
    using Spi2EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 14, bool>;
    using Spi3EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 15, bool>;
    using Usart2EnableMask       = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 17, bool>;
    using I2C1EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 21, bool>;
    using I2C2EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 22, bool>;
    using I2C3EnableMask         = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 23, bool>;
    using PowerEnableMask        = RegisterMask<APB1ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 28, bool>;

    // APB2 enable register
    using Tim1EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using Usart1EnableMask       = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 4,  bool>;
    using Usart6EnableMask       = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 5,  bool>;
    using Adc1EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 8,  bool>;
    using SdioEnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 11, bool>;
    using Spi1EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 12, bool>;
    using Spi4EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 13, bool>;
    using SysConfigCtrlEnMask    = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 14, bool>;
    using Tim9EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 16, bool>;
    using Tim10EnableMask        = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 17, bool>;
    using Tim11EnableMask        = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 18, bool>;
    using Spi5EnableMask         = RegisterMask<APB2ENR_Tag, reg::BitFieldAccessFlag::RW, 1, 20, bool>;
};

/**
 * @brief RCC(Reset & Clock Control Register) abstraction.
 * 
 * Provides type-safe register access using static member types, allowing
 * fine-grained control of Reset and Clock control registers.
 */
 class ResetClockCtrlRegs
 {
     private:
        inline static constexpr uint32_t BASE_ADDR = 0x40023800UL;
     public:
        ResetClockCtrlRegs() = delete;
        
        using ClockControlReg = Register<rcc::CR_Tag,       BASE_ADDR + 0x00>;
        using PllConfigReg    = Register<rcc::PLLCFGR_Tag,  BASE_ADDR + 0x04>;
        using ConfigReg       = Register<rcc::CFGR_Tag,     BASE_ADDR + 0x08>;
        using Ahb1ResetReg    = Register<rcc::AHB1RSTR_Tag, BASE_ADDR + 0x10>;
        using Ahb2ResetReg    = Register<rcc::AHB2RSTR_Tag, BASE_ADDR + 0x14>;
        using Apb1ResetReg    = Register<rcc::APB1RSTR_Tag, BASE_ADDR + 0x20>;
        using Apb2ResetReg    = Register<rcc::APB2RSTR_Tag, BASE_ADDR + 0x24>;
        using Ahb1EnableReg   = Register<rcc::AHB1ENR_Tag,  BASE_ADDR + 0x30>;
        using Ahb2EnableReg   = Register<rcc::AHB2ENR_Tag,  BASE_ADDR + 0x34>;
        using Apb1EnableReg   = Register<rcc::APB1ENR_Tag,  BASE_ADDR + 0x40>;
        using Apb2EnableReg   = Register<rcc::APB2ENR_Tag,  BASE_ADDR + 0x44>;
 };

#endif