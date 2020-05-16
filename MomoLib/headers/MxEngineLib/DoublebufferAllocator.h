#pragma once

#include <cstdint>
#include <cstdlib>
#include <utility>

namespace MxEngine
{
    /*
    Doublebuffer allocator class by #Momo
    uses pair of two allocators, one for allocation, one for storage
    when swapped, current allocator becomes storage allocator, storage is cleared and become current
    */
    template<typename AllocatorBase>
    class DoublebufferAllocator
    {
        /*!
        pair of two allocators with type AllocatorBase
        */
        std::pair<AllocatorBase, AllocatorBase> allocators;
        /*!
        memory chunk data for two allocators, first is stored in 0 and 1 indicies, second in 2 and 3 indicies
        */
        std::tuple<uint8_t*, size_t, uint8_t*, size_t> data = { nullptr, 0, nullptr, 0 };
    public:
        /*!
        initializes doublebuffer allocator
        \param base1 begin of memory chunk for first  allocator (current)
        \param size1 size  of memory chunk for first  allocator (current)
        \param base2 begin of memory chunk for second allocator (storage)
        \param size2 size  of memory chunk for second allocator (storage)
        */
        void Init(uint8_t* base1, size_t size1, uint8_t* base2, size_t size2)
        {
            allocators.first.Init(base1, size1);
            allocators.second.Init(base2, size2);
            data = { base1, size1, base2, size2 };
        }

        DoublebufferAllocator() = default;

        /*!
        constructs doublebuffer allocator
        \param base1 begin of memory chunk for first  allocator (current)
        \param size1 size  of memory chunk for first  allocator (current)
        \param base2 begin of memory chunk for second allocator (storage)
        \param size2 size  of memory chunk for second allocator (storage)
        */
        DoublebufferAllocator(uint8_t* base1, size_t size1, uint8_t* base2, size_t size2)
        {
            this->Init(base1, size1, base2, size2);
        }

        /*!
        swaps allocators. Storage allocator becomes current, but losts all its data, current allocator becomes storage one
        */
        void Swap()
        {
            std::swap(allocators.first, allocators.second);
            std::swap(std::get<0>(data), std::get<2>(data));
            std::swap(std::get<1>(data), std::get<3>(data));

            allocators.first.Init(std::get<0>(data), std::get<1>(data));
        }

        /*!
        getter for first allocator
        \returns current allocator which is used for allocation
        */
        AllocatorBase& Current() { return allocators.first; }
        /*!
        getter for second allocator
        \returns second allocator which is used for storage
        */
        AllocatorBase& Swapped() { return allocators.second; }
    };
}