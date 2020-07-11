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

MAKE_EVENT_BASE(BaseEvent);
using EventDispatcher = MxEngine::EventDispatcherImpl<BaseEvent>;

class SomeEvent : public BaseEvent
{
public:
    MAKE_EVENT(SomeEvent);
};

int main()
{
    EventDispatcher dispatcher;
    dispatcher.AddEventListener("x", [](SomeEvent& e) { std::cout << "SomeEvent x\n"; });
    dispatcher.AddEventListener("y", [](SomeEvent& e) { std::cout << "SomeEvent y\n"; });

    SomeEvent e;
    dispatcher.Invoke(e);
    dispatcher.RemoveEventListener("x");
    dispatcher.Invoke(e);
}