#ifndef _SPIREGS_HPP_
#define _SPIREGS_HPP_

#include "register_base.hpp"

#include <cstdint>
#include <stdint.h>
#include <assert.h>

/**
 * @brief GPIO-related types and utilities for STM32-style GPIO peripheral access.
 */
namespace spi
{
    enum class Peripherals : uint32_t
    {
        Spi_1 = 0x40013000UL,
        Spi_2 = 0x40003800UL,
        Spi_3 = 0x40003C00UL,
        Spi_4 = 0x40013400UL,
        Spi_5 = 0x40015000UL
    };

    enum class ClockPhase : uint32_t
    {
        FirstEdge  = 0U,
        SecondEdge = 1U
    };

    enum class ClockPolarity : uint32_t
    {
        Low = 0U,
        High = 1U
    };

    enum class MasterSelection : uint32_t
    {
        Slave  = 0U,
        Master = 1U
    };

    enum class BaudRateControl: uint32_t
    {
        Div_2   = 0U,
        Div_4   = 1U,
        Div_8   = 2U,
        Div_16  = 3U,
        Div_32  = 4U,
        Div_64  = 5U,
        Div_128 = 6U,
        Div_256 = 7U
    };

    enum class BitOrder : uint32_t
    {
        MostSignificBit  = 0U,
        LeastSignifBit   = 1U
    };

    enum class InternSlaveSelect : uint32_t
    {
        Select    = 0U,
        Deselect  = 1U
    };

    enum class SlaveMng : uint32_t
    {
        Hardware = 0U,
        Software = 1U
    };

    enum class TxMode : uint32_t
    {
        FullDuplex     = 0U,
        ReceiveOnly    = 1U
    };

    enum class DataFrameFormat : uint32_t
    {
        _8bit  = 0U,
        _16bit = 1U
    };

    enum class CrcNext : uint32_t
    {
        DataPhase         = 0U,
        NextTransferIsCRC = 1U
    };

    enum class BidiOutput : uint32_t
    {
        RxOnly  = 0U,
        TxOnly  = 1U
    };

    enum class BidiDataMode : uint32_t
    {
        TwoLines  = 0U,
        OneLine   = 1U
    };

    enum class FrameFormat : uint32_t
    {
        MotorolaMode = 0U,
        TIMode       = 1U
    };

    struct CR1_Tag {};
    struct CR2_Tag {};
    struct SR_Tag {};
    struct DR_Tag {};

    using ClkPhaseMask        = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 0, ClockPhase>;
    using ClkPolarityMask     = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 1, ClockPolarity>;
    using MasterSelectMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 2, MasterSelection>;
    using BaudRateCtrlMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 3, 3, BaudRateControl>;
    using SpiEnableMask       = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 6, bool>;
    using BitOrderMask        = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 7, BitOrder>;
    using InternSlaveSelMask  = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 8, InternSlaveSelect>;
    using SlaveMngMask        = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 9, SlaveMng>;
    using TxModeMask          = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 10, TxMode>;
    using DataFrameFormatMask = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 11, DataFrameFormat>;
    using CrcNextMask         = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 12, CrcNext>;
    using CrcEnableMask       = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 13, bool>;
    using BidiOutputMask      = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 14, BidiOutput>;
    using BidiDataModeMask    = RegisterMask<CR1_Tag, reg::BitFieldAccessFlag::RW, 1, 15, BidiDataMode>;

    using RxBuffDmaEnMask       = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 0, bool>;
    using TxBuffDmaEnMask       = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 1, bool>;
    using SlaveSelOutEnMask     = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 2, bool>;
    using FrameFormatMask       = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 4, FrameFormat>;
    using ErrIEnMask            = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 5, bool>;
    using RxBuffNotEmptyIEnMask = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 6, bool>;
    using TxBuffNotEmptyIEnMask = RegisterMask<CR2_Tag, reg::BitFieldAccessFlag::RW, 1, 7, bool>;

    using RxNotEmptyStatMask     = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 0, bool>;
    using TxNotEmptyStatMask     = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 1, bool>;
    using ChSideStatMask         = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 2, bool>;
    using UnderrrunStatMask      = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 3, bool>;
    using CrcErrStatMask         = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 4, bool>;
    using ModeFaultStatMask      = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 5, bool>;
    using OverrunStatMask        = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 6, bool>;
    using BusyStatMask           = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 7, bool>;
    using FrameFormatErrStatMask = RegisterMask<SR_Tag, reg::BitFieldAccessFlag::RW, 1, 8, bool>;

    using DataMask               = RegisterMask<DR_Tag,  reg::BitFieldAccessFlag::RW, 16, 0,  uint16_t>;
};

template<spi::Peripherals Periph>
class SpiRegs
{
    private:
        inline static constexpr uint32_t BASE_ADDR = static_cast<uint32_t>(Periph);
    public:
        SpiRegs() = delete;

        using ControlReg1 = Register<spi::CR1_Tag, BASE_ADDR + 0x00>;
        using ControlReg2 = Register<spi::CR2_Tag, BASE_ADDR + 0x04>;
        using StatusReg   = Register<spi::SR_Tag,  BASE_ADDR + 0x08>;
        using DataReg     = Register<spi::DR_Tag,  BASE_ADDR + 0x0C>;
};

#endif
