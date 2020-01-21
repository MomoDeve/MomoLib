#define MAKE_EVENT \
template<typename T> friend class EventDispatcher;\
public: virtual size_t GetEventType() const override { return this->eventType; } private:\
inline static size_t eventType

template<typename EventBase>
class EventDispatcher
{
	using CallbackList = std::vector<std::function<void(EventBase&)>>;
	using EventList = std::vector<std::unique_ptr<EventBase>>;
	CallbackList callbacks;
	EventList events;

	size_t eventTypeId = 0;

	template<typename EventType>
	void RegisterEventType()
	{
		static size_t typeId = ++this->eventTypeId;
		EventType::eventType = typeId;
	}
public:

	template<typename EventType>
	void AddEvent(std::function<void(EventType&)> func)
	{
		RegisterEventType<EventType>();
		this->callbacks.push_back([func = std::move(func)](EventBase& e)
		{
			if (e.GetEventType() == EventType::eventType)
				func((EventType&)e);
		});
	}

	template<typename EventType>
	void AddEvent(void(*func)(EventType&))
	{
		this->AddEvent(std::function<void(EventType&)>(func));
	}

	void Invoke(EventBase& event)
	{
		for (const auto& callback : this->callbacks)
		{
			callback(event);
		}
	}

	void Shedule(std::unique_ptr<EventBase> event)
	{
		this->events.push_back(std::move(event));
	}

	void InvokeAll()
	{
		for (const auto& event : this->events)
		{
			this->Invoke(*event);
		}
		this->events.clear();
	}
};
