#include "stdafx.h"
#include "Camera.h"
#include "Engine.h"
#include "GameEntity.h"
#include "Input.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderHelper.h"

Engine::Engine()
{
}


Engine::~Engine()
{
    glfwTerminate();
    if ( fs ) glDeleteShader( fs );
    if ( vs ) glDeleteShader( vs );
    if ( tankMesh ) delete tankMesh;
    if ( tankMaterial ) delete tankMaterial;
    if ( camera ) delete camera;
    for ( size_t i = 0; i < gameEntities.size(); ++i )
        delete gameEntities[ i ];

    Input::Release();
}

int Engine::Init()
{
    //initializing GLFW
    if ( glfwInit() == GLFW_FALSE )
    {
        DEBUG_PRINT( "GLFW failed to initialize" );
        return 1;
    }
    else DEBUG_PRINT( "GLFW successfully initialized!" );

    //creating window
    window = glfwCreateWindow(
        Configs::WindowWidth,
        Configs::WindowHeight,
        Configs::WindowTitle,
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

    //init glew
    if ( glewInit() != GLEW_OK )
    {
        DEBUG_PRINT( "GLEW failed to initialize" );
        return 1;
    }
    else DEBUG_PRINT( "GLEW successfully initialized!" );

    int result = LoadAssets();
    if ( result != 0 ) return result;

    result = InitGameEntities();
    if ( result != 0 ) return result;

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

        /* GAMEPLAY UPDATE */
        for ( size_t i = 0; i < gameEntities.size(); ++i )
            gameEntities[ i ]->Update();

        camera->Update();

        /* PRE-RENDER */
        //start off with clearing the 'color buffer'
        //clear the window to have c o r n f l o w e r   b l u e
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor( 0.392f, 0.584f, 0.929f, 1.0f );

        /* RENDER */
        for ( size_t i = 0; i < gameEntities.size(); ++i )
            gameEntities[ i ]->Render( camera );

        /* POST-RENDER */

        //'clear' for next draw call
        glBindVertexArray( 0 );
        glUseProgram( 0 );

        //swaps the front buffer with the back buffer
        glfwSwapBuffers( window );
    }
}

int Engine::LoadAssets()
{
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
    tankMesh = new Mesh();
    tankMesh->InitWithVertexArray( vertices, _countof( vertices ), shaderProgram );

    //load material
    tankMaterial = new Material( shaderProgram );

    camera = new Camera(
        static_cast<float>( Configs::WindowWidth ),
        static_cast<float>( Configs::WindowHeight )
    );

    return 0;
}

int Engine::InitGameEntities()
{
    gameEntities.push_back( new GameEntity( 
        tankMesh,
        tankMaterial,
        glm::vec3( 0.f ),
        glm::vec3( 0.f ),
        glm::vec3( 1.f )
    ) );
    return 0;
}
