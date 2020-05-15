// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistributionand use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditionsand the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditionsand the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <memory>

#define MX_ASSERT(x) assert(x)

namespace MxEngine
{
    /*
    StackAllocator class by #Momo
    accepts chunk of memory and its size, do NOT allocate or free memory by itself
    allocates aligned chunk of memory, adjusting `top` pointer.
    last allocated object can be freed by Free() method (LIFO idiom)
    object constructor is called automatically, object destructor is called when using StackAlloc()
    */
    class StackAllocator
    {
    public:
        using DataPointer = uint8_t*;
    private:
        /*!
        begin of memory chunk in use
        */
        DataPointer base = nullptr;
        /*!
        end of currently used memory chunk (filled with allocated data)
        */
        DataPointer top = nullptr;
        /*!
        total size (in bytes) of memory chunk
        */
        size_t size = 0;

        /*!
        aligns address upwards (if align = 4, 4->8, 7->8, 9->12 etc)
        \param address address to align
        \param align required alignment
        \returns aligned address
        */
        uintptr_t AlignAddress(uintptr_t address, size_t align)
        {
            const size_t mask = align - 1;
            MX_ASSERT((align & mask) == 0); // check for the power of 2
            return (address + align) & ~mask;
        }

        /*!
        aligns pointer upwards (if align = 4, 4->8, 7->8, 9->12 etc)
        \param pointer to align
        \param align required alignment
        \returns aligned pointer
        */
        uint8_t* AlignPointer(uint8_t* ptr, size_t align)
        {
            return reinterpret_cast<uint8_t*>(AlignAddress((uintptr_t)ptr, align));
        }

    public:
        /*!
        creates empty stack allocator (nullptr chunk pointer, size = 0)
        */
        StackAllocator() {}

        /*!
        assigns new memory chunk to stack allocator
        \param data begin of memory chunk
        \param bytes size in bytes of memory chunk
        */
        void Init(DataPointer data, size_t bytes)
        {
            this->base = data;
            this->top = data;
            this->size = bytes;
        }

        /*!
        constructs stack allocator, assigning it memory chunk
        \param data begin of memory chunk
        \param bytes size in bytes of memory chunk
        */
        StackAllocator(DataPointer data, size_t bytes)
            : base(data), top(data), size(bytes)
        {
        }

        /*!
        memory chunk getter
        \returns pointer to begin of memory chunk
        */
        DataPointer GetBase()
        {
            return this->base;
        }

        /*!
        returns pointer to raw allocated memory
        \param bytes minimal requested block size
        \param align minimal alignment of pointer (defaults to 1)
        \returns pointer to memory
        */
        [[nodiscard]] DataPointer RawAlloc(size_t bytes, size_t align = 1)
        {
            MX_ASSERT(this->base != nullptr);

            DataPointer aligned = AlignPointer(this->top, align);
            aligned[-1] = (uint8_t)(aligned - this->top);
            this->top = aligned + bytes;

            MX_ASSERT(this->top <= this->base + this->size);

            return aligned;
        }

        /*!
        frees raw allocated memory by pointer to its begin
        \param ptr pointer to deallocate
        \warning ptr MUST be a pointer to last allocated chunk,
        calling RawFree on some other chunk will probably result in crash
        */
        void RawFree(uint8_t* ptr)
        {
            MX_ASSERT(ptr >= this->base && ptr < this->base + this->size);
            size_t shift = ptr[-1];
            this->top = ptr - shift;
        }

        /*!
        constructs object of type T in memory and returns pointer to it
        \param args arguments for object construction
        \returns pointer to unmanaged object
        */
        template<typename T, typename... Args>
        [[nodiscard]] T* Alloc(Args&&... args)
        {
            DataPointer ptr = RawAlloc(sizeof(T), alignof(T));
            return new (ptr) T(std::forward<Args>(args)...);
        }

        /*!
        constructs object of type T
        \param args arguments for object construction
        \returns smart pointer to object with custom deleter
        \warning you should NOT change order of destruction of allocated objects (by moving them, for example) in any way,
        as it will probably results in crash
        */
        template<typename T, typename... Args>
        [[nodiscard]] auto StackAlloc(Args&&... args)
        {
            auto deleter = [this](T* ptr) { this->Free(ptr); };
            using SmartPtr = std::unique_ptr<T, decltype(deleter)>;
            return SmartPtr(this->Alloc<T>(std::forward<Args>(args)...), std::move(deleter));
        }

        /*!
        destroyes object of type T, calling its destructor
        \param value object to destroy
        \warning  ptr MUST be a pointer to last allocated object,
        calling Free on some other object will probably result in crash
        */
        template<typename T>
        void Free(T* ptr)
        {
            ptr->~T();
            RawFree(reinterpret_cast<uint8_t*>(ptr));
        }

        /*!
        prints whole memory chunk byte by byte in hex format into stream provided
        \param out stream to output to
        */
        void Dump(std::ostream& out)
        {
            for (size_t i = 0; i < this->size; i++)
            {
                out << std::hex << (int)this->base[i];
            }
            out << std::dec << "\n --- dumped " << size << " bytes --- \n";
        }
    };
}