#pragma once

#include "define.h"

#include <vector>
#include <deque>
#include <atomic>
#include <thread>
#include <functional>

namespace Core
{
	namespace System
	{
		enum class THREAD_STATUS
		{
			WAITING,
			WORKING,
			SHUTDOWN
		};

		class ThreadPool;

		class ThreadInfo
		{
		public:
			ThreadInfo() : m_ID(0u), m_status(THREAD_STATUS::WAITING) {}
			~ThreadInfo() { if (m_status != THREAD_STATUS::SHUTDOWN) { Shutdown(); } }

			unsigned int  GetID() const { return m_ID.load(); }
			THREAD_STATUS GetStatus() const { return m_status.load(); }
			void SetStatus(const THREAD_STATUS new_status) { if(m_status != THREAD_STATUS::SHUTDOWN) m_status.store(new_status); }

			void StartThread(const unsigned int ID, ThreadPool* m_manager);
			void Shutdown();

		private:
			std::atomic<unsigned int>  m_ID;
			std::atomic<THREAD_STATUS> m_status;

			std::thread m_thread;
		};

		class ThreadPool
		{
		public:
			ThreadPool();
			~ThreadPool() { if (!stopped) Shutdown(); }

			void Initialise();
			void Shutdown();

			void AddJob(std::function<void()> New_Job);
			bool HasNoJob();
			bool TryGetJob(std::function<void()>* out_Job);

		private:
			std::vector<ThreadInfo> m_threadList;

			std::atomic<bool> stopped = false;

			std::deque<std::function<void()>> m_jobQueue;

			std::atomic_flag jobListLock = ATOMIC_FLAG_INIT;
		};
	}
}