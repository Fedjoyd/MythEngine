#include "..\..\includes\Ressources\ScriptSource.h"

#include <fstream>

#include "Core/Debug/Log.h"

#include "imgui_stdlib.h"

Ressources::ScriptVariable::ScriptVariable(const ScriptVariable& other) :
    Name(other.Name),
    Type(other.Type)
{
    Value = nullptr;
    switch (other.Type)
    {
    case TYPE_VARIABLE::INT:
        Value = (void*)new int(other.GetIntValue());
        break;
    case TYPE_VARIABLE::DOUBLE:
        Value = (void*)new double(other.GetDoubleValue());
        break;
    case TYPE_VARIABLE::STRING:
        Value = (void*)new std::string(other.GetStringValue());
        break;
    }
}

Ressources::ScriptVariable::~ScriptVariable()
{
    switch (Type)
    {
    case TYPE_VARIABLE::INT:
        delete ((int*)Value);
        break;
    case TYPE_VARIABLE::DOUBLE:
        delete ((double*)Value);
        break;
    case TYPE_VARIABLE::STRING:
        delete ((std::string*)Value);
        break;
    }
}

Ressources::ScriptSource::ScriptSource(const std::string& p_name) :
	Ressource(p_name),
	m_isFile(false),
	m_path(""),
	m_source("")
{
}

Ressources::ScriptSource::ScriptSource(const std::string& p_name, const std::string& p_path) :
	Ressource(p_name),
	m_isFile(true),
	m_path(p_path),
	m_source("")
{
}

void Ressources::ScriptSource::LoadRessource()
{
    if (m_loaded)
        return;
    m_loaded = true;

    if (m_isFile)
    {
        std::ifstream fichier(m_path.c_str(), std::ios::in);  // on ouvre en lecture

        if (fichier)  // si l'ouverture a fonctionn�
        {
            std::string currentLigne;  // d�claration d'une cha�ne qui contiendra la ligne lue
            while (std::getline(fichier, currentLigne))  // on met dans "contenu" la ligne
                m_source += currentLigne + '\n';

            fichier.close();
        }
        else
        {
            MYTH_ERROR("error on script loading");
            UnloadRessource();
        }
    }

    CTinyJS* JSLoadcontext = new CTinyJS();
    CScriptVar* varLink = nullptr;

    try {
        JSLoadcontext->execute(m_source);
        varLink = JSLoadcontext->root;
    }
    catch (CScriptException* e) { MYTH_ERROR(e->text.c_str()); UnloadRessource(); }


    if (varLink != nullptr) {
        if (!varLink->isUndefined())
        {
            CScriptVarLink* link = varLink->firstChild;

            while (link) {
                if (link->var->isInt())
                    m_variableDefault.push_back(ScriptVariable(link->name, link->var->getInt()));
                if (link->var->isDouble())
                    m_variableDefault.push_back(ScriptVariable(link->name, link->var->getDouble()));
                if (link->var->isString())
                    m_variableDefault.push_back(ScriptVariable(link->name, link->var->getString()));

                if (link->var->isFunction() && link->name == "Start") m_hasStartFunction = true;
                if (link->var->isFunction() && link->name == "Update") m_hasUpdateFunction = true;
                if (link->var->isFunction() && link->name == "FixedUpdate") m_hasFixedUpdateFunction = true;
#ifdef MODE_EDITOR
                if (link->var->isFunction() && link->name == "EditorUpdate") m_hasEditorUpdateFunction = true;
#endif // MODE_EDITOR
                if (link->var->isFunction() && link->name == "Draw") m_hasDrawFunction = true;

                link = link->nextSibling;
            }
        }
    }

    delete JSLoadcontext;
}

void Ressources::ScriptSource::UnloadRessource()
{
    if (m_isFile)
        m_source = "";
    
    m_hasStartFunction = false;
    m_hasUpdateFunction = false;
    m_hasFixedUpdateFunction = false;
#ifdef MODE_EDITOR
    m_hasEditorUpdateFunction = false;
#endif // MODE_EDITOR
    m_hasDrawFunction = false;

    m_variableDefault.clear();

    m_loaded = false;
}

#ifdef MODE_EDITOR
void Ressources::ScriptSource::ShowEditorControl()
{
    ImGui::Value("Has Start function", m_hasStartFunction);
    ImGui::Value("Has Update function", m_hasUpdateFunction);
    ImGui::Value("Has FixedUpdate function", m_hasFixedUpdateFunction);
    ImGui::Value("Has EditorUpdate function", m_hasEditorUpdateFunction);
    ImGui::Value("Has Draw function", m_hasDrawFunction);
    
    ImGui::Spacing();

    for (unsigned int i = 0; i < m_variableDefault.size(); i++)
    {
        switch (m_variableDefault[i].Type)
        {
        case TYPE_VARIABLE::INT:
            ImGui::InputInt((m_variableDefault[i].Name + "##ScriptSource").c_str(), m_variableDefault[i].GetIntPtr());
            break;
        case TYPE_VARIABLE::DOUBLE:
            ImGui::InputDouble((m_variableDefault[i].Name + "##ScriptSource").c_str(), m_variableDefault[i].GetDoublePtr());
            break;
        case TYPE_VARIABLE::STRING:
            ImGui::InputText((m_variableDefault[i].Name + "##ScriptSource").c_str(), m_variableDefault[i].GetStringPtr());
            break;
        }
    }
}
#endif // MODE_EDITOR
