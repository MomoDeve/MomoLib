
#include "MxEngineLib/EventDispatcher.h"

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