// GLEW STATIC indique qu'on utilise la librairie gllew32s
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Inutile dans mon cas car je suis sur NVIDIA
extern "C"
{
    _declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001; 
}

struct MyApplication
{
    // Attributs
    int _width;
    int _height;

    // Définit la size
    inline void setSize(int pWidth, int pHeight)
    {
        _width = pWidth;
        _height = pHeight;
    }

    // Initialisation - a appeler au debut
    bool initialize()
    {
        std::cout << "Carte Graphique : " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version OpenGL : " << glGetString(GL_VERSION) << std::endl;
        std::cout << "Version GLSL : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        //std::cout << "Extensions disponibles : " << glGetString(GL_EXTENSIONS) << std::endl;
        
        // Afficher une extension précise à un index:
        int numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        for (int i = 0; i < numExtensions; ++i)
        {
            std::cout << "Extensions[" << i << "] : " << glGetStringi(GL_EXTENSIONS, i) << std::endl;
        }

        return true;
    }

    // TODO
    void deinitialize()
    {
        // ...
    }

    // Tout le contenu de rendu
    void render()
    {
        // Définir la taille des viewports
        int viewportWidth = _width / 2;
        int viewportHeight = _height / 2;

        static const float triangles[] = {
            -0.8f, +0.8f, // sommet #0
            +0.0f, -0.8f, // sommet #1
            +0.8f, +0.8f  // sommet #2
        };

        static const float colors[] = {
            1.0f, 1.0f, 1.0f, // sommet #0
            0.0f, 1.0f, 0.0f, // sommet #1
            0.0f, 0.0f, 1.0f  // sommet #2
        };
        
        // On définit nos canaux 'position' et 'color'
        // Note: dépend du GPU 'NVIDIA', 'AMD'...
        static constexpr uint32_t POSITION = 0;
        static constexpr uint32_t COLOR = 3;

        glEnableVertexAttribArray(POSITION);
        // on active la fonctionnalite scissor
        glEnable(GL_SCISSOR_TEST);
        //glEnable(GL_CULL_FACE); // active la suppresion des faces cachees (par defaut sens horaire)
        //glFrontFace(GL_CCW);

        // Définir le viewport pour le premier écran en haut à gauche
        glViewport(0, viewportHeight, viewportWidth, viewportHeight);
        glScissor(0, viewportHeight, viewportWidth, viewportHeight);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);        
        
        // On va créer un triangle avec des couleurs dans ce premier VP
        {
            // si le contenu des donnes est un tableau il faut le dire a OpenGL avec glEnableVertexAttribArray()
            glEnableVertexAttribArray(POSITION);
            // Les paramètres de cette fonction sont les suivants :
            // index: Le numéro du canal de données des vertex(location).
            // size : Le nombre de composantes pour chaque vertex.Par exemple, pour les positions en 3D, size est égal à 3.
            // type : Le type de données pour chaque composante.Les types les plus couramment utilisés sont GL_FLOAT pour les données à virgule flottante, GL_INT pour les données entières et GL_UNSIGNED_BYTE pour les données de couleurs en RGBA.
            // normalized : Un booléen indiquant si les données doivent être normalisées ou pas.Si normalized est vrai, les données sont normalisées entre 0 et 1 ou - 1 et 1, en fonction du type de données.
            // stride : L'espacement en octets entre chaque vertex dans le tableau des vertex
            // pointer : Le pointeur vers le début des données pour le canal.
            glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), &triangles[0]);

            // On set les couleurs avant de draw les triangles pour que ça soit pris en compte
            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), colors);
            // Puis on draw les triangles
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Définir le viewport pour le deuxième écran en haut à droite
        glViewport(viewportWidth, viewportHeight, viewportWidth, viewportHeight);
        glScissor(viewportWidth, viewportHeight, viewportWidth, viewportHeight);
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Définir le viewport pour le troisième écran en bas à gauche
        glViewport(0, 0, viewportWidth, viewportHeight);
        glScissor(0, 0, viewportWidth, viewportHeight);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Définir le viewport pour le quatrième écran en bas à droite
        glViewport(viewportWidth, 0, viewportWidth, viewportHeight);
        glScissor(viewportWidth, 0, viewportWidth, viewportHeight);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
    }
};

int main(void)
{
    GLFWwindow* window;
    MyApplication myApp;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum error = glewInit();

    // A partir de maintenant, on peut utiliser des commandes OpenGL
    if (!myApp.initialize())
    {
        return -1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // gere le dimensionnement de la fenetre
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        myApp.setSize(width, height);

        // rendu
        myApp.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    myApp.deinitialize();

    glfwTerminate();
    return 0;
}