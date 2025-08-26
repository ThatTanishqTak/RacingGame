#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

class EventBus
{
public:
    template<typename Event>
    using Handler = std::function<void(const Event&)>;

    template<typename Event>
    void Subscribe(const Handler<Event>& handler)
    {
        auto& a_Handlers = Subscribers[typeid(Event)];
        a_Handlers.push_back([handler](const void* e) { handler(*static_cast<const Event*>(e)); });
    }

    template<typename Event>
    void Publish(const Event& event)
    {
        auto it = Subscribers.find(typeid(Event));
        if (it == Subscribers.end())
        {
            return;
        }

        for (auto& it_Handler : it->second)
        {
            it_Handler(&event);
        }
    }

private:
    using BaseHandler = std::function<void(const void*)>;
    std::unordered_map<std::type_index, std::vector<BaseHandler>> Subscribers;
};

extern EventBus g_EventBus;