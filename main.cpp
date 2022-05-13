#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include<vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "mesh.h"
#include "obiect.h"
#include "scena.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"

// functii ce creaza shaderele
GLuint createProgram(char* VSFile, char* FSFile);
std::string readFile(const char *filePath);

// functie ce creaza scena
// aceasta functie trebuie rescrisa de voi
Scena createScena(GLuint programShadere);


// functii ce initializeaza fereastra si
// proceseaza intrarile de la mouse si tastatura
GLFWwindow* initializari();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// configurari
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
glm::vec3 cameraPos   = glm::vec3(5.0f, 20.0f, 50.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 6.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // initializam fereastra
    GLFWwindow* fereastra = initializari();

    // cream shaderele si folosim programul creat
    // (avand o scena simpla folosim un singur program)
    GLuint program = createProgram("obiect1.vert","obiect1.frag");
    glUseProgram(program);

    //cream scena
    Scena S = createScena(program);


    while(!glfwWindowShouldClose(fereastra))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // procesam intrarile ferestrei: din taste si/sau mouse
        processInput(fereastra);

        // trimitem matricea projection catre shadere
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program,"projection"), 1, GL_FALSE,&projection[0][0]);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        S.DrawScene();

        glfwSwapBuffers(fereastra);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

Scena createScena(GLuint program){

    Scena aux = Scena();
    // load models
    // -----------


    // TO DO: aici incarcati modelele voastre
    // fie luati datele dintr-un fisier, fie puneti datele aici


    // aici aveti un exemplu extrem de sumar cum puteti incarca si folosi doua obiecte.

    locatie l = {0.0, 0.0, 0.0};
    rotatie r = {0.0, 1.0, 1.0, 1.0};
    scalare s = {1.0f, 1.0f, 1.0f};


    aux.addObiect(Obiect(500, "resurse/pereti/untitled.obj", {0,0,0}, {0, 1, 0, 0}, s, program));
    // adaugam o rotatie acolo pentru a fi corect amplasat
    //aux.addRotatieObiect(500,{90, 1, 0, 0});



    // adaugam bordurile tablii de sah
    // un cub este de 2x2
    // incepem de la 2 si din 2 in 2 adaugam cate un cub
    for(int i = 2; i < 20; i+=2){
        // adaugam sirul |
        aux.addObiect(Obiect(500+i+1, aux.getObiect(500), {i,0,0}, {0, 0, 1, 0}, s, program));

        // adaugam sirul |_
        aux.addObiect(Obiect(600+i+1, aux.getObiect(500), {0,0,i}, {0, 0, 1, 0}, s, program));

        // adaugam sirul |_|
        aux.addObiect(Obiect(700+i+1, aux.getObiect(500), {i,0,18}, {0, 0, 1, 0}, s, program));

        // adaugam sirul |=|
        aux.addObiect(Obiect(800+i+1, aux.getObiect(500), {18,0,i}, {0, 0, 1, 0}, s, program));
    }



    // negru, alb, negru, alb primul rand

    // adaugam paralelipipedele unde vor sta piesele in tabla de joc
    // incepem de la 2 deoarece pe pozitiile 0 si 1 sunt cuburile/bordura tabli de joc
    // primul rand, al 3-lea, etc
    for(int j=2;j<=16;j+=4) {
        // adaugam piesele negre
        aux.addObiect(Obiect(7000-j, "resurse/pereti/rectangle.obj", {j,-0.5,2}, {90, 1, 0, 0}, s, program));
        aux.addRotatieObiect(7000-j,{90, 1, 0, 0});
        //
        aux.addObiect(Obiect(98998-j, "resurse/pereti/pionBlack.obj", {14,2,2}, {0, 1, 0, 0}, s, program));


        for(int i=6;i<=16;i+=4) {
            aux.addObiect(Obiect(7000-j+i, aux.getObiect(7000-j), {j,-0.5,i}, {90, 1, 0, 0}, s, program));
            aux.addRotatieObiect(7000-j+i,{90, 1, 0, 0});
            //
            aux.addObiect(Obiect(98998-j, aux.getObiect(98998-j), {14,2,i}, {0, 1, 0, 0}, s, program));

        }

        // adaugam piesele albe
        aux.addObiect(Obiect(7111-j, "resurse/pereti/rectangleWhite.obj", {j,-0.5,4}, {90, 1, 0, 0}, s, program));
        aux.addRotatieObiect(7111-j,{90, 1, 0, 0});
         //
        aux.addObiect(Obiect(98998-j, "resurse/pereti/pionBlack.obj", {14,2,4}, {0, 1, 0, 0}, s, program));


         for(int i=8;i<=16;i+=4) {
            aux.addObiect(Obiect(7111-j+i, aux.getObiect(7111-j), {j,-0.5,i}, {90, 1, 0, 0}, s, program));
            aux.addRotatieObiect(7111-j+i,{90, 1, 0, 0});
            //
            aux.addObiect(Obiect(98998-j+i, aux.getObiect(98998-j), {14,2,i}, {0, 1, 0, 0}, s, program));

        }
    }


    // alb, negru, alb, negru al 2-lea rand

    // adaugam paralelipipedele unde vor sta piesele in tabla de joc
    // incepem de la 2 deoarece pe pozitiile 0 si 1 sunt cuburile/bordura tabli de joc
    // al 2-lea rand, al 4-lea, etc
    for(int j=4;j<=16;j+=4) {
        // adaugam piesele albe + pionii
        aux.addObiect(Obiect(10-j, "resurse/pereti/rectangleWhite.obj", {j,-0.5,2}, {90, 1, 0, 0}, s, program));
        aux.addRotatieObiect(10-j,{90, 1, 0, 0});
        aux.addObiect(Obiect(100000-j, "resurse/pereti/pionWhite.obj", {4,2,2}, {0, 1, 0, 0}, s, program));

        for(int i=6;i<=16;i+=4) {
            aux.addObiect(Obiect(10-j+i, aux.getObiect(10-j), {j,-0.5,i}, {90, 1, 0, 0}, s, program));
            aux.addRotatieObiect(10-j+i,{90, 1, 0, 0});
            aux.addObiect(Obiect(100000-j+i, aux.getObiect(100000-j), {4,2,i}, {0, 1, 0, 0}, s, program));

        }

        // adaugam piesele negre + pionii
        aux.addObiect(Obiect(11-j, "resurse/pereti/rectangle.obj", {j,-0.5,4}, {90, 1, 0, 0}, s, program));
        aux.addRotatieObiect(11-j,{90, 1, 0, 0});
        aux.addObiect(Obiect(100000-j, "resurse/pereti/pionWhite.obj", {4,2,4}, {0, 1, 0, 0}, s, program));

         for(int i=8;i<=16;i+=4) {
            aux.addObiect(Obiect(11-j+i, aux.getObiect(11-j), {j,-0.5,i}, {90, 1, 0, 0}, s, program));
            aux.addRotatieObiect(11-j+i,{90, 1, 0, 0});
            aux.addObiect(Obiect(100000-j+i, aux.getObiect(100000-j), {4,2,i}, {0, 1, 0, 0}, s, program));
        }
    }


    // adaugam cei patru nebuni
    aux.addObiect(Obiect(10000, "resurse/pereti/nebun.obj", {3,1.7,4}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(10000, "resurse/pereti/nebun.obj", {3,1.7,14}, {0, 1, 0, 0}, s, program));

    aux.addObiect(Obiect(1012000, "resurse/pereti/nebun1.obj", {17,1.7,4}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(1000120, "resurse/pereti/nebun1.obj", {17,1.7,14}, {0, 1, 0, 0}, s, program));


    //adaugam cele patru turnuri
    aux.addObiect(Obiect(10000, "resurse/pereti/turn.obj", {2,0,2}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(10000, "resurse/pereti/turn.obj", {2,0,16}, {0, 1, 0, 0}, s, program));

    aux.addObiect(Obiect(1000, "resurse/pereti/turn1.obj", {16,0,2}, {0, 1, 90, 0}, s, program));
    aux.addObiect(Obiect(1000, "resurse/pereti/turn1.obj", {16,0,16}, {0, 1, 90, 0}, s, program));


    //adaugam cei patru cai
    aux.addObiect(Obiect(10000, "resurse/pereti/knight_horse3.obj", {-3,0,4.5}, {0, 1, 0, 90}, s, program));
    aux.addObiect(Obiect(10000, "resurse/pereti/knight_horse3.obj", {-3,0,10.5}, {0, 1, 0, 90}, s, program));

    aux.addObiect(Obiect(1000, "resurse/pereti/horse3.obj", {11,0,4.5}, {0, 1, 90, 0}, s, program));
    aux.addObiect(Obiect(1000, "resurse/pereti/horse3.obj", {11,0,10.5}, {0, 1, 90, 0}, s, program));


    //adaugam gogoasa cu glazura
    aux.addObiect(Obiect(10000, "resurse/pereti/donut4.obj", {-2,-1,-7}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(10000, "resurse/pereti/icing3.obj", {-2,-1.09,-7}, {0, 1, 0, 0}, s, program));

    aux.addObiect(Obiect(10000, "resurse/pereti/donut4.obj", {18,-1,-7}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(10000, "resurse/pereti/icing3.obj", {18,-1.09,-7}, {0, 1, 0, 0}, s, program));

    // adaugam canile
    aux.addObiect(Obiect(9954433652, "resurse/pereti/cana.obj", {4,-0.8,-8}, {0, 0, 120, 0}, s, program));
    aux.addObiect(Obiect(9954433652, "resurse/pereti/cana.obj", {14,-0.8,-8}, {0, 0, 120, 0}, s, program));


    // adaugam reginele si regii
    aux.addObiect(Obiect(100100, "resurse/pereti/regina.obj", {2,2,10}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(110000, "resurse/pereti/king_chess2.obj", {-0.5,0,7}, {0, 1, 0, 0}, s, program));

    aux.addObiect(Obiect(100100, "resurse/pereti/regina1.obj", {16,2,10}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(110000, "resurse/pereti/king.obj", {13.5,0,7}, {0, 1, 0, 0}, s, program));


    // adaugam masa
    aux.addObiect(Obiect(870000, "resurse/pereti/masa8.obj", {9,-0.9,7.5}, {0, 1, 0, 0}, s, program));


    // adaugam picioarele masei
    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {-4,-5.9,25}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {-4,-10.9,25}, {0, 1, 0, 0}, s, program));


    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {-4,-5.9,-10}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {-4,-10.9,-10}, {0, 1, 0, 0}, s, program));


    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {24,-5.9,25}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {24,-10.9,25}, {0, 1, 0, 0}, s, program));


    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {24,-5.9,-10}, {0, 1, 0, 0}, s, program));
    aux.addObiect(Obiect(970000, "resurse/pereti/piciorMasa.obj", {24,-10.9,-10}, {0, 1, 0, 0}, s, program));


    // adaugam cele 4 podele
    aux.addObiect(Obiect(995432, "resurse/pereti/podeaCamera.obj", {42,-15,5}, {90, 0, 1, 0}, s, program));
    aux.addObiect(Obiect(995432, "resurse/pereti/podeaCamera.obj", {42,-15,63.25}, {90, 0, 1, 0}, s, program));

    aux.addObiect(Obiect(995432, "resurse/pereti/podeaCamera.obj", {-16,-15,5}, {90, 0, 1, 0}, s, program));
    aux.addObiect(Obiect(995432, "resurse/pereti/podeaCamera.obj", {-16,-15,63.25}, {90, 0, 1, 0}, s, program));

    // adaugam cei 4 pereti
    aux.addObiect(Obiect(995431, "resurse/pereti/pereti.obj", {42,-15,5}, {90, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(995432, "resurse/pereti/pereti.obj", {42,-15,63.25}, {90, 0, 90, 0}, s, program));

    aux.addObiect(Obiect(995433, "resurse/pereti/pereti.obj", {13,-15,-24}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(995434, "resurse/pereti/pereti.obj", {-45,-15,-24}, {0, 0, 90, 0}, s, program));



    // adaugam vazele si piedestalele
    aux.addObiect(Obiect(99544334, "resurse/pereti/vase.obj", {36,0,-16}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(99544332, "resurse/pereti/piedestal.obj", {36,0,-16}, {0, 0, 90, 0}, s, program));

    aux.addObiect(Obiect(99544334, "resurse/pereti/vase.obj", {-16,0,-16}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(99544332, "resurse/pereti/piedestal.obj", {-16,0,-16}, {0, 0, 90, 0}, s, program));

    aux.addObiect(Obiect(99544334, "resurse/pereti/vase.obj", {36,0,50}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(99544332, "resurse/pereti/piedestal.obj", {36,0,50}, {0, 0, 90, 0}, s, program));


     // adaugam scaunele si pernele
    aux.addObiect(Obiect(8776, "resurse/pereti/wood_chair.obj", {-6,-6,10}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(8776, "resurse/pereti/wood_chair.obj", {24,-6,6}, {180, 0, 1, 0}, s, program));

    aux.addObiect(Obiect(8776, "resurse/pereti/wood_chair_pillow.obj", {-6,-6,10}, {0, 0, 90, 0}, s, program));
    aux.addObiect(Obiect(8776, "resurse/pereti/wood_chair_pillow.obj", {30,-6,10.5}, {0, 0, 90, 0}, s, program));


    return aux;
}


GLFWwindow* initializari()
{
    /*
        functia initializeaza bibliotecile glfw si glad, creaza o fereastra si
            o ataseaza  unui context OpenGL
    */

    //initializam glfw
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cream fereastra
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "scena_01", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    // facem ca aceasta fereastra sa fie contextul curent


    //atasam fereastra contextului opengl
    //glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<" nu s-a initializat biblioteca GLAD!";
    }


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    return window;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(20 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


GLuint createProgram(char* VSFile, char* FSFile)
{

    /*
        functia creaza un program shader folosind ca fisiere sursa VSFile si FSFile
        IN: numele fisierelor sursa
        OUT: aliasul programului shader
    */

    GLuint vertexShader, fragmentShader, shaderProgram;
    int success;
    char infoLog[512];


    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    if( 0 == vertexShader )
    {
        std::cout << "Error creating vertex shader." << std::endl;
        exit(1);
    }

    std::string shaderCode = readFile(VSFile);
    const char *codeArray = shaderCode.c_str();
    glShaderSource( vertexShader, 1, &codeArray, NULL );

    glCompileShader(vertexShader);

    // verficam daca s-a reusit compilarea codului

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if( 0 == fragmentShader )
    {
        std::cout << "Error creating fragment shader." << std::endl;
        exit(1);
    }


    shaderCode = readFile(FSFile);
    codeArray = shaderCode.c_str();
    glShaderSource( fragmentShader, 1, &codeArray, NULL );


    glCompileShader(fragmentShader);

    // se verifica compilarea codului

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //cream programul

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // se verifica procesul de link

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath;
        std::cerr << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
