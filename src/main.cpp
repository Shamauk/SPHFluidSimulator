#include <vector>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "fluid.h"

Fluid fluidSimulator;

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

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
    float windowSizeRatio = 0.75;
    float windowWidth = 800;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowWidth * windowSizeRatio, "SPH Fluid Simulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    fluidSimulator.setMinY(fluidSimulator.getMinY() * windowSizeRatio);
    fluidSimulator.setMaxY(fluidSimulator.getMaxY() * windowSizeRatio);

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
    

    // Generate circle vertex and index data
    int circleSegments = 22;
    std::vector<float> circleVertices = generateCircleVertices(fluidSimulator.getParticleRadius(), circleSegments);
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Camera settings
    glm::vec3 cameraPosition(0.0f, 0.0f, 3.5f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);

    // Projection settings
    float left = fluidSimulator.getMinX();
    float right = fluidSimulator.getMaxX();
    float bottom = fluidSimulator.getMinY();
    float top = fluidSimulator.getMaxY();
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    // Create view and projection matrices
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);
    glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);

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

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(fluidSimulator.getParticleColor()));


    fluidSimulator.createScene(2);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(shaderProgram);
        // Clear the screen to black
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Draw particles
        glBindVertexArray(VAO);

        for (const Particle& p : fluidSimulator.getParticles()) {
            glm::dvec3 position3D(p.position.x, p.position.y, 0.0);
            glm::mat4 model = glm::translate(glm::mat<4,4,double>(1.0), position3D);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Draw the circle
            glDrawElements(GL_TRIANGLES, circleIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Run a step
        fluidSimulator.updateParticleStates();
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