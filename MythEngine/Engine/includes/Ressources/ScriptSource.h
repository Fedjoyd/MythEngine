#pragma once

#include "Ressource.h"

#include <string>
#include <vector>

#include "tinyJS/TinyJS.h"

namespace Ressources
{
	enum class TYPE_VARIABLE : unsigned int
	{
		INT = 0u,
		DOUBLE,
		STRING
	};

	class ScriptVariable
	{
	public:
		ScriptVariable(const ScriptVariable& other);
		ScriptVariable(const std::string& p_Name, const int p_intValue) : Name(p_Name), Type(TYPE_VARIABLE::INT) { Value = (void*)new int(p_intValue); }
		ScriptVariable(const std::string& p_Name, const double p_doubleValue) : Name(p_Name), Type(TYPE_VARIABLE::DOUBLE) { Value = (void*)new double(p_doubleValue); }
		ScriptVariable(const std::string& p_Name, const std::string& p_stringValue) : Name(p_Name), Type(TYPE_VARIABLE::STRING) { Value = (void*)new std::string(p_stringValue); }
		~ScriptVariable();

		std::string Name;
		TYPE_VARIABLE Type;

		void* Value;

		int& GetIntValue() { return *((int*)Value); }
		int  GetIntValue() const { return *((int*)Value); }
		double& GetDoubleValue() { return *((double*)Value); }
		double  GetDoubleValue() const { return *((double*)Value); }
		std::string& GetStringValue() { return *((std::string*)Value); }
		const std::string& GetStringValue() const { return *((std::string*)Value); }

		int* GetIntPtr() { return (int*)Value; }
		double* GetDoublePtr() { return (double*)Value; }
		std::string* GetStringPtr() { return (std::string*)Value; }
	};

	class ScriptSource : public Ressource
	{
	public:
		ScriptSource(const std::string& p_name);
		ScriptSource(const std::string& p_name, const std::string& p_path);
		~ScriptSource() {}

		const std::type_info& GetRessourceType() const override { return typeid(*this); }

		void  SetPath(const std::string& p_path) { m_path = p_path; m_isFile = true; m_source = ""; }
		const std::string& GetPath() const { return m_path; }
		void  SetSource(const std::string& p_source) { m_path = ""; m_isFile = false; m_source = p_source; }
		const std::string& GetSource() const { return m_source; }

		const std::vector<ScriptVariable>& GetVariableDefault() const { return m_variableDefault; }

		bool HasStartFunction() const { return m_hasStartFunction; }
		bool HasUpdateFunction() const { return m_hasUpdateFunction; }
		bool HasFixedUpdateFunction() const { return m_hasFixedUpdateFunction; }
		bool HasDrawFunction() const { return m_hasDrawFunction; }

		void LoadRessource() override;
		void UnloadRessource() override;

#ifdef MODE_EDITOR
		bool HasEditorUpdateFunction() const { return m_hasEditorUpdateFunction; }
		void ShowEditorControl() override;
#endif // MODE_EDITOR

	private:
		bool        m_isFile;
		std::string m_path;
		std::string m_source;

		std::vector<ScriptVariable> m_variableDefault;

		bool m_hasStartFunction = false;
		bool m_hasUpdateFunction = false;
		bool m_hasFixedUpdateFunction = false;
#ifdef MODE_EDITOR
		bool m_hasEditorUpdateFunction = false;
#endif // MODE_EDITOR
		bool m_hasDrawFunction = false;
	};
}