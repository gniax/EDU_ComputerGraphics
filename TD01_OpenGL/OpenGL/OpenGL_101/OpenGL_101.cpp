// -----------------
// CONTROLES :
// 
// Z Q S D
// Clic droit de souris pour rotate la camera
// Zoom possible avec molette
// 
// --
// 
// GLEW STATIC indique qu'on utilise la librairie gllew32s
#define GLEW_STATIC 1
#define TINYOBJLOADER_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../common/Camera.h"
#include "../common/Object3D.h"
#include "../common/Texture.h"
#include "../common/Shader.h"
#include "../common/Grid3D.h"

#include <iostream>
#include <vector>

// Inutile dans mon cas car je suis sur NVIDIA
extern "C"
{
    _declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}

// Caméra
Camera g_Camera;

// Objet 3D
Object3D g_Object1;
Object3D g_Object2;
Object3D g_Object3;
Grid3D g_Grid;

Shader g_Shader1;
Shader g_Shader2;
Shader g_Shader3;
Shader g_GridShader;

// Texture
Texture g_Texture("dragon.png");

// Dimensions initiales de la fenêtre
int g_WindowSizeX = 1200;
int g_WindowSizeY = 800;

// Pointeur sur la fenêtre principale
GLFWwindow* g_Window = NULL;

//Gerer les erreurs de GLFW
static void ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Gerer le redimensionnement de la fenetre
static void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    g_WindowSizeX = width;
    g_WindowSizeY = height;

    // On ajuste le viewport quand la fenêtre est redimensionnée
    glViewport(0, 0, width, height);
}

// Suivre si le bouton droit de la souris est enfoncé
bool g_RightMouseButtonDown = false;

// Callback pour le bouton de la souris
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            g_RightMouseButtonDown = true;
        else if (action == GLFW_RELEASE)
            g_RightMouseButtonDown = false;
    }
}

// Callback pour la molette de la souris
static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    const float zoomSpeed = 0.65f;
    g_Camera.zoom(zoomSpeed * (float)yoffset);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // bottom vers top

    lastX = xpos;
    lastY = ypos;

    if (g_RightMouseButtonDown)
    {
        const float sensitivity = 0.25f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        float lYaw = g_Camera.getYaw();
        float lPitch = g_Camera.getPitch();

        g_Camera.setYaw(lYaw + xoffset);
        g_Camera.setPitch(lPitch + yoffset);

        // on evite que l'ecran se retourne
        if (lPitch + yoffset > 89.0f)
            g_Camera.setPitch(89.0f);
        if (lPitch + yoffset < -89.0f)
            g_Camera.setPitch(-89.0f);

        g_Camera.updateViewMatrix();
    }
}



// Gérer les entrées du clavier
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float cameraSpeed = 0.75f;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f)); // avant
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f)); // arrière
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f)); // gauche
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f)); // droite
}

// Initialiser GLFW et GLEW
bool InitOpenGL()
{
    // Initialisation avec GLFW
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    // Creation d'une fenêtre avec GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    g_Window = glfwCreateWindow(g_WindowSizeX, g_WindowSizeY, "OpenGL Course", NULL, NULL);

    if (g_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        return false;
    }

    // Capture the mouse cursor
    glfwSetKeyCallback(g_Window, KeyCallback);
    glfwSetCursorPosCallback(g_Window, MouseCallback);
    glfwSetMouseButtonCallback(g_Window, MouseButtonCallback);
    glfwSetScrollCallback(g_Window, ScrollCallback);

    // Definition du contexte OpenGL a utiliser par GLFW
    glfwMakeContextCurrent(g_Window);
    glfwSetWindowSizeCallback(g_Window, WindowSizeCallback);

    // Initialisation de la bibliothèque GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return false;
    }

    // Affichage de la version d'OpenGL utilisee
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    // Definition du mode de remplissage
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Definition de la couleur de fond
    glClearColor(0.25f, 0.45f, 0.75f, 1.0f);

    // Activation du z-buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // melange de couleurs
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compilation des shaders
    g_Shader3 = Shader("basic");
    if (!g_Shader3.LoadVertexShader("shaders/basic.vs.glsl") ||
        !g_Shader3.LoadFragmentShader("shaders/basic.fs.glsl") ||
        !g_Shader3.Create())
    {
        std::cerr << "Failed to create basic shader." << std::endl;
        return false;
    }

    g_Shader2 = Shader("blinnphong");
    if (!g_Shader2.LoadVertexShader("shaders/blinnphong.vs.glsl") ||
        !g_Shader2.LoadFragmentShader("shaders/blinnphong.fs.glsl") ||
        !g_Shader2.Create())
    {
        std::cerr << "Failed to create blinnphong shader." << std::endl;
        return false;
    }

    g_Shader1 = Shader("phong");
    if (!g_Shader1.LoadVertexShader("shaders/phong.vs.glsl") ||
        !g_Shader1.LoadFragmentShader("shaders/phong.fs.glsl") ||
        !g_Shader1.Create())
    {
        std::cerr << "Failed to create phong shader." << std::endl;
        return false;
    }

    g_GridShader = Shader("grid");
    if (!g_GridShader.LoadVertexShader("shaders/grid.vs.glsl") ||
        !g_GridShader.LoadFragmentShader("shaders/grid.fs.glsl") ||
        !g_GridShader.Create())
    {
        std::cerr << "Failed to create grid shader." << std::endl;
        return false;
    }

    // charger les objets 3D
    if (!g_Object1.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 1." << std::endl;
        return false;
    }
    g_Object1.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    g_Object1.SetRotation(45, glm::vec3(1.0f, 1.0f, 1.0f)); // on tourne celui-ci par exemple

    if (!g_Object2.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 2." << std::endl;
        return false;
    }
    g_Object2.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); 

    if (!g_Object3.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 3." << std::endl;
        return false;
    }
    g_Object3.SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));

    // grille de repere 3d
    g_Grid = Grid3D(100, 1.0f, g_GridShader);

    // Charger la texture
    if (!g_Texture.LoadTexture()) {
        std::cerr << "Failed to load the texture." << std::endl;
        return false;
    }

    // Position de depart de ma camera
    g_Camera.setPosition(glm::vec3(5.0f, 5.0f, 20.5f)); // deplace la caméra 3 fois plus loin que la position du cube
    g_Camera.setPitch(-15.0f);

    return true;
}

