#ifndef _POWERREGS_HPP_
#define _POWERREGS_HPP_

#include "./register_base.hpp"

#include <stdint.h>
#include <assert.h>
#include <type_traits>

namespace pwr
{
    struct CR_Tag {};

    enum class VoltageScalingOutSel : uint32_t
    {
        Default = 0U,
        Scale_3,
        Scale_2,
        Scale_1
    };

    using VoltageScalingOutSelMask = RegisterMask<CR_Tag, reg::BitFieldAccessFlag::RW, 2, 14, VoltageScalingOutSel>;
};

/**
 * @brief Power Controller registers abstraction.
 * 
 * Static class.
 */
 class PowerCtrlRegs
 {
     private:
        inline static constexpr uint32_t BASE_ADDR = 0x40007000UL;
     public:
        PowerCtrlRegs() = delete;
        
        using ControlReg = Register<pwr::CR_Tag, BASE_ADDR + 0x00>;
 };

#endif