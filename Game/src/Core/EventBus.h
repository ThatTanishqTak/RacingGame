#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <algorithm>

class EventBus
{
public:
    template<typename Event>
    using Handler = std::function<void(const Event&)>;

    template<typename Event>
    int Subscribe(const Handler<Event>& handler)
    {
        std::lock_guard<std::mutex> l_Lock(m_Mutex);
        int l_Token = m_NextToken++;
        auto& a_Handlers = m_Subscribers[typeid(Event)];
        a_Handlers.emplace_back(l_Token, [handler](const void* e) { handler(*static_cast<const Event*>(e)); });
        m_TokenTypes.emplace(l_Token, typeid(Event));

        return l_Token;
    }

    template<typename Event>
    void Publish(const Event& event)
    {
        std::vector<BaseHandler> l_HandlersCopy;
        {
            std::lock_guard<std::mutex> l_Lock(m_Mutex);
            auto a_It = m_Subscribers.find(typeid(Event));
            if (a_It == m_Subscribers.end())
            {
                return;
            }

            for (auto& it_Pair : a_It->second)
            {
                l_HandlersCopy.push_back(it_Pair.second);
            }
        }

        for (auto& it_Handler : l_HandlersCopy)
        {
            it_Handler(&event);
        }
    }

    void Unsubscribe(int token)
    {
        std::lock_guard<std::mutex> l_Lock(m_Mutex);
        auto a_TypeIt = m_TokenTypes.find(token);
        if (a_TypeIt == m_TokenTypes.end())
        {
            return;
        }

        auto a_SubIt = m_Subscribers.find(a_TypeIt->second);
        if (a_SubIt != m_Subscribers.end())
        {
            auto& a_Handlers = a_SubIt->second;
            a_Handlers.erase(std::remove_if(a_Handlers.begin(), a_Handlers.end(), [token](const auto& it_Pair) { return it_Pair.first == token; }), a_Handlers.end());
        }

        m_TokenTypes.erase(a_TypeIt);
    }

private:
    using BaseHandler = std::function<void(const void*)>;
    std::unordered_map<std::type_index, std::vector<std::pair<int, BaseHandler>>> m_Subscribers;
    std::unordered_map<int, std::type_index> m_TokenTypes;
    int m_NextToken = 0;
    std::mutex m_Mutex;
};

extern EventBus g_EventBus;