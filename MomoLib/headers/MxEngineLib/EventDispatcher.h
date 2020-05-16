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

#include <memory>
#include <functional>
#include <unordered_set>
#include <typeindex>
#include <map>
#include <algorithm>
#include <cstdint>

namespace MxEngine
{
	#define MX_ASSERT(x) assert(x)
	#define MAKE_SCOPE_PROFILER(x) // stub, as no profiler available

	/*!
	hash table for crc32 algorithm
	*/
	static constexpr uint32_t crc_table[256] =
	{
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
		0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
		0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
		0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
		0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
		0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
		0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
		0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
		0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
		0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
		0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
		0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
		0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
		0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
		0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
		0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
		0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
		0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
		0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
		0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
		0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
		0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};

	using StringId = uint32_t;

	/*!
	computes hash for a string at compile time
	\param str string from which hash is computed
	\param size size of str, if str is literal, sizeof(str) can be taken
	\param idx current index in string, defaults to 0
	\param prev_crc part of already computed hash, defaults to 0xFFFFFFFF
	\returns hash of the string
	*/
	constexpr StringId crc32(const char* str, size_t size, size_t idx = 0, uint32_t prev_crc = 0xFFFFFFFF)
	{
		if (size == idx)
			return prev_crc ^ 0xFFFFFFFF;
		else
			return crc32(str, size, idx + 1, (prev_crc >> 8) ^ crc_table[(prev_crc ^ str[idx]) & 0xFF]);
	}

	// This doesn't take into account the null char
	// computed hash of string literal at compile time
	#define STRING_ID(x) (crc32(x, sizeof(x)))

	/*!
	transforms string literal into hash by appending _id suffix ("str"_id)
	\param s string to transform
	\patam size sizeof string
	*/
	constexpr StringId operator"" _id(char const* s, size_t size)
	{
		return crc32(s, size);
	}

	template<typename T>
	using UniqueRef = std::unique_ptr<T>;

	/*
	creates base event class and adds GetEventType() pure virtual method. Used for EventDispatcher class
	*/
	#define MAKE_EVENT_BASE(name) struct name { inline virtual uint32_t GetEventType() const = 0; virtual ~name() = default; }

