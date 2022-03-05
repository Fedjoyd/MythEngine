#include "..\..\includes\ECS\ScriptInstance.h"

#include "Core/Debug/Log.h"

#include "tinyJS/TinyJS_Functions.h"
#include "imgui_stdlib.h"

ECS::ScriptInstance::ScriptInstance() :
	Component(),
	m_script(nullptr)
{
}

Ressources::ScriptVariable* ECS::ScriptInstance::GetValue(const std::string& VariableName)
{
	for (unsigned int i = 0; i < m_instanceValue.size(); i++)
		if (VariableName == m_instanceValue[i].Name)
			return &m_instanceValue[i];

	return nullptr;
}

const Ressources::ScriptVariable* ECS::ScriptInstance::GetValue(const std::string& VariableName) const
{
	for (unsigned int i = 0; i < m_instanceValue.size(); i++)
		if (VariableName == m_instanceValue[i].Name)
			return &m_instanceValue[i];

	return nullptr;
}

int ECS::ScriptInstance::GetintValue(const std::string& VariableName) const
{
	const Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return 0;
	if (Variable->Type != Ressources::TYPE_VARIABLE::INT)
		return 0;

	return Variable->GetIntValue();
}

double ECS::ScriptInstance::GetDoubleValue(const std::string& VariableName) const
{
	const Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return 0.0;
	if (Variable->Type != Ressources::TYPE_VARIABLE::DOUBLE)
		return 0.0;

	return Variable->GetDoubleValue();
}

std::string ECS::ScriptInstance::GetStringValue(const std::string& VariableName) const
{
	const Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return "";
	if (Variable->Type != Ressources::TYPE_VARIABLE::STRING)
		return "";

	return Variable->GetStringValue();
}

bool ECS::ScriptInstance::SetintValue(const std::string& VariableName, const int p_value)
{
	Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return false;
	if (Variable->Type != Ressources::TYPE_VARIABLE::INT)
		return false;

	Variable->GetIntValue() = p_value;
	return true;
}

bool ECS::ScriptInstance::SetDoubleValue(const std::string& VariableName, const double p_value)
{
	Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return false;
	if (Variable->Type != Ressources::TYPE_VARIABLE::DOUBLE)
		return false;

	Variable->GetDoubleValue() = p_value;
	return true;
}

bool ECS::ScriptInstance::SetStringValue(const std::string& VariableName, const std::string& p_value)
{
	Ressources::ScriptVariable* Variable = GetValue(VariableName);

	if (Variable == nullptr)
		return false;
	if (Variable->Type != Ressources::TYPE_VARIABLE::STRING)
		return false;

	Variable->GetStringValue() = p_value;
	return true;
}

// loop function

void ECS::ScriptInstance::Start(Core::GameObject* p_gameObject)
{
	if (m_script != nullptr)
	{
		if (!m_script->IsLoaded())
			return;

		DuplicateVariableFromSource();

		if (m_script->HasStartFunction())
		{
			CTinyJS* JScontext = new CTinyJS();
			InitialiseJavascriptContext(JScontext);

			try {
				JScontext->execute(m_script->GetSource());
				InjectValueInJS(JScontext);
				JScontext->execute("Start();");
				ExtractValueFromJS(JScontext);
			}
			catch (CScriptException* e) { MYTH_ERROR(e->text.c_str()); }

			delete JScontext;
		}
	}
}

void ECS::ScriptInstance::Update(Core::GameObject* p_gameObject)
{
	if (m_script != nullptr)
	{
		if (!m_script->IsLoaded() || !m_script->HasUpdateFunction())
			return;

		DuplicateVariableFromSource();

		CTinyJS* JScontext = new CTinyJS();
		InitialiseJavascriptContext(JScontext);

		try {
			JScontext->execute(m_script->GetSource());
			InjectValueInJS(JScontext);
			JScontext->execute("Update();");
			ExtractValueFromJS(JScontext);
		}
		catch (CScriptException* e) { /*Core::Debug::Log::Error(e->text.c_str());/**/ }

		delete JScontext;
	}
}

