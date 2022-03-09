#include "Core/System/Application.h"

#include "Core/System/Input.h"
#include "Core/System/Time.h"
#include "Core/System/CameraManager.h"

#ifdef MODE_EDITOR
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif // EDITOR_MODE

Core::System::Application Core::System::Application::singleton = Core::System::Application();

#ifdef MODE_EDITOR
void Core::System::Application::ShowSettingsWindow(bool* p_opened)
{
    if (p_opened != nullptr)
        if (!(*p_opened))
            return;

    ImGui::Begin("Settings", p_opened);
    ImGui::End();
}
#endif // MODE_EDITOR

bool Core::System::Application::Initialize(int p_width, int p_height, const std::string& p_name, bool p_fullscreen)
{
    const char* glsl_version = "#version 450 core";

    // Initialize the library
    if (!glfwInit())
    {
        MYTH_FATAL("GLFW initialization failed");
        return false;
    }

    singleton.m_name = p_name;

    // Create a windowed mode window and its OpenGL context
    singleton.m_glfwWindow = glfwCreateWindow(p_width, p_height, singleton.m_name.c_str(), NULL, NULL);
    if (!singleton.m_glfwWindow)
    {
        glfwTerminate();
        MYTH_FATAL("Window creation failed");
        return false;
    }

    glfwMakeContextCurrent(singleton.m_glfwWindow);
    if (gladLoadGL() == 0)
    {
        glfwTerminate();
        MYTH_FATAL("gladLoaderLoadGL failed");
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(singleton.m_glfwWindow);

#ifdef MODE_EDITOR
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(singleton.m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Core::System::SetKeyCallback(singleton.m_glfwWindow);

    System::Input::GetInstance().AddKeyInput(EDITOR_INPUT_NAME, GLFW_KEY_E);
#endif // EDITOR_MODE

    System::Input::GetInstance().AddKeyInput(CAMERA_FRONT_INPUT_NAME, GLFW_KEY_W);
    System::Input::GetInstance().AddKeyInput(CAMERA_BACK_INPUT_NAME, GLFW_KEY_S);
    System::Input::GetInstance().AddKeyInput(CAMERA_LEFT_INPUT_NAME, GLFW_KEY_A);
    System::Input::GetInstance().AddKeyInput(CAMERA_RIGHT_INPUT_NAME, GLFW_KEY_D);
    System::Input::GetInstance().AddKeyInput(CAMERA_UP_INPUT_NAME, GLFW_KEY_SPACE);
    System::Input::GetInstance().AddKeyInput(CAMERA_DOWN_INPUT_NAME, GLFW_KEY_LEFT_CONTROL);
    System::Input::GetInstance().AddKeyInput(CAMERA_SPEED_INPUT_NAME, GLFW_KEY_LEFT_SHIFT);
    System::Input::GetInstance().AddKeyInput(CAMERA_LOOK_INPUT_NAME, GLFW_MOUSE_BUTTON_2, true);

    System::CameraManager::GetInstance().Setup(singleton.m_glfwWindow);

    singleton.m_threadPool.Initialise();

    MYTH_INFO("Initialization finidshed !");

    return true;
}

void Core::System::Application::UpdateAndDraw()
{
    if (singleton.m_queryLoadScene)
    {
        std::shared_ptr<Ressources::Scene> nextScene = singleton.m_ressourcesManager.GetScene(singleton.m_nextSceneName);

        if (nextScene != nullptr)
        {
            singleton.m_gameObjectManager.LoadScene(nextScene);
            singleton.m_ressourcesManager.UnloadUnusedRessource();

            MYTH_INFO("Scene %s loaded !", nextScene->GetName().c_str());

            singleton.m_gameObjectManager.Start();
        }
        else
            MYTH_ERROR("Scene %s does not exist !", singleton.m_nextSceneName.c_str());

        singleton.m_nextSceneName = "";
        singleton.m_queryLoadScene = false;
    }

    // Poll for and process events
    glfwPollEvents();
    System::Input::GetInstance().Update(singleton.m_glfwWindow);
    System::Time::GetInstance().Update();

#ifdef MODE_EDITOR
    if (singleton.m_run)
    {
        singleton.m_gameObjectManager.Update();

        if (System::Time::Fixing())
            singleton.m_gameObjectManager.FixedUpdate();
    }
    else
    {
        singleton.m_gameObjectManager.EditorUpdate();
    }
#endif // EDITOR_MODE
#ifndef MODE_EDITOR
    singleton.m_gameObjectManager.Update();

    if (System::Time::Fixing())
        singleton.m_gameObjectManager.FixedUpdate();
#endif // EDITOR_MODE

    // Render here
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    singleton.m_gameObjectManager.FixedUpdate();
    singleton.m_gameObjectManager.Draw();

#ifdef MODE_EDITOR
    if (System::Input::IsKeyPressed(EDITOR_INPUT_NAME))
        singleton.m_editorOpen = !singleton.m_editorOpen;

    if (singleton.m_editorOpen)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (ImGuiDockNodeFlags_PassthruCentralNode & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("WorkSpace", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("EditorWorkSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "CTRL+N", false, false)) {}
                if (ImGui::MenuItem("Save", "CTRL+S", false, false)) {}
                if (ImGui::MenuItem("Open", "CTRL+O", false, false)) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}  // Disabled item
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}  // Disabled item
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}  // Disabled item
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}  // Disabled item
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("Demo ImGui", NULL, &(singleton.m_demoImGuiWindow))) {}
                if (ImGui::MenuItem("Settings", NULL, &(singleton.m_settingsWindow))) {}
                if (ImGui::MenuItem("Console", NULL, &(singleton.m_consoleWindow))) {}
                if (ImGui::MenuItem("GameObject Manager", NULL, &(singleton.m_gameObjectManagerWindow))) {}
                if (ImGui::MenuItem("GameObject Editor", NULL, &(singleton.m_gameObjectEditorrWindow))) {}
                if (ImGui::MenuItem("Ressources Manager", NULL, &(singleton.m_ressourcesManagerWindow))) {}
                if (ImGui::MenuItem("Ressources Editor", NULL, &(singleton.m_ressourceEditorWindow))) {}
                if (ImGui::MenuItem("Input Editor", NULL, &(singleton.m_inputEditorWindow))) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Run"))
            {
                if (ImGui::MenuItem("Run", NULL, &(singleton.m_run))) {}
                if (ImGui::MenuItem("Start")) { singleton.m_gameObjectManager.Start(); }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (singleton.m_demoImGuiWindow) ImGui::ShowDemoWindow(&singleton.m_demoImGuiWindow);

        singleton.ShowSettingsWindow(&singleton.m_settingsWindow);
        Input::GetInstance().ShowEditorWindow(&singleton.m_inputEditorWindow);

        singleton.m_gameObjectManager.ShowManagerWindow(&singleton.m_gameObjectManagerWindow);
        singleton.m_gameObjectManager.ShowEditorWindow(&singleton.m_gameObjectEditorrWindow);

        singleton.m_ressourcesManager.ShowManagerWindow(&singleton.m_ressourcesManagerWindow);
        singleton.m_ressourcesManager.ShowEditorWindow(&singleton.m_ressourceEditorWindow);

        Debug::Log::GetInstance().ShowEditorWindow(&singleton.m_consoleWindow);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
#endif // EDITOR_MODE

    // Swap front and back buffers
    glfwSwapBuffers(singleton.m_glfwWindow);
}

void Core::System::Application::Kill()
{
#ifdef MODE_EDITOR
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif // EDITOR_MODE

    singleton.m_threadPool.Shutdown();

    glfwDestroyWindow(singleton.m_glfwWindow);
    glfwTerminate();
}
