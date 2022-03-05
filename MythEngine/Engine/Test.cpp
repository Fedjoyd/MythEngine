#include "Test.h"

#include "imgui.h"

void TestScene::RegisterGameObjects(std::vector<std::unique_ptr<Core::GameObject>>& p_GameObjectList)
{
    TestComponent* currentComponent = new TestComponent();
    TestComponent* currentComponent_1 = new TestComponent();
    TestComponent* currentComponent_2 = new TestComponent();

    if (m_ressourceManager != nullptr)
    {
        std::shared_ptr<TestRessource> currentRessource = m_ressourceManager->GetRessource<TestRessource>("TheTestRess");

        if (currentRessource == nullptr)
        {
            currentRessource = std::make_shared<TestRessource>("TheTestRess");
            m_ressourceManager->RegisterAndLoadRessource(std::static_pointer_cast<Ressources::Ressource, TestRessource>(currentRessource));
        }

        currentComponent->GetCurrentRessource() = currentRessource;
        currentComponent_1->GetCurrentRessource() = currentRessource;
        currentComponent_2->GetCurrentRessource() = currentRessource;
    }

    Core::GameObject* newGameObj = new Core::GameObject("testGO");
    newGameObj->GetComponents().emplace_back(currentComponent);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_2");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_3");
    newGameObj->GetComponents().emplace_back(currentComponent_1);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_4");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_5");
    newGameObj->GetComponents().emplace_back(currentComponent_2);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_6");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_7");
    p_GameObjectList.emplace_back(newGameObj);
}

#ifdef MODE_EDITOR
void TestRessource::ShowEditorControl()
{
    ImGui::Text("ressource name : %s", GetName().c_str());
}

void TestComponent::ShowEditorControl(const unsigned int p_indexComponent)
{
    ImGui::Text("ressource ptr : %p", m_ressource.get());
}
#endif // MODE_EDITOR
