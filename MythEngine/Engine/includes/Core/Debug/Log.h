#pragma once

#include <string>
#include <atomic>
#include <sstream>

#include "imgui.h"

#include "define.h"

#pragma warning( disable : 4005 )
#define MYTH_CURRENT_FILE __FILE__

#ifdef MODE_EDITOR
#define MYTH_DEBUG(text, ...) Core::Debug::Log::Debug(MYTH_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define MYTH_INFO(text, ...) Core::Debug::Log::Info(MYTH_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define MYTH_WARNING(text, ...) Core::Debug::Log::Warning(MYTH_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define MYTH_ERROR(text, ...) Core::Debug::Log::Error(MYTH_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define MYTH_FATAL(text, ...) Core::Debug::Log::Fatal(MYTH_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#endif // MODE_EDITOR
#ifndef MODE_EDITOR
#define MYTH_DEBUG(text, ...) Core::Debug::Log::Debug(text, __VA_ARGS__)
#define MYTH_INFO(text, ...) Core::Debug::Log::Info(text, __VA_ARGS__) 
#define MYTH_WARNING(text, ...) Core::Debug::Log::Warning(text, __VA_ARGS__)
#define MYTH_ERROR(text, ...) Core::Debug::Log::Error(text, __VA_ARGS__)
#define MYTH_FATAL(text, ...) Core::Debug::Log::Fatal(text, __VA_ARGS__)
#endif // !MODE_EDITOR

namespace Core
{
	namespace Debug
	{
		enum class LOG_LEVEL : unsigned int
		{
			DEBUG = 0u,
			INFO,
			WARNING,
			ERROR,
			FATAL
		};

		class ILogger
		{
		public:
			virtual void Print(const std::string& p_str) = 0;
			virtual void Clear() = 0;
		};

		class Log
		{
		public:
			~Log() {}

			static Log& GetInstance() { return singleton; }

#ifdef MODE_EDITOR
			// safe for multithreading
			static void Debug(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
			// safe for multithreading
			static void Info(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
			// safe for multithreading
			static void Warning(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
			// safe for multithreading
			static void Error(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
			// safe for multithreading
			static void Fatal(const char* p_file, const unsigned p_line, const char* p_fmt, ...);

			void ShowEditorWindow(bool* p_opened = nullptr);
#endif // MODE_EDITOR
#ifndef MODE_EDITOR
			// safe for multithreading
			static void Debug(const char* p_fmt, ...);
			// safe for multithreading
			static void Info(const char* p_fmt, ...);
			// safe for multithreading
			static void Warning(const char* p_fmt, ...);
			// safe for multithreading
			static void Error(const char* p_fmt, ...);
			// safe for multithreading
			static void Fatal(const char* p_fmt, ...);
#endif // !MODE_EDITOR

			void Clear();

			void SetLogger(ILogger* p_logger = nullptr) { m_externalLogger = p_logger; }
			void SetShowLevel(const LOG_LEVEL level, bool p_show = true) { LOG_LEVEL_show[(unsigned int)level] = p_show; }

		private:
			static Log singleton;

			Log();

			void Print(const LOG_LEVEL level, const bool writeFL = false, const char* p_file = "", const unsigned p_line = 0u);

			std::stringstream m_cout;
			ILogger* m_externalLogger;
#ifdef MODE_EDITOR
			ImGuiTextBuffer m_textBuffer;
			bool m_scrollToBottom;
#endif // MODE_EDITOR
			std::atomic_flag m_writting = ATOMIC_FLAG_INIT;

			bool LOG_LEVEL_show[5] = { true, true, true, true, true };
		};
	}
}