	/*
	inserted into class body of derived classes from base event. Using compile-time hash from class name to generate type id
	*/
	#define MAKE_EVENT(class_name) \
	template<typename T> friend class EventDispatcher;\
	public: inline virtual uint32_t GetEventType() const override { return eventType; } private:\
	constexpr static uint32_t eventType = STRING_ID(#class_name)

	/*!
	EventDispatcher class is used to handle all events inside MxEngine. Events can either be dispatch for Application (global) or
	for currently active scene. Note that events are NOT dispatched when developer console is opened and instead sheduled until it close
	*/
	template<typename EventBase>
	class EventDispatcher
	{
		using CallbackBaseFunction = std::function<void(EventBase&)>;
		using NamedCallback = std::pair<std::string, CallbackBaseFunction>;
		using CallbackList = std::vector<NamedCallback>;
		using EventList = std::vector<UniqueRef<EventBase>>;
		using EventTypeIndex = uint32_t;

		/*!
		list of sheduled all events (executed once per frame by Application class)
		*/
		EventList events;
		/*!
		maps event id to list of event listeners of that id
		*/
		std::unordered_map<EventTypeIndex, CallbackList> callbacks;
		/*!
		shedules listeners which will be added next frame.
		This cache exists because sometimes user wants to add new listener inside other listener callback, which may result in crash.
		*/
		std::unordered_map<EventTypeIndex, CallbackList> toAddCache;
		/*!
		shedules listeners which will be removed next frame.
		This cache exists because sometimes user wants to remove event listener inside other listener callback (or even perform self-removal), which may result in crash.
		*/
		std::vector<std::string> toRemoveCache;

		/*!
		immediately invokes all listeners of event, if any exists
		\param event event to dispatch
		*/
		inline void ProcessEvent(EventBase& event)
		{
			auto& eventCallbacks = this->callbacks[event.GetEventType()];
			for (const auto& [name, callback] : eventCallbacks)
			{
				callback(event);
			}
		}

		/*!
		adds new listener callback to cache queue
		\param name callback name (used for removal)
		\param func callback functor
		*/
		template<typename EventType>
		inline void AddCallbackImpl(std::string name, CallbackBaseFunction&& func)
		{
			this->toAddCache[EventType::eventType].emplace_back(std::move(name), std::move(func));
		}

		/*!
		immediately removes all listeners with same name from list
		\param callbacks list of listeners callbacks
		\param name name of listeners to search for
		*/
		inline void RemoveEventByName(CallbackList& callbacks, const std::string& name)
		{
			auto it = std::remove_if(callbacks.begin(), callbacks.end(), [&name](const auto& p)
				{
					return p.first == name;
				});
			callbacks.resize(it - callbacks.begin());
		}

		/*!
		performs cache update, removing events from toRemoveCache list and adding events from toAddCache list
		*/
		inline void RemoveQueuedEvents()
		{
			for (auto it = this->toRemoveCache.begin(); it != this->toRemoveCache.end(); it++)
			{
				for (auto& [event, callbacks] : this->callbacks)
				{
					RemoveEventByName(callbacks, *it);
				}
			}
			this->toRemoveCache.clear();

			for (auto it = this->toAddCache.begin(); it != this->toAddCache.end(); it++)
			{
				auto& [event, funcs] = *it;
				for (auto& func : funcs)
				{
					callbacks[event].push_back(std::move(func));
				}
			}
			for (auto& list : this->toAddCache) list.second.clear();
		}

	public:
		/*!
		adds new event listener to dispatcher (listener placed in waiting queue until next frame).
		Note that multiple listeners may have same name. If so, deleting by name will result in removing all of them
		\param name name of listener (used for deleting listener)
		\param func listener callback functor
		*/
		template<typename EventType>
		void AddEventListener(const std::string& name, std::function<void(EventType&)> func)
		{
			this->AddCallbackImpl<EventType>(name, [func = std::move(func)](EventBase& e)
			{
				if (e.GetEventType() == EventType::eventType)
					func((EventType&)e);
			});
		}

		/*!
		adds new event listener to dispatcher (listener is placed in waiting queue until next frame).
		Note that multiple listeners may have same name. If so, deleting by name will result in removing all of them
		\param name name of listener (used for deleting listener)
		\param func listener callback functor
		*/
		template<typename FunctionType>
		void AddEventListener(const std::string& name, FunctionType&& func)
		{
			this->AddEventListener(name, std::function(std::forward<FunctionType>(func)));
		}

		/*!
		removes all event listeners by their names (action is placed in waiting queue until next frame)
		\param name name of listeners to be deleted
		*/
		void RemoveEventListener(const std::string& name)
		{
			this->toRemoveCache.push_back(name);
		}

		/*!
		Immediately invokes event of specific type. Note that invokation also forces queues to be invalidated
		\param event event to dispatch
		*/
		template<typename Event>
		void Invoke(Event& event)
		{
			this->RemoveQueuedEvents();
			auto& eventCallbacks = this->callbacks[event.GetEventType()];
			for (const auto& [name, callback] : eventCallbacks)
			{
				callback(event);
			}
		}

		/*!
		Adds event to event queue. All such events will be dispatched in next frames in the order they were added
		\param event event to shedule dispatch
		*/
		void AddEvent(UniqueRef<EventBase> event)
		{
			this->events.push_back(std::move(event));
		}

		/*!
		Invokes all shedules events in the order they were added. Note that invokation also forces queues to be invalidated
		*/
		void InvokeAll()
		{
			this->RemoveQueuedEvents();

			for (size_t i = 0; i < this->events.size(); i++)
			{
				MAKE_SCOPE_PROFILER(typeid(*this->events[i]).name());
				this->ProcessEvent(*this->events[i]);
			}
			this->events.clear();
		}
	};
}