void DrawObject(Object3D &pObject, Shader& pShader)
{
    // Utilisation du shader
    pShader.use();

    std::string shaderName = pShader.getName();
    if (shaderName == "basic")
    {
        pShader.setVec4("u_Color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        pShader.setVec3("v_Color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        pShader.setVec3("a_Color", glm::vec3(1.0f, 0.5f, 0.31f));
    }
    else if (shaderName == "blinnphong")
    {
        float temps = static_cast<float>(glfwGetTime());
        const int32_t TIME = glGetUniformLocation(pShader.getProgramID(), "u_Time");
        glUniform1f(TIME, temps);

        // BlinnPhong Shader
        pShader.setVec3("u_ViewPosition", pObject.GetPosition());
        pShader.setVec3("u_LightDirection", glm::vec3(1.0f, 1.0f, 1.0f));
        pShader.setVec3("u_Material.DiffuseColor", glm::vec3(0.8f, 0.8f, 0.8f));
        pShader.setVec3("u_Material.SpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
        pShader.setFloat("u_Material.Shininess", 32.0f);

        pShader.setMat4("u_ViewMatrix", g_Camera.getViewMatrix());
        pShader.setMat4("u_ProjectionMatrix", g_Camera.getProjectionMatrix(g_WindowSizeX, g_WindowSizeY));
        glm::mat4 model = pObject.GetTransformation();
        pShader.setMat4("model", model);

        pShader.setInt("u_Sampler", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_Texture.GetTextureID());
    }

    else if (shaderName == "phong")
    {
        // Phong Shader
        glm::vec3 lightPosition = pObject.GetPosition();
        pShader.setVec3("viewPos", g_Camera.getPosition());
        pShader.setVec3("light.position", lightPosition);
        pShader.setVec3("light.ambient", glm::vec3(0.1f, 0.8f, 0.1f));
        pShader.setVec3("light.diffuse", glm::vec3(0.1f, 0.2f, 0.9f));
        pShader.setVec3("light.specular", glm::vec3(0.3f, 0.4f, 1.0f));
        pShader.setInt("material.diffuse", 0);
        pShader.setInt("material.specular", 1);
        pShader.setFloat("material.shininess", 32.0f);
    }

    // Maj de la matrice de projection
    glm::mat4 projection = g_Camera.getProjectionMatrix(g_WindowSizeX, g_WindowSizeY);
    pShader.setMat4("projection", projection);

    glm::mat4 model = pObject.GetTransformation();
    pShader.setMat4("model", model);

    // Maj de la matrice de vue de l'objet
    glm::mat4 view = g_Camera.getViewMatrix();
    pShader.setMat4("view", view);

    // Dessin de l'objet
    pObject.Draw();

    // Utilisation de la texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_Texture.GetTextureID());
}

void DrawGrid()
{
    // Applique les transfo et la camera 
    glm::mat4 projection = g_Camera.getProjectionMatrix(g_WindowSizeX, g_WindowSizeY);
    glm::mat4 view = g_Camera.getViewMatrix();

    // On draw la grid
    g_GridShader.use();
    g_GridShader.setMat4("projection", projection);
    g_GridShader.setMat4("view", view);
    g_Grid.drawGrid();
}


int main(int argc, char** argv)
{
    if (!InitOpenGL())
    {
        std::cerr << "Failed to initialize OpenGL." << std::endl;
        return -1;
    }

    // Boucle principale
    while (!glfwWindowShouldClose(g_Window)) {
        // Clears the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawGrid();

        // Draw premier cube + shader 1
        DrawObject(g_Object1, g_Shader1);

        // Draw second cube + shader 3 
        DrawObject(g_Object2, g_Shader3);

        // Draw troisieme object + shader 2
        DrawObject(g_Object3, g_Shader2);

        // Swap les buffers
        glfwSwapBuffers(g_Window);

        // Events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