void ECS::ScriptInstance::FixedUpdate(Core::GameObject* p_gameObject)
{
	if (m_script != nullptr)
	{
		if (!m_script->IsLoaded() || !m_script->HasFixedUpdateFunction())
			return;

		DuplicateVariableFromSource();

		CTinyJS* JScontext = new CTinyJS();
		InitialiseJavascriptContext(JScontext);

		try {
			JScontext->execute(m_script->GetSource());
			InjectValueInJS(JScontext);
			JScontext->execute("FixedUpdate();");
			ExtractValueFromJS(JScontext);
		}
		catch (CScriptException* e) { /*Core::Debug::Log::Error(e->text.c_str());/**/ }

		delete JScontext;
	}
}

#ifdef MODE_EDITOR
void ECS::ScriptInstance::EditorUpdate(Core::GameObject* p_gameObject)
{
	if (m_script != nullptr)
	{
		if (!m_script->IsLoaded() || !m_script->HasEditorUpdateFunction())
			return;

		DuplicateVariableFromSource();

		CTinyJS* JScontext = new CTinyJS();
		InitialiseJavascriptContext(JScontext);

		try {
			JScontext->execute(m_script->GetSource());
			InjectValueInJS(JScontext);
			JScontext->execute("EditorUpdate();");
			ExtractValueFromJS(JScontext);
		}
		catch (CScriptException* e) { /*Core::Debug::Log::Error(e->text.c_str());/**/ }

		delete JScontext;
	}
}

void ECS::ScriptInstance::ShowEditorControl(const unsigned int p_indexComponent)
{
	if (m_script != nullptr)
		if (m_script->IsLoaded())
			for (unsigned int i = 0; i < m_instanceValue.size(); i++)
			{
				switch (m_instanceValue[i].Type)
				{
				case Ressources::TYPE_VARIABLE::INT:
					if (ImGui::Button(("Reset##" + m_instanceValue[i].Name + "_intScriptSource").c_str()))
						m_instanceValue[i].GetIntValue() = m_script->GetVariableDefault()[i].GetIntValue();
					ImGui::SameLine();
					ImGui::InputInt((m_instanceValue[i].Name + "##intScriptSource").c_str(), m_instanceValue[i].GetIntPtr());
					break;

				case Ressources::TYPE_VARIABLE::DOUBLE:
					if (ImGui::Button(("Reset##" + m_instanceValue[i].Name + "_doubleScriptSource").c_str()))
						m_instanceValue[i].GetDoubleValue() = m_script->GetVariableDefault()[i].GetDoubleValue();
					ImGui::SameLine();
					ImGui::InputDouble((m_instanceValue[i].Name + "##doubleScriptSource").c_str(), m_instanceValue[i].GetDoublePtr());
					break;

				case Ressources::TYPE_VARIABLE::STRING:
					if (ImGui::Button(("Reset##" + m_instanceValue[i].Name + "_stringScriptSource").c_str()))
						m_instanceValue[i].GetStringValue() = m_script->GetVariableDefault()[i].GetStringValue();
					ImGui::SameLine();
					ImGui::InputText((m_instanceValue[i].Name + "##stringScriptSource").c_str(), m_instanceValue[i].GetStringPtr());
					break;
				}
			}
}
#endif // MODE_EDITOR

void ECS::ScriptInstance::Draw(Core::GameObject* p_gameObject)
{
	if (m_script != nullptr)
	{
		if (!m_script->IsLoaded() || !m_script->HasDrawFunction())
			return;

		DuplicateVariableFromSource();

		CTinyJS* JScontext = new CTinyJS();
		InitialiseJavascriptContext(JScontext);

		try {
			JScontext->execute(m_script->GetSource());
			InjectValueInJS(JScontext);
			JScontext->execute("Draw();");
			ExtractValueFromJS(JScontext);
		}
		catch (CScriptException* e) { /*Core::Debug::Log::Error(e->text.c_str());/**/ }

		delete JScontext;
	}
}

// context transfert function

void js_LogDebug(CScriptVar* v, void* userdata) { MYTH_DEBUG(v->getParameter("text")->getString().c_str()); }
void js_LogInfo(CScriptVar* v, void* userdata) { MYTH_INFO(v->getParameter("text")->getString().c_str()); }
void js_LogWarning(CScriptVar* v, void* userdata) { MYTH_WARNING(v->getParameter("text")->getString().c_str()); }
void js_LogError(CScriptVar* v, void* userdata) { MYTH_ERROR(v->getParameter("text")->getString().c_str()); }
void js_LogFatal(CScriptVar* v, void* userdata) { MYTH_FATAL(v->getParameter("text")->getString().c_str()); }

