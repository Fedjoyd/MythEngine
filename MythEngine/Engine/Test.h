#pragma once

#include "Ressources/RessourcesManager.h"
#include "Ressources/Scene.h"
#include "Ressources/Ressource.h"
#include "ECS/Component.h"
#include "Core/GameObject.h"

class TestRessource : public Ressources::Ressource
{
public:
    TestRessource(const std::string& p_name) : Ressource(p_name) {}
    ~TestRessource() {}

    const std::type_info& GetRessourceType() const override { return typeid(*this); }

#ifdef MODE_EDITOR
    void ShowEditorControl() override;
#endif // MODE_EDITOR

private:
};

class TestComponent : public ECS::Component
{
public:
    TestComponent() : Component(), m_ressource(nullptr) {}
    ~TestComponent() {}

    const std::type_info& GetComponentType() const override { return typeid(*this); }

    std::shared_ptr<TestRessource>& GetCurrentRessource() { return m_ressource; }
    const std::shared_ptr<TestRessource>& GetCurrentRessource() const { return m_ressource; }

#ifdef MODE_EDITOR
    void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // MODE_EDITOR

private:
    std::shared_ptr<TestRessource> m_ressource;
};/**/

class TestScene : public Ressources::Scene
{
public:
    TestScene(const std::string& p_name) : Scene(p_name) {}
    ~TestScene() {}

    void RegisterGameObjects(std::vector<std::unique_ptr<Core::GameObject>>& p_GameObjectList) override;

private:
};