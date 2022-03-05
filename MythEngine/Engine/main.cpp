#include "includes/Core/System/Application.h"

#include "Test.h"

int main()
{
    Core::System::Application::Initialize(900, 600, "Hello World");

    std::shared_ptr<Ressources::Scene> scnToRegister = std::static_pointer_cast<Ressources::Scene, TestScene>(std::make_shared<TestScene>("scene_de_test"));
    Core::System::Application::GetInstance().GetRessourcesManager().AddScene(scnToRegister);
    Core::System::Application::QueryLoadScene("scene_de_test");

    // Loop until the user closes the window
    while (!Core::System::Application::ShouldExit())
    {
        Core::System::Application::UpdateAndDraw();
    }

    Core::System::Application::Kill();

    return 0;
}
