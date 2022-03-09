#include "Test.h"
#include "ECS/Model.h"
#include "ECS/Camera.h"
#include "Core/System/CameraManager.h"

void TestScene::RegisterGameObjects(std::vector<std::unique_ptr<Core::GameObject>>& p_GameObjectList)
{
    ECS::Component* currentComponent = new ECS::Model("ressources/assets/Sponza/sponza.obj");
    ECS::Component* newComponent = new ECS::Model("ressources/assets/cube.obj");
    ECS::Component* newCameraBack = new ECS::Camera();

    ECS::Camera* newCamera = new ECS::Camera();
    Core::System::CameraManager::GetInstance().SetCamera(newCamera);

    Core::GameObject* newGameObj = new Core::GameObject("testGO");
    newGameObj->GetComponents().emplace_back(currentComponent);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_2");
    newGameObj->GetComponents().emplace_back(newComponent);
    newGameObj->GetComponents().emplace_back(newCameraBack);
    p_GameObjectList.emplace_back(newGameObj);

    newGameObj = new Core::GameObject("testGO_3");
    newGameObj->GetComponents().emplace_back(newCamera);
    p_GameObjectList.emplace_back(newGameObj);
}