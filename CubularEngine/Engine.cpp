#include "stdafx.h"
#include "Camera.h"
#include "Engine.h"
#include "Input.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderHelper.h"
#include "SceneGraph.h"
#include "ResourceManager.h"

Engine::Engine() { }

Engine::~Engine()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //glfwDestroyWindow( window );
    glfwTerminate();
    if ( fs ) glDeleteShader( fs );
    if ( vs ) glDeleteShader( vs );
    if ( camera ) delete camera;
    if ( sceneGraph ) delete sceneGraph;

    ResourceManager::ReleaseInstance();
    Input::Release();
    Configs::Release();
}

int Engine::Init()
{
    //initialize configs
    config = Configs::GetInstance();

    //initializing GLFW
    glfwSetErrorCallback( Engine::ErrorCallBack );
    if ( glfwInit() == GLFW_FALSE )
    {
        DEBUG_PRINT( "GLFW failed to initialize" );
        return 1;
    }
    else DEBUG_PRINT( "GLFW successfully initialized!" );

    //creating window
    window = glfwCreateWindow(
        config->GetWindowWidth(),
        config->GetWindowHeight(),
        config->GetWindowTitle(),
        nullptr,
        nullptr
    );
    if ( window == nullptr )
    {
        DEBUG_PRINT( "GLFW failed to create window" );
        return 1;
    }
    else DEBUG_PRINT( "Window successfully created!" );
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );

    //init glew
    if ( glewInit() != GLEW_OK )
    {
        DEBUG_PRINT( "GLEW failed to initialize" );
        return 1;
    }
    else DEBUG_PRINT( "GLEW successfully initialized!" );

    // Set up Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // binding for glfw platform and openGL
    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( "#version 400" );

    // style
    ImGui::StyleColorsDark();

    int result = LoadAssets();
    if ( result != 0 ) return result;

    sceneGraph = new SceneGraph();
    sceneGraph->InitFromConfig();

    //init systems
    input = Input::GetInstance();
    input->Init( window );

    return 0;
}

void Engine::Run()
{
    //set up viewport
    int windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    glViewport( 0, 0, windowWidth, windowHeight );

    //set up other stuff
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    DEBUG_PRINT( "Graphics is ready!" );

    while ( !glfwWindowShouldClose( window ) )
    {
        /* INPUT */
        glfwPollEvents();

        //breaks out of the loop if user presses ESC
        if ( input->IsKeyDown( GLFW_KEY_ESCAPE ) )
            break;

        //ready imgui for the next frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* GAMEPLAY UPDATE */
        sceneGraph->Update();
        camera->Update();

        /* PRE-RENDER */
        //start off with clearing the 'color buffer'
        //clear the window to have c o r n f l o w e r   b l u e
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor( 0.392f, 0.584f, 0.929f, 1.0f );

        /* RENDER */
        sceneGraph->Render( camera );

        /* POST-RENDER */
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

        //'clear' for next draw call
        glBindVertexArray( 0 );
        glUseProgram( 0 );

        //swaps the front buffer with the back buffer
        glfwSwapBuffers( window );
    }
}

void Engine::ErrorCallBack( int error, const char * description )
{
    DEBUG_PRINT( "GLFW Error %d: %s\n", error, description );
}

int Engine::LoadAssets()
{
    ResourceManager *resourceMngr = ResourceManager::GetInstance();

    //load and build shaders
    vs = ShaderHelper::LoadFromFile( "assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER );
    fs = ShaderHelper::LoadFromFile( "assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER );
    shaderProgram = ShaderHelper::LinkShaderProgram( vs, fs );
    if ( shaderProgram == 0 )
    {
        DEBUG_PRINT( "Shader Program failed to link" );
        return 1;
    }

    //load mesh
    GLfloat vertices[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };
    Mesh* tankMesh = new Mesh();
    tankMesh->InitWithVertexArray( vertices, _countof( vertices ), shaderProgram );
    resourceMngr->AddMesh( std::string( C_MESH_TANK ), tankMesh );

    //load material
    Material* tankMaterial = new Material( shaderProgram );
    resourceMngr->AddMaterial( std::string( C_MAT_TANK ), tankMaterial );

    camera = new Camera(
        static_cast<float>( config->GetWindowWidth() ),
        static_cast<float>( config->GetWindowHeight() )
    );

    return 0;
}

