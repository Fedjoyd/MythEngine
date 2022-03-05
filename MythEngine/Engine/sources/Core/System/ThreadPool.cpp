#include "Core/System/ThreadPool.h"

#include "Core/Debug/Log.h"
#define MYTH_CURRENT_FILE "ThreadPool.cpp"

void Infinite_loop_function(Core::System::ThreadInfo* current_thread, Core::System::ThreadPool* m_manager)
{
	MYTH_DEBUG("thread %d has been started", current_thread->GetID());

	while (current_thread->GetStatus() != Core::System::THREAD_STATUS::SHUTDOWN)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (m_manager->HasNoJob())
			continue;

		std::function<void()> JobToDo;
		if (!m_manager->TryGetJob(&JobToDo))
			continue;

		current_thread->SetStatus(Core::System::THREAD_STATUS::WORKING);
		MYTH_DEBUG("thread %d started work", current_thread->GetID());

		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		JobToDo();

		std::chrono::duration<float> durationMilli = (std::chrono::high_resolution_clock::now() - start);

		current_thread->SetStatus(Core::System::THREAD_STATUS::WAITING);
		MYTH_DEBUG("thread %d work take %0.3f ms", current_thread->GetID(), durationMilli.count());
	}

	MYTH_DEBUG("thread %d is shutdown", current_thread->GetID());
}

void Core::System::ThreadInfo::StartThread(const unsigned int ID, ThreadPool* m_manager)
{
	m_ID.store(ID);
	m_thread = std::thread(Infinite_loop_function, this, m_manager);
}

void Core::System::ThreadInfo::Shutdown()
{
	m_status.store(THREAD_STATUS::SHUTDOWN);
	m_thread.join();
}

// threadpool
// --------------------------------------------------------------

Core::System::ThreadPool::ThreadPool()
{
	m_threadList = std::vector<ThreadInfo>(std::thread::hardware_concurrency());
}

void Core::System::ThreadPool::Initialise()
{
	for (unsigned int i = 0; i < m_threadList.size(); i++)
		m_threadList[i].StartThread(i, this);
}

void Core::System::ThreadPool::AddJob(std::function<void()> New_Job)
{
	while (jobListLock.test_and_set())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	m_jobQueue.push_back(New_Job);

	jobListLock.clear();
}

bool Core::System::ThreadPool::HasNoJob()
{
	while (jobListLock.test_and_set())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	bool toReturn = m_jobQueue.empty();

	jobListLock.clear();

	return toReturn;
}

bool Core::System::ThreadPool::TryGetJob(std::function<void()>* out_Job)
{
	while (jobListLock.test_and_set())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	if (m_jobQueue.empty() || out_Job == nullptr)
	{
		jobListLock.clear();

		return false;
	}

	*out_Job = m_jobQueue.front();
	m_jobQueue.pop_front();

	jobListLock.clear();

	return true;
}

void Core::System::ThreadPool::Shutdown()
{
//	for (auto& thr : m_threadList)
//		thr.Shutdown();

	m_threadList.clear();
	stopped = true;
}
