#ifndef _USARTREGS_HPP_
#define _USARTREGS_HPP_

#include "register_base.hpp"

#include <cstdint>
#include <stdint.h>
#include <assert.h>

namespace usart
{
    struct SR_Tag {};

    struct DR_Tag {};

    struct BRR_Tag {};

    struct CR1_Tag {};

    struct CR2_Tag {};

    struct CR3_Tag {};

    enum class Peripherals : uint32_t
    {
        Usart1 = 0x40011000UL,
        Usart2 = 0x40004400UL,
        Usart6 = 0x40011400UL 
    };

    enum class ReceiverWakeUp : uint8_t
    {
        ActiveMode = 0U,
        MuteMode
    };

    enum class Oversampling : uint8_t
    {
        Over_16 = 0U,
        Over_8
    };

    using ParityErrStatMask   = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 0, bool>;
    using FramingErrStatMask  = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 1, bool>;
    using NoiseDetStatMask    = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 2, bool>;
    using OverrunErrStatMask  = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 3, bool>;
    using IdleLineDetStatMask = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 4, bool>;
    using RxNotEmptyStatMask  = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 5, bool>;
    using TxCompleteStatMask  = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 6, bool>;
    using TxEmptyStatMask     = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 7, bool>;
    using LinBreakDetStatMask = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 8, bool>;
    using ClearToSendStatMask = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 9, bool>;

    using DataMask             = RegisterMask<DR_Tag,  reg::BitFieldAccessFlag::RW, 8, 0,  uint8_t>;
    using BaudRateFractionMask = RegisterMask<BRR_Tag, reg::BitFieldAccessFlag::RW, 4, 0,  uint8_t>;
    using BaudRateMantissaMask = RegisterMask<BRR_Tag, reg::BitFieldAccessFlag::RW, 12, 4, uint16_t>;

    using SendBreakCharMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 0,  bool>;
    using ReceiverWakeUpMask   = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 1,  ReceiverWakeUp>;
    using RxEnableMask         = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 2,  bool>;
    using TxEnableMask         = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 3,  bool>;
    using RxNotEmptyIEnMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 5,  bool>;
    using TxCompleteIEnMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 6,  bool>;
    using TxEmptyIEnMask       = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 7,  bool>;
    using UsartEnableMask      = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 13, bool>;
    using OversamplingModeMask = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 15, Oversampling>;    

    using ErrorIEnableMask     = RegisterMask<CR3_Tag, reg::BitFieldAccessFlag::RW, 1, 0, bool>;
    using DmaRxEnableMask      = RegisterMask<CR3_Tag, reg::BitFieldAccessFlag::RW, 1, 6, bool>;
    using DmaTxEnableMask      = RegisterMask<CR3_Tag, reg::BitFieldAccessFlag::RW, 1, 7, bool>;
};

/**
 * @brief USART abstraction.
 * 
 * Static class.
 * 
 * @tparam Periph USART peripheral (USART1, USART2...).
 */
template<usart::Peripherals Periph>
class UsartRegs
{
    private:
        inline static constexpr uint32_t BASE_ADDR = static_cast<uint32_t>(Periph);
    public:
        UsartRegs() = delete;

        using StatusReg   = Register<usart::SR_Tag,  BASE_ADDR + 0x00>;
        using DataReg     = Register<usart::DR_Tag,  BASE_ADDR + 0x04>;
        using BaudRateReg = Register<usart::BRR_Tag, BASE_ADDR + 0x08>;
        using ControlReg1 = Register<usart::CR1_Tag, BASE_ADDR + 0x0C>;
        using ControlReg2 = Register<usart::CR2_Tag, BASE_ADDR + 0x10>;
        using ControlReg3 = Register<usart::CR3_Tag, BASE_ADDR + 0x14>;
};

#endif
