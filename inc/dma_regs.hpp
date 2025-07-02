#ifndef _DMAREGS_HPP_
#define _DMAREGS_HPP_

#include "register_base.hpp"

#include <cstdint>
#include <stdint.h>

namespace dma
{
    enum class Peripherals : uint32_t
    {
        Dma_1 = 0x40026000UL,
        Dma_2 = 0x40026400UL
    };

    enum class Streams : uint8_t
    {
        Stream_0 = 0U,
        Stream_1,
        Stream_2,
        Stream_3,
        Stream_4,
        Stream_5,
        Stream_6,
        Stream_7
    };

    enum class Channels : uint8_t
    {
        Ch_0 = 0U,
        Ch_1,
        Ch_2,
        Ch_3,
        Ch_4,
        Ch_5,
        Ch_6,
        Ch_7
    };

    enum class DataSize : uint8_t
    {
        Byte = 0U,
        HalfWord,
        Word
    };

    enum class PriorityLevel : uint8_t
    {
        Low = 0U,
        Medium,
        High,
        VeryHigh
    };

    enum class AddrIncrementMode : uint8_t
    {
        AddrPtrFixed = 0U,
        AddrPtrIncr
    };

    enum class TransferDirection : uint8_t
    {
        PeriphToMem = 0U,
        MemToPeriph,
        MemToMem
    };

    enum class FifoThreshold : uint8_t
    {
        Full_25 = 0U,
        Full_50,
        Full_75,
        Full_100
    };

    enum class FifoStatus : uint8_t
    {
        Less_25 = 0U,
        Less_50,
        Less_75,
        Less_100,
        Empty,
        Full
    };

    constexpr uint32_t StatusFieldsPos[] = 
    {
        5, 11, 21, 27,
        5, 11, 21, 27
    };

    struct LISR_Tag {};

    struct HISR_Tag {};

    struct LIFCR_Tag {};

    struct HIFCR_Tag {};

    struct SxCR_Tag {};

    struct SxNDTR_Tag {};

    struct SxPAR_Tag {};

    struct SxM0AR_Tag {};

    struct SxM1AR_Tag {};

    struct SxFCR_Tag {};

    template<typename Tag, reg::BitFieldAccessFlag AccessFlag, Streams Stream>
    struct StatusMask : RegisterMask<Tag, AccessFlag, 1, StatusFieldsPos[static_cast<uint8_t>(Stream)]> 
    {
        /**
         * @brief Constructs a raw mask that covers only the pin's bitfield without a value.
         * 
         * Useful for clear-only operations or for masking out bits, or for reading.
         * 
         * @param pin GPIO pin number.
         */
        constexpr StatusMask()
            : RegisterMask<Tag, AccessFlag, 1, StatusFieldsPos[static_cast<uint8_t>(Stream)]>() 
        {
            if constexpr (std::is_same_v<Tag, LISR_Tag> || std::is_same_v<Tag, LIFCR_Tag>)
                static_assert(static_cast<uint8_t>(Stream) < 4, "I don't belong here, I belong in HIGH register ;) !");
            else if constexpr (std::is_same_v<Tag, HISR_Tag> || std::is_same_v<Tag, HIFCR_Tag>)
                static_assert(static_cast<uint8_t>(Stream) >= 4, "I don't belong here, I belong in LOW register ;) !");
        }
    };

    template<Streams Stream>
    using TxCompleteIStatMask = StatusMask<std::conditional_t<static_cast<uint32_t>(Stream) < 4, LISR_Tag, HISR_Tag>, reg::BitFieldAccessFlag::RO, Stream>;

    template<Streams Stream>
    using TxCompleteIClrMask = StatusMask<std::conditional_t<static_cast<uint32_t>(Stream) < 4, LIFCR_Tag, HIFCR_Tag>, reg::BitFieldAccessFlag::WO, Stream>;