void ECS::ScriptInstance::InitialiseJavascriptContext(CTinyJS* context)
{
	registerFunctions(context);
	context->addNative("function LogDebug(text)", &js_LogDebug, nullptr);
	context->addNative("function LogInfo(text)", &js_LogInfo, nullptr);
	context->addNative("function LogWarning(text)", &js_LogWarning, nullptr);
	context->addNative("function LogError(text)", &js_LogError, nullptr);
	context->addNative("function LogFatal(text)", &js_LogFatal, nullptr);
}

void ECS::ScriptInstance::InjectValueInJS(CTinyJS* context)
{
	for (const Ressources::ScriptVariable& VarToTransfer : m_instanceValue)
	{
		switch (VarToTransfer.Type)
		{
		case Ressources::TYPE_VARIABLE::INT:
			context->getScriptVariable(VarToTransfer.Name)->setInt(VarToTransfer.GetIntValue());
			break;
		case Ressources::TYPE_VARIABLE::DOUBLE:
			context->getScriptVariable(VarToTransfer.Name)->setDouble(VarToTransfer.GetDoubleValue());
			break;
		case Ressources::TYPE_VARIABLE::STRING:
			context->getScriptVariable(VarToTransfer.Name)->setString(VarToTransfer.GetStringValue());
			break;
		}
	}
}

void ECS::ScriptInstance::ExtractValueFromJS(CTinyJS* context)
{
	if (m_script == nullptr)
		return;

	for (unsigned int i = 0; i < m_instanceValue.size(); i++)
	{
		switch (m_instanceValue[i].Type)
		{
		case Ressources::TYPE_VARIABLE::INT:
			m_instanceValue[i].GetIntValue() = context->getScriptVariable(m_instanceValue[i].Name)->getInt();
			break;
		case Ressources::TYPE_VARIABLE::DOUBLE:
			m_instanceValue[i].GetDoubleValue() = context->getScriptVariable(m_instanceValue[i].Name)->getDouble();
			break;
		case Ressources::TYPE_VARIABLE::STRING:
			m_instanceValue[i].GetStringValue() = context->getScriptVariable(m_instanceValue[i].Name)->getString();
			break;
		default:
			break;
		}
	}
}

void ECS::ScriptInstance::DuplicateVariableFromSource()
{
	if (m_instanceValue.size() == m_script->GetVariableDefault().size())
		return;

	std::unordered_map<std::string, int> intMap;
	std::unordered_map<std::string, double> doubleMap;
	std::unordered_map<std::string, std::string> stringMap;

	for (const Ressources::ScriptVariable& curVar : m_instanceValue)
	{
		switch (curVar.Type)
		{
		case Ressources::TYPE_VARIABLE::INT:
			intMap.insert({ curVar.Name, curVar.GetIntValue() });
			break;
		case Ressources::TYPE_VARIABLE::DOUBLE:
			doubleMap.insert({ curVar.Name, curVar.GetDoubleValue() });
			break;
		case Ressources::TYPE_VARIABLE::STRING:
			stringMap.insert({ curVar.Name, curVar.GetStringValue() });
			break;
		}
	}

	m_instanceValue.clear();

	for (const Ressources::ScriptVariable& currentVariable : m_script->GetVariableDefault())
		m_instanceValue.emplace_back(currentVariable);

	for (const std::pair<std::string, int>& curInt : intMap)
	{
		Ressources::ScriptVariable* curVar = GetValue(curInt.first);

		if (curVar != nullptr)
			curVar->GetIntValue() = curInt.second;
	}

	for (const std::pair<std::string, double>& curDouble : doubleMap)
	{
		Ressources::ScriptVariable* curVar = GetValue(curDouble.first);

		if (curVar != nullptr)
			curVar->GetDoubleValue() = curDouble.second;
	}

	for (const std::pair<std::string, std::string>& curString : stringMap)
	{
		Ressources::ScriptVariable* curVar = GetValue(curString.first);

		if (curVar != nullptr)
			curVar->GetStringValue() = curString.second;
	}
}
