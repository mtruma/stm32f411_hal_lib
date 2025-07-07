/**
 * @brief Example for NUCLEO-F411RE
 *
 * PA5 -> onboard led
 * This board uses stlink that generates input source clock 
 * instead of oscilator in many dev boards (like blackpill).
 * That is why HSE Bypass is enabled otherwise dont enable this.
 *
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "flash_regs.hpp"
#include "pwr_regs.hpp"
#include "rcc_regs.hpp"
#include "gpio_regs.hpp"

#include "system_stm32f4xx.h"

#include <stdint.h>

// Init system clock to 96Mhz
void system_init(void)
{
    // Config input source clock (be aware that HSE bypass is enabled in this example, you maybe not need it enabled)
    ResetClockCtrlRegs::ClockControlReg::set(rcc::HseBypassMask(true) | rcc::HseOnMask(true));
    while(!(ResetClockCtrlRegs::ClockControlReg::read(rcc::HseReadyMask())));

    // Enable power interface clock
    ResetClockCtrlRegs::Apb1EnableReg::set(rcc::PowerEnableMask());

    // Set voltage scaling to support our freq of 96Mhz
    PowerCtrlRegs::ControlReg::set(pwr::VoltageScalingOutSelMask(pwr::VoltageScalingOutSel::Scale_1));
    // Config Flash register to wait state 3 as described in reference manual (this is required otherwise it will not work properly)
    FlashRegs::AccessControlReg::set(flash::LatencyMask(flash::Latency::WaitState_3));

    // Configure PLL
    ResetClockCtrlRegs::PllConfigReg::clear(rcc::PllMMask() | rcc::PllNMask() | rcc::PllPMask() | rcc::PllQMask() | rcc::PllSrcMask());
    ResetClockCtrlRegs::PllConfigReg::set(rcc::PllMMask(4) | rcc::PllNMask(192) | rcc::PllPMask(rcc::PllP::Div_4) | rcc::PllSrcMask(rcc::PllSource::Hse));
    ResetClockCtrlRegs::ConfigReg::set(rcc::APB1PrescalerMask(rcc::APB1Prescaler::Pre_2));

    // Enable PLL and wait until it is stabilised
    ResetClockCtrlRegs::ClockControlReg::set(rcc::PllOnMask(true));
    while(!(ResetClockCtrlRegs::ClockControlReg::read(rcc::PllReadyMask())));
        
    // Switch to using PLL as system clock and wait until it is stabilised
    ResetClockCtrlRegs::ConfigReg::set(rcc::SysClkSwitchMask(rcc::SysClkSwitch::Pll));
    while (!(ResetClockCtrlRegs::ConfigReg::read(rcc::SysClkSwitchStatMask())));
}

void init_onboard_led()
{
    // First enable clock for GPIOA
    ResetClockCtrlRegs::Ahb1EnableReg::set(rcc::GpioAEnableMask(true));

    // Configure mode for PA5 -> onboard LED on NUCLEO-F411RE
    GpioRegs<gpio::Port::A>::ModeReg::set(gpio::ModeMask<gpio::Pins::P5>(gpio::Mode::Output));
}

int main()
{
    system_init();
    init_onboard_led();

    auto onboard_led_mask = gpio::OutputDataMask<gpio::Pins::P5>();

    while (true)
    {
        // For precise delay there are better implementation, this is only for example
        for (auto count = 0; count < 9600000; ++count);
        // Turn on led
        GpioRegs<gpio::Port::A>::OutputDataReg::set(onboard_led_mask);
        for (auto count = 0; count < 9600000; ++count);
        // Turn off led
        GpioRegs<gpio::Port::A>::OutputDataReg::clear(onboard_led_mask);

    }
    return 0;
}


