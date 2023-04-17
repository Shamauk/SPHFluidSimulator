#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "particle.hpp"
#include "sceneManager.hpp"
#include "simulatorManager.hpp"

// PROJECTION PARAMS
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float VIEW_WIDTH = 1200;
const float VIEW_HEIGHT = 900;

// Play vars
int stepsToRun = 0;
bool runSimulation = false;

// SceneManager
SimulatorManager simulatorManager(VIEW_WIDTH, VIEW_HEIGHT);
SceneManager sceneManager(VIEW_WIDTH, VIEW_HEIGHT);

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
std::vector<float> generateCircleVertices(float radius, int segments);
std::vector<unsigned int> generateCircleIndices(int segments);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
// void processInput(GLFWwindow *window);

int main() {
    // Initialize GLFW
    glfwInit();

    // Define version and compatibility settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create OpenGL window and context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPH Fluid Simulator", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // Check for window creation failure
    if (!window) 
    {
        std::cout << "Failed to create window" << std::endl;
        // Terminate GLFW
        glfwTerminate();
        return 1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; glewInit();

    // Setup managers
    sceneManager.changeScene((short) 1);
    simulatorManager.changeSimulator((short) 1, sceneManager.getParticleRadius());
    
    // Generate circle vertex and index data
    int circleSegments = 5;
    std::vector<float> circleVertices = generateCircleVertices(sceneManager.getParticleRadius(), circleSegments);
    std::vector<unsigned int> circleIndices = generateCircleIndices(circleSegments);

    // Create VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleIndices.size() * sizeof(unsigned int), circleIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create view and projection matrices
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::ortho(0.f, VIEW_WIDTH, 0.f, VIEW_HEIGHT, -1.f, 1.f);

    const char* vertexShaderSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core

        out vec4 FragColor;

        uniform vec3 objectColor;

        void main() {
            FragColor = vec4(objectColor, 1.0);
        }
    )";


    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);

    glm::vec3 waterColor = glm::vec3(0.0,0.1,1.0);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(waterColor));

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        
        if (runSimulation || stepsToRun > 0) {
            stepsToRun = stepsToRun < 1 ? 0 : stepsToRun - 1;
            // Run a step
            sceneManager.update();
            simulatorManager.update(sceneManager.getParticles());
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const Particle& p : sceneManager.getParticles()) {
            glm::dvec3 position3D(p.getPosition(), 0.0);
            glm::mat4 model = glm::translate(glm::mat<4,4,double>(1.0), position3D);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Draw the circle
            glDrawElements(GL_TRIANGLES, circleIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

std::vector<float> generateCircleVertices(float radius, int segments) {
    std::vector<float> vertices;

    // Add the center vertex
    vertices.push_back(0.0f); 
    vertices.push_back(0.0f); 
    vertices.push_back(0.0f); 

    // Add vertices for the circle perimeter
    for (int i = 0; i <= segments; ++i) {
        float angle = (2.0f * M_PI * i) / static_cast<float>(segments);
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        vertices.push_back(x);       
        vertices.push_back(y);       
        vertices.push_back(0.0f);    
    }

    return vertices;
}

std::vector<unsigned int> generateCircleIndices(int segments) {
    std::vector<unsigned int> indices;

    for (int i = 0; i < segments; ++i) {
        indices.push_back(0); 
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    // Close the last triangle
    indices.push_back(0); 
    indices.push_back(segments + 1);
    indices.push_back(1);

    return indices;
}


GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
//     else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//         runSimulation = !runSimulation;
//     else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         stepsToRun++;
//     else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT) {
//         for (int i = 0; i < 9; i++)
//         {
//             int key = GLFW_KEY_1 + i;
//             if (glfwGetKey(window, key) == GLFW_PRESS)
//             {
//                 runSimulation = false;
//                 stepsToRun = 0;
//                 simulatorManager.changeSimulator(i+1, sceneManager.getParticleRadius());
//             }
//         }
//     } else {
//         for (int i = 0; i < 9; i++)
//         {
//             int key = GLFW_KEY_1 + i;
//             if (glfwGetKey(window, key) == GLFW_PRESS)
//             {
//                 runSimulation = false;
//                 stepsToRun = 0;
//                 sceneManager.changeScene(i+1);
//             }
//         }
//     }
// }

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
        else if (key == GLFW_KEY_SPACE)
            runSimulation = !runSimulation;
        else if (key == GLFW_KEY_S)
            stepsToRun++;
        else if (key == GLFW_KEY_R) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.reset();
       } else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && (mods & GLFW_MOD_SHIFT)) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.reset();
            simulatorManager.changeSimulator(key - GLFW_KEY_1 + 1, sceneManager.getParticleRadius());
        } else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.changeScene(key - GLFW_KEY_1 + 1);
        } 
    }
}

