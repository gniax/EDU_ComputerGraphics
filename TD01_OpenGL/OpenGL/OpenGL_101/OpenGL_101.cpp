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
int g_WindowSizeX = 800;
int g_WindowSizeY = 600;

// Pointeur sur la fenêtre principale
GLFWwindow* g_Window = NULL;

// Méthode pour gérer les erreurs de GLFW
static void ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Méthode pour gérer le redimensionnement de la fenêtre
static void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    g_WindowSizeX = width;
    g_WindowSizeY = height;

    // On ajuste le viewport quand la fenêtre est redimensionnée
    glViewport(0, 0, width, height);
}

// Variable pour suivre si le bouton droit de la souris est enfoncé
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (lPitch + yoffset > 89.0f)
            g_Camera.setPitch(89.0f);
        if (lPitch + yoffset < -89.0f)
            g_Camera.setPitch(-89.0f);

        g_Camera.updateViewMatrix();
    }
}



// Méthode pour gérer les entrées du clavier
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float cameraSpeed = 0.75f; // ajustez la vitesse au besoin
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f)); // avant
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f)); // arrière
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f)); // gauche
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        g_Camera.move(cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f)); // droite
}

// Methode pour initialiser GLFW et GLEW
bool InitOpenGL()
{
    // Initialisation de la bibliothèque GLFW
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

    // Charger les objets 3D
    if (!g_Object1.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 1." << std::endl;
        return false;
    }
    g_Object1.BindShader(g_Shader1);
    //g_Object1.SetScale(5.0f); // réduit la taille du cube de /3

    if (!g_Object2.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 2." << std::endl;
        return false;
    }
    g_Object2.BindShader(g_Shader2);
    g_Object2.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // déplace l'objet en (0, 0, -5)
    //g_Object2.SetScale(0.3f); // réduit la taille du cube de /3

    if (!g_Object3.LoadFromFile("cube.obj")) {
        std::cerr << "Failed to load the 3D object 3." << std::endl;
        return false;
    }
    g_Object3.BindShader(g_Shader3);
    g_Object3.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    g_Object3.SetScale(0.001f); // réduit la taille du cube de /3

    // grille de repere 3d
    g_Grid = Grid3D(100, 1.0f, g_GridShader);

    // Charger la texture
    if (!g_Texture.LoadTexture()) {
        std::cerr << "Failed to load the texture." << std::endl;
        return false;
    }

    // Position de depart de ma camera
    g_Camera.setPosition(glm::vec3(5.0f, 5.0f, 20.5f)); // déplace la caméra 3 fois plus loin que la position du cube
    g_Camera.setPitch(-15.0f);

    return true;
}

void DrawObject(Object3D &pObject)
{
    Shader lShader = pObject.GetShader();
    // Utilisation du shader
    lShader.use();

    std::string shaderName = lShader.getName();
    if (shaderName == "basic")
    {
        // Basic Shader - we only modify the color
        lShader.setVec3("v_Color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        lShader.setVec3("a_Position", pObject.GetPosition());
        lShader.setVec3("a_Color", glm::vec3(1.0f, 0.5f, 0.31f));
    }
    else if (shaderName == "blinnphong")
    {
        // BlinnPhong Shader
        lShader.setVec3("u_ViewPosition", pObject.GetPosition());
        lShader.setVec3("u_LightDirection", glm::vec3(1.0f, 1.0f, 1.0f));
        lShader.setVec3("u_Material.DiffuseColor", glm::vec3(0.8f, 0.8f, 0.8f));
        lShader.setVec3("u_Material.SpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lShader.setFloat("u_Material.Shininess", 32.0f);
        lShader.setMat4("u_ProjectionMatrix", g_Camera.getProjectionMatrix(g_WindowSizeX, g_WindowSizeY));
        lShader.setInt("u_TextureSampler", 0);  // Assurez-vous que la texture est liée à l'emplacement de texture 0
    }
    else if (shaderName == "phong")
    {
        // Phong Shader
        glm::vec3 lightPosition = pObject.GetPosition();
        lShader.setVec3("viewPos", g_Camera.getPosition());
        lShader.setVec3("light.position", lightPosition);
        lShader.setVec3("light.ambient", glm::vec3(0.1f, 0.8f, 0.1f));
        lShader.setVec3("light.diffuse", glm::vec3(0.1f, 0.2f, 0.9f));
        lShader.setVec3("light.specular", glm::vec3(0.3f, 0.4f, 1.0f));
        lShader.setInt("material.diffuse", 0);
        lShader.setInt("material.specular", 1);
        lShader.setFloat("material.shininess", 32.0f);

    }

    // Mise à jour de la matrice de projection
    glm::mat4 projection = g_Camera.getProjectionMatrix(g_WindowSizeX, g_WindowSizeY);
    lShader.setMat4("projection", projection);

    glm::mat4 model = pObject.GetTransformation();
    glm::mat4 view = g_Camera.getViewMatrix();

    lShader.setMat4("model", model);
    lShader.setMat4("view", view);

    // Utilisation de la texture
    g_Texture.UseTexture();

    // Dessin de l'objet
    pObject.Draw();
}

void DrawGrid()
{
    // Apply transformations and camera setup
    glm::mat4 view = g_Camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(g_Camera.getFov()), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 100.0f);

    // Draw the grid
    g_GridShader.use();
    g_GridShader.setMat4("view", view);
    g_GridShader.setMat4("projection", projection);
    g_Grid.drawGrid();
}

int main(int argc, char** argv)
{
    if (!InitOpenGL())
    {
        std::cerr << "Failed to initialize OpenGL." << std::endl;
        return -1;
    }

    // Boucle principale de notre application
    while (!glfwWindowShouldClose(g_Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        DrawObject(g_Object1);
        glPopMatrix();

        //glPushMatrix();
        //DrawObject(g_Object2);
        //glPopMatrix();

        //glPushMatrix();
        //DrawObject(g_Object3);
        //glPopMatrix();

        glPushMatrix();
        DrawGrid();
        glPopMatrix();

        glfwSwapBuffers(g_Window);
        glfwPollEvents();
    }

    // Destruction de la fenêtre et fin du programme
    glfwDestroyWindow(g_Window);
    glfwTerminate();

    return 0;
}