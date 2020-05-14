#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <memory>

#define MX_ASSERT(x) assert(x)

/*
LinearAllocator class by #Momo
accepts chunk of memory and its size, do NOT allocate or free memory by itself
allocates aligned chunk of memory, adjusting `top` pointer. 
does not provide Free() method, always grows in size
object constructor is called automatically, object destructor is called when using StackAlloc()
*/
class LinearAllocator
{
public:
    using DataPointer = uint8_t*;
private:
    DataPointer base = nullptr;
    DataPointer top = nullptr;
    size_t size = 0;

    // align address upwards or left the same
    uintptr_t AlignAddress(uintptr_t address, size_t align)
    {
        const size_t mask = align - 1;
        MX_ASSERT((align & mask) == 0); // check for the power of 2
        return (address + align - 1) & ~mask;
    }

    uint8_t* AlignPointer(uint8_t* ptr, size_t align)
    {
        return reinterpret_cast<uint8_t*>(AlignAddress((uintptr_t)ptr, align));
    }

public:
    LinearAllocator() {}

    void Init(DataPointer data, size_t bytes)
    {
        this->base = data;
        this->top = data;
        this->size = bytes;
    }

    LinearAllocator(DataPointer data, size_t bytes)
        : base(data), top(data), size(bytes)
    {
    }

    [[nodiscard]] DataPointer RawAlloc(size_t bytes, size_t align = 1)
    {
        MX_ASSERT(this->base != nullptr);

        DataPointer aligned = AlignPointer(this->top, align);
        this->top = aligned + bytes;

        MX_ASSERT(this->top <= this->base + this->size);

        return aligned;
    }

    template<typename T, typename... Args>
    [[nodiscard]] T* Alloc(Args&&... args)
    {
        DataPointer ptr = RawAlloc(sizeof(T), alignof(T));
        return new (ptr) T(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    [[nodiscard]] auto StackAlloc(Args&&... args)
    {
        constexpr static auto deleter = [](T* ptr) { ptr->~T(); };
        using SmartPtr = std::unique_ptr<T, decltype(deleter)>;
        return SmartPtr(this->Alloc<T>(std::forward<Args>(args)...), deleter);
    }

    void Dump(std::ostream& out) const
    {
        for (size_t i = 0; i < this->size; i++)
        {
            out << std::hex << (int)this->base[i];
        }
        out << std::dec << "\n --- dumped " << size << " bytes --- \n";
    }
};