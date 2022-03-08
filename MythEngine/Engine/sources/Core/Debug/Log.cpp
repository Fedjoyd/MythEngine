#include "Core/Debug/Log.h"

#include <vector> 

Core::Debug::Log Core::Debug::Log::singleton;

ImGuiTextBuffer tempTextBuffer = ImGuiTextBuffer();

const char* LOG_LEVEL_char[] = {
	" DEBUG ",
	" INFO  ",
	"WARNING",
	" ERROR ",
	" FATAL ",
};

void Core::Debug::Log::Print(const LOG_LEVEL level, const bool writeFL, const char* p_file, const unsigned p_line)
{
	if (!LOG_LEVEL_show[(unsigned int)level])
	{
		m_cout.str("");
		return;
	}

	std::string fmt = "[" + std::string(LOG_LEVEL_char[(unsigned int)level]) + "]" + (writeFL ? "[" + std::string(p_file) + "][line " + std::to_string(p_line) + "]" : "") + " : " + m_cout.str() + "\n";
	m_cout.str("");

	if (singleton.m_externalLogger != nullptr)
		singleton.m_externalLogger->Print(fmt);

#ifdef MODE_EDITOR
	singleton.m_textBuffer.append(fmt.c_str());
	singleton.m_scrollToBottom = true;
#endif
}

#ifdef MODE_EDITOR
void Core::Debug::Log::ShowEditorWindow(bool* p_opened)
{
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

	ImGui::SetNextWindowSize(ImVec2(500, 400));
	ImGui::Begin("Console", p_opened);

	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	if (ImGui::Button(("Debug (" + std::string(LOG_LEVEL_show[0u] ? "Y" : "N") + ")").c_str()))
		LOG_LEVEL_show[0u] = !LOG_LEVEL_show[0u];
	ImGui::SameLine();
	if (ImGui::Button(("Info (" + std::string(LOG_LEVEL_show[1u] ? "Y" : "N") + ")").c_str()))
		LOG_LEVEL_show[1u] = !LOG_LEVEL_show[1u];
	ImGui::SameLine();
	if (ImGui::Button(("Warning (" + std::string(LOG_LEVEL_show[2u] ? "Y" : "N") + ")").c_str()))
		LOG_LEVEL_show[2u] = !LOG_LEVEL_show[2u];
	ImGui::SameLine();
	if (ImGui::Button(("Error (" + std::string(LOG_LEVEL_show[3u] ? "Y" : "N") + ")").c_str()))
		LOG_LEVEL_show[3u] = !LOG_LEVEL_show[3u];
	ImGui::SameLine();
	if (ImGui::Button(("Fatal (" + std::string(LOG_LEVEL_show[4u] ? "Y" : "N") + ")").c_str()))
		LOG_LEVEL_show[4u] = !LOG_LEVEL_show[4u];

	ImGui::Separator();

	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	ImGui::TextUnformatted(m_textBuffer.begin());
	if (m_scrollToBottom)
		ImGui::SetScrollHereY(1.f);
	m_scrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}

void Core::Debug::Log::Debug(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::DEBUG, true, p_file, p_line);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Info(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::INFO, true, p_file, p_line);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Warning(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::WARNING, true, p_file, p_line);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Error(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::ERROR, true, p_file, p_line);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Fatal(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::FATAL, true, p_file, p_line);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}
#endif // MODE_EDITOR
#ifndef MODE_EDITOR
void Core::Debug::Log::Debug(const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::DEBUG);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Info(const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::INFO);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Warning(const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::WARNING);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Error(const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::ERROR);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}

void Core::Debug::Log::Fatal(const char* p_fmt, ...)
{
	while (singleton.m_writting.test_and_set()) {}

	va_list args;
	va_start(args, p_fmt);
	tempTextBuffer.appendfv(p_fmt, args);
	va_end(args);

	singleton.m_cout << tempTextBuffer.c_str();
	singleton.Print(LOG_LEVEL::FATAL);

	tempTextBuffer.clear();

	singleton.m_writting.clear();
}
#endif // !MODE_EDITOR


void Core::Debug::Log::Clear()
{
#ifdef MODE_EDITOR
	m_textBuffer.clear();
#endif
	if (singleton.m_externalLogger != nullptr)
		singleton.m_externalLogger->Clear();
}

Core::Debug::Log::Log() :
#ifdef MODE_EDITOR
	m_scrollToBottom(true),
	m_textBuffer(),
#endif
	m_externalLogger(nullptr),
	m_cout()
{
}
