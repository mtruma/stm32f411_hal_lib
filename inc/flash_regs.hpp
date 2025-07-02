#ifndef _FLASHREGS_HPP_
#define _FLASHREGS_HPP_

#include "./register_base.hpp"

#include <stdint.h>
#include <assert.h>

namespace flash
{
    struct ACR_Tag {};

    enum class Latency : uint8_t
    {
        WaitState_0 = 0U,
        WaitState_1,
        WaitState_2,
        WaitState_3,
        WaitState_4,
        WaitState_5,
        WaitState_6
    };

    using LatencyMask = RegisterMask<ACR_Tag, reg::BitFieldAccessFlag::RW, 4, 0, Latency>;
};

/**
 * @brief Flash registers abstraction.
 * 
 * Provides type-safe register access using static member types, allowing
 * fine-grained control of Flash registers.
 */
 class FlashRegs
 {
     private:
        inline static constexpr uint32_t BASE_ADDR = 0x40023C00UL;
     public:
        FlashRegs() = delete;
        
        using AccessControlReg = Register<flash::ACR_Tag, BASE_ADDR + 0x00>;
 };

#endif