    using StreamEnableMask     = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 0, bool>;
    using DirectModeErrIEnMask = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 1, bool>;
    using TxErrIEnableMask     = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 2, bool>;
    using HalfTxIEnableMask    = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 3, bool>;
    using TxIEnableMask        = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 4, bool>;
    using TxDirectionMask      = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 2, 6, TransferDirection>;
    using CircularModeMask     = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 8, bool>;
    using PeriphIncrModeMask   = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 9, AddrIncrementMode>;
    using MemIncrModeMask      = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 1, 10, AddrIncrementMode>;
    using PeriphDataSizeMask   = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 2, 11, DataSize>;
    using MemDataSizeMask      = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 2, 13, DataSize>;
    using PriorityLvlMask      = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 2, 16, PriorityLevel>;
    using ChannelSelMask       = RegisterMask<SxCR_Tag, reg::BitFieldAccessFlag::RW, 3, 25, Channels>;

    using NumOfDataMask        = RegisterMask<SxNDTR_Tag, reg::BitFieldAccessFlag::RW, 16, 0, uint16_t>;

    using PeriphAddrMask       = RegisterMask<SxPAR_Tag,  reg::BitFieldAccessFlag::RW, 32, 0, uint32_t>;
    using Mem0AddrMask         = RegisterMask<SxM0AR_Tag, reg::BitFieldAccessFlag::RW, 32, 0, uint32_t>;
    using Mem1AddrMask         = RegisterMask<SxM1AR_Tag, reg::BitFieldAccessFlag::RW, 32, 0, uint32_t>;

    using FifoThresholdMask    = RegisterMask<SxFCR_Tag, reg::BitFieldAccessFlag::RW, 2, 0, FifoThreshold>;
    using DirectModeDisMask    = RegisterMask<SxFCR_Tag, reg::BitFieldAccessFlag::RW, 1, 2, bool>;
    using FifoStatusMask       = RegisterMask<SxFCR_Tag, reg::BitFieldAccessFlag::RO, 3, 3, FifoStatus>;
    using FifoErrIEnMask       = RegisterMask<SxFCR_Tag, reg::BitFieldAccessFlag::RW, 1, 7, bool>;

};

template<dma::Peripherals Peripheral>
class DmaRegs
{
    private:
        inline static constexpr uint32_t BASE_ADDR = static_cast<uint32_t>(Peripheral);
    public:
        using LowIStatReg   = Register<dma::LISR_Tag,  BASE_ADDR + 0x00>;
        using HighIStatReg  = Register<dma::HISR_Tag,  BASE_ADDR + 0x04>;
        using LowIClearReg  = Register<dma::LIFCR_Tag, BASE_ADDR + 0x08>;
        using HighIClearReg = Register<dma::HIFCR_Tag, BASE_ADDR + 0x0C>;

        template<dma::Streams Stream>
        using ConfigReg     = Register<dma::SxCR_Tag, BASE_ADDR + 0x10 + static_cast<uint32_t>(Stream) * 0x18>;

        template<dma::Streams Stream>
        using NumOfDataReg  = Register<dma::SxNDTR_Tag, BASE_ADDR + 0x14 + static_cast<uint32_t>(Stream) * 0x18>;

        template<dma::Streams Stream>
        using PeriphAddrReg  = Register<dma::SxPAR_Tag, BASE_ADDR + 0x18 + static_cast<uint32_t>(Stream) * 0x18>;

        template<dma::Streams Stream>
        using Mem0AddrReg  = Register<dma::SxM0AR_Tag, BASE_ADDR + 0x1C + static_cast<uint32_t>(Stream) * 0x18>;

        template<dma::Streams Stream>
        using Mem1AddrReg  = Register<dma::SxM1AR_Tag, BASE_ADDR + 0x20 + static_cast<uint32_t>(Stream) * 0x18>;

        template<dma::Streams Stream>
        using FifoControlReg  = Register<dma::SxFCR_Tag, BASE_ADDR + 0x24 + static_cast<uint32_t>(Stream) * 0x18>;
};


#endif
