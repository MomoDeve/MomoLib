#include "timeutils.h"
#include "big_integer.h"
#include "delegate.h"
#include "event.h"
#include "matrix.h"
#include "slab_allocator.h"
#include "meta.h"
#include "splay_tree.h"
#include "treap.h"
#include "utils.h"
#include "timeutils.h"

#include "MxEngineLib/EventDispatcher.h"
#include "MxEngineLib/LinearAllocator.h"
#include "MxEngineLib/StackAllocator.h"
#include "MxEngineLib/PoolAllocator.h"
#include "MxEngineLib/RandomAllocator.h"
#include "MxEngineLib/ChunkAllocator.h"
#include "MxEngineLib/DoublebufferAllocator.h"

#include <iostream>

int main()
{
    uint8_t mem1[128], mem2[64];
    
    MxEngine::DoublebufferAllocator<MxEngine::StackAllocator> alloc;
    alloc.Init(mem1, 128, mem2, 64);
    alloc.Current().RawAlloc(100);
    alloc.Swap();
    alloc.Current().RawAlloc(100); // assert is triggered here
}