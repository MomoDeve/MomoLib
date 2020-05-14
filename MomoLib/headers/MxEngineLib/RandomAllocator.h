#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <memory>
#include <iomanip>

#define MX_ASSERT(x) assert(x)

/*
RandomAllocator class by #Momo
accepts chunk of memory and its size, do NOT allocate or free memory by itself
allocates objects of any type, each can be freed in any order
object constructor is called automatically, object destructor is called on Free()
*/
class RandomAllocator
{
public:
    using DataPointer = uint8_t*;
private:
    // align address upwards or left the same
    uintptr_t AlignAddress(uintptr_t address, size_t align)
    {
        const size_t mask = align - 1;
        MX_ASSERT((align & mask) == 0); // check for the power of 2
        return (address + align - 1) & ~mask;
    }

    // align 
    uintptr_t AlignAddressWithPadding(uintptr_t address, size_t align)
    {
        const size_t mask = align - 1;
        MX_ASSERT((align & mask) == 0); // check for the power of 2
        return (address + align) & ~mask;
    }

    uint8_t* AlignPointerWithPadding(uint8_t* ptr, size_t align)
    {
        return reinterpret_cast<uint8_t*>(AlignAddressWithPadding((uintptr_t)ptr, align));
    }

    uint8_t* AlignPointer(uint8_t* ptr, size_t align)
    {
        return reinterpret_cast<uint8_t*>(AlignAddress((uintptr_t)ptr, align));
    }

    struct Header
    {
        uintptr_t next;

        Header* GetNext()
        {
            return (Header*)(next & ~1);
        }

        size_t GetSize()
        {
            return (uint8_t*)GetNext() - (uint8_t*)this - sizeof(Header);
        }

        bool IsFree()
        {
            return !(next & 1);
        }

        void MakeFree()
        {
            next &= ~1;
        }

        void MakeBusy()
        {
            next |= 1;
        }

        uint8_t* GetData()
        {
            return (uint8_t*)this + sizeof(Header);
        }
    };

    Header* first = nullptr;
    Header* last = nullptr;

    void Split(Header* header, size_t bytes, size_t align)
    {
        if (header->GetSize() < bytes + 2 * sizeof(Header))
            return; // not enough space to create new block

        align = (align < sizeof(uintptr_t)) ? sizeof(uintptr_t) : align;

        Header* next = (Header*)AlignPointer(header->GetData() + bytes, align);
        next->next = (uintptr_t)header->GetNext();
        next->MakeFree();
        header->next = (uintptr_t)next;
    }

    bool Collapse(Header* header)
    {
        Header* next = header->GetNext();
        if (next != last && next->IsFree())
        {
            header->next = (uintptr_t)next->GetNext();
#if !defined(NDEBUG)
            next->next = 0;
#endif
            return true;
        }
        return false;
    }

public:
    RandomAllocator() {}

    void Init(DataPointer data, size_t bytes)
    {
        MX_ASSERT(bytes > sizeof(Header));
        first = (Header*)data;
        last = (Header*)(data + bytes);

        first->next = (uintptr_t)last;
        first->MakeFree();
    }

    RandomAllocator(DataPointer data, size_t bytes)
    {
        this->Init(data, bytes);
    }

    DataPointer GetBase()
    {
        return (DataPointer)first;
    }

    [[nodiscard]] DataPointer RawAlloc(size_t bytes, size_t align = 1)
    {
        MX_ASSERT(this->first != nullptr);

        Header* current = first;
        while (current != last)
        {
            if (current->IsFree())
            {
                while (this->Collapse(current));
                uint8_t* data = current->GetData();
                size_t available = current->GetSize();
                if (available >= bytes)
                {
                    this->Split(current, bytes, align);
                    current->MakeBusy();
                    return data;
                }
            }
            current = current->GetNext();
        }
        return nullptr;
    }

    void RawFree(uint8_t* ptr)
    {
        MX_ASSERT(ptr > (uint8_t*)first && ptr < (uint8_t*)last);
        Header* current = (Header*)(ptr - sizeof(Header));
        while (this->Collapse(current));
        current->MakeFree();
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
        auto deleter = [this](T* ptr) { this->Free(ptr); };
        using SmartPtr = std::unique_ptr<T, decltype(deleter)>;
        return SmartPtr(this->Alloc<T>(std::forward<Args>(args)...), std::move(deleter));
    }

    template<typename T>
    void Free(T* ptr)
    {
        ptr->~T();
        RawFree(reinterpret_cast<uint8_t*>(ptr));
    }

    void Dump(std::ostream& out) const
    {
        uint8_t* base = (uint8_t*)first;
        size_t size = (uint8_t*)last - base;
        for (size_t i = 0; i < size; i++)
        {
            if (i % sizeof(uintptr_t) == 0) out << ' ';
            out << std::setfill('0') << std::setw(2);
            out << std::hex << (int)base[i];
        }
        out << std::dec << "\n --- dumped " << size << " bytes --- \n";
    }
};