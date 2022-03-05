#pragma once

#include "Component.h"

#include <memory>
#include <unordered_map>

#include "tinyJS/TinyJS.h"
#include "Ressources/ScriptSource.h"

namespace ECS
{
	class ScriptInstance : public Component
	{
	public:
		ScriptInstance();
		~ScriptInstance() {}

		const std::type_info& GetComponentType() const override { return typeid(*this); }

		void SetScript(std::shared_ptr<Ressources::ScriptSource>& p_script) { m_script = p_script; }
		const std::shared_ptr<Ressources::ScriptSource>& GetScript() const { return m_script; }

		Ressources::ScriptVariable*       GetValue(const std::string& VariableName);
		const Ressources::ScriptVariable* GetValue(const std::string& VariableName) const;

		int    GetintValue(const std::string& VariableName) const;
		bool   GetBoolValue(const std::string& VariableName) const { return (GetintValue(VariableName) > 0); }
		double GetDoubleValue(const std::string& VariableName) const;
		float  GetFloatValue(const std::string& VariableName) const { return (float)GetDoubleValue(VariableName); }
		std::string GetStringValue(const std::string& VariableName) const;

		bool SetintValue(const std::string& VariableName, const int p_value);
		bool SetBoolValue(const std::string& VariableName, const bool p_value) { return SetintValue(VariableName, (int)p_value); }
		bool SetDoubleValue(const std::string& VariableName, const double p_value);
		bool SetFloatValue(const std::string& VariableName, const float p_value) { return SetDoubleValue(VariableName, (double)p_value); }
		bool SetStringValue(const std::string& VariableName, const std::string& p_value);

		void Start(Core::GameObject* p_gameObject) override;
		void Update(Core::GameObject* p_gameObject) override;
		void FixedUpdate(Core::GameObject* p_gameObject) override;
#ifdef MODE_EDITOR
		void EditorUpdate(Core::GameObject* p_gameObject) override;
		void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // MODE_EDITOR
		void Draw(Core::GameObject* p_gameObject) override;

	private:
		void InitialiseJavascriptContext(CTinyJS* context);
		void InjectValueInJS(CTinyJS* context);
		void ExtractValueFromJS(CTinyJS* context);
		void DuplicateVariableFromSource();

		std::shared_ptr<Ressources::ScriptSource> m_script;
		//std::unordered_map<std::string, CScriptVariable> m_instanceValue;
		std::vector<Ressources::ScriptVariable> m_instanceValue;
	};
}