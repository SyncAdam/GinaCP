#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaT);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
//cameraDirection actually points towards the camera
glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
glm::vec3 lightPosition = glm::vec3(1.0f, 2.0f, 2.0f);

float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraFOV = 45.0f;

    float lastXPos = SCR_WIDTH / 2;
    float lastYPos = SCR_HEIGHT / 2;


int main()
{

	unsigned int VBO;  //vertex buffer object
	unsigned int VAO;  //vertex array object
    unsigned int lightVAO;

    //------------------------------

    //glfw initialization and configuration
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gina", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //callback function for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    //------------------------------


    //------------------------------


    Shader ginaShader("./shader.vs", "./shader.fs");
    Shader lightShader("./shader.vs", "./lightshader.fs");

	//generate vertex buffer object for storing vertex data in gpu memory, vertex array object and element array object
	glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO); //generate buffer


    //bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attributes.
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer to gl array buffer target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer to gl array buffer target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


	//glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

	//wireframe mode gl_line/gl-fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    ginaShader.use();
    ginaShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    ginaShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    ginaShader.setVec3("lightPos", lightPosition);


    //------------------------------

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(cameraFOV), 800.0f / 600.0f, 0.1f, 100.0f);
        ginaShader.setMat4("projection", projection);

    //------------------------------


    //render loop

    float deltaT = 0.0f;
    float lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        deltaT = glfwGetTime() - lastFrame;
        std::cout << 1/deltaT << std::endl;
        lastFrame = glfwGetTime();

        processInput(window, deltaT);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ginaShader.use();

        lightPosition = glm::vec3(2*sin((float)glfwGetTime()), 2*cos((float)glfwGetTime()), 1.0f);

        ginaShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ginaShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ginaShader.setVec3("lightPos", lightPosition);

        //cameraDirection actually points towards the camera
        cameraDirection = glm::normalize(cameraPosition - cameraTarget);
        cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
        cameraUp = glm::cross(cameraDirection, cameraRight);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(cameraFOV), 800.0f / 600.0f, 0.1f, 100.0f);
        ginaShader.setMat4("projection", projection);

        view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

        //float angle = (1) * 10 * (float)glfwGetTime(); 
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));

        ginaShader.setMat4("model", model);
        ginaShader.setMat4("view", view);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

      
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        glm::mat4 light = glm::mat4(1.0f);
        light = glm::translate(light, lightPosition);
        light = glm::scale(light, glm::vec3(0.3f));
        lightShader.setMat4("model", light);
        lightShader.setVec3("lightPos", lightPosition);

        glBindVertexArray(lightVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaT)
{
    const float cameraSpeed = 2.0f * deltaT;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraTarget;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraTarget;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)));
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xOffset = xpos - lastXPos;
    float yOffset = ypos - lastYPos;

    lastXPos = xpos;
    lastYPos = ypos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    cameraYaw += xOffset;
    cameraPitch -= yOffset;

    if(cameraPitch > 89.0f) cameraPitch = 89.0f;
    if(cameraPitch < -89.0f) cameraPitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraTarget = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* windows, double xoffset, double yoffset)
{

    cameraFOV -= (float) yoffset;
    if(cameraFOV > 45.0f) cameraFOV = 45.0f;
    if(cameraFOV < 5.0f) cameraFOV = 5.0f;
}
