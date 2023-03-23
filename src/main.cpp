#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "fluidSimulator.h"

FluidSimulator fluidSimulator;

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
glm::mat4 createViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& target, const glm::vec3& upVector);
glm::mat4 createPerspectiveMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

std::vector<float> generateSphereVertices(float radius, int segments) {
    std::vector<float> vertices;
    for (int i = 0; i <= segments; i++) {
        float v = i / static_cast<float>(segments);
        float phi = v * M_PI;
        for (int j = 0; j <= segments; j++) {
            float u = j / static_cast<float>(segments);
            float theta = u * 2.0f * M_PI;

            float x = radius * std::cos(theta) * std::sin(phi);
            float y = radius * std::sin(theta) * std::sin(phi);
            float z = radius * std::cos(phi);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    return vertices;
}

std::vector<unsigned int> generateSphereIndices(int segments) {
    std::vector<unsigned int> indices;
    for (int i = 0; i < segments; i++) {
        for (int j = 0; j < segments; j++) {
            int a = i * (segments + 1) + j;
            int b = a + segments + 1;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(a + 1);

            indices.push_back(b);
            indices.push_back(b + 1);
            indices.push_back(a + 1);
        }
    }
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "SPH Fluid Simulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Check for window creation failure
    if (!window) 
    {
        // Terminate GLFW
        glfwTerminate();
        return 1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; glewInit();

    // Creating particles
    fluidSimulator.addParticleFromXYZWithRestDensity(0.0, 0.0, 0.0, 1);
    fluidSimulator.addParticleFromXYZWithRestDensity(-1.5, 0.0, 0.0, 1);
    fluidSimulator.addParticleFromXYZWithRestDensity(1.5, 0.0, 0.0, 1);
    fluidSimulator.addParticleFromXYZWithRestDensity(1.0, 0.25, 0.0, 1);

    // Generate sphere vertex and index data
    float sphereRadius = 0.1f;
    int sphereSegments = 16;
    std::vector<float> sphereVertices = generateSphereVertices(sphereRadius, sphereSegments);
    std::vector<unsigned int> sphereIndices = generateSphereIndices(sphereSegments);

    // Create VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* vertexShaderSrc = R"(
        #version 150 core
        in vec3 position;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main()
        {
            gl_Position = projection * view * model * vec4(position, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 150 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(0.0, 0.4, 1.0, 1.0); // Watery blue color
        }
    )";

    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);

    // Camera settings
    glm::vec3 cameraPosition(0.0f, 0.0f, 25.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);

    // Projection settings
    float fieldOfView = 45.0f;
    float aspectRatio = 800.0f / 600.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    // Create view and projection matrices
    glm::mat4 viewMatrix = createViewMatrix(cameraPosition, cameraTarget, upVector);
    glm::mat4 projectionMatrix = createPerspectiveMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);


    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Take time step
        fluidSimulator.updateParticles();

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Draw particles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        for (const Particle::Particle& p : fluidSimulator.particles) {
            // Create model matrix for the particle
            glm::mat4 model = glm::translate(glm::mat<4,4,double>(1.0), p.position);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Draw the sphere
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
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

glm::mat4 createViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& target, const glm::vec3& upVector) {
    return glm::lookAt(cameraPosition, target, upVector);
}

glm::mat4 createPerspectiveMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    return glm::perspective(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
}