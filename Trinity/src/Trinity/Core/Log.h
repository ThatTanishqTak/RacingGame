#pragma once

#include "Trinity/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Trinity
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define TR_CORE_TRACE(...)    ::Trinity::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TR_CORE_INFO(...)     ::Trinity::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TR_CORE_WARN(...)     ::Trinity::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TR_CORE_ERROR(...)    ::Trinity::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TR_CORE_CRITICAL(...) ::Trinity::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define TR_TRACE(...)         ::Trinity::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TR_INFO(...)          ::Trinity::Log::GetClientLogger()->info(__VA_ARGS__)
#define TR_WARN(...)          ::Trinity::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TR_ERROR(...)         ::Trinity::Log::GetClientLogger()->error(__VA_ARGS__)
#define TR_CRITICAL(...)      ::Trinity::Log::GetClientLogger()->critical(__VA_ARGS__)