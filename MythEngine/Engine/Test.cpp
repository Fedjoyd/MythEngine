#include "Test.h"
#include "ECS/Model.h"

void TestScene::RegisterGameObjects(std::vector<std::unique_ptr<Core::GameObject>>& p_GameObjectList)
{
    ECS::Component* currentComponent = new ECS::Model("ressources/assets/sphere.obj");
    ECS::Component* newComponent = new ECS::Model("ressources/assets/cube.obj");

    Core::GameObject* newGameObj = new Core::GameObject("testGO");
    newGameObj->GetComponents().emplace_back(currentComponent);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_2");
    newGameObj->GetComponents().emplace_back(newComponent);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_3");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_4");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_5");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_6");
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_7");
    p_GameObjectList.emplace_back(newGameObj);
}