#include <vector>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "fluidSimulator.h"
#include "light.h"

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

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
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

// void drawRectangle(const glm::vec3& min, const glm::vec3& max, GLuint shaderProgram) {
//     float rectangleVertices[] = {
//         min.x, min.y, min.z,
//         min.x, max.y, min.z,
//         min.x, min.y, max.z,
//         min.x, max.y, max.z,
//         max.x, min.y, min.z,
//         max.x, max.y, min.z,
//     };

//     GLuint rectangleIndices[] = {
//         0, 3, 2,
//         0, 1, 3,
//         0, 4, 5,
//         0, 5, 1
//     };

//     // Create VAO, VBO, and EBO
//     GLuint VAO, VBO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     // Draw the rectangle
//     glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

//     // Clean up
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
// }

void drawRectangle(const glm::vec3& min, const glm::vec3& max, GLuint shaderProgram) {
    float rectangleVertices[] = {
        min.x, min.y, max.z,  // left-back-top vertex
        min.x, min.y, min.z,  // left-front-top vertex
        min.x, max.y, max.z,  // left-back-bottom vertex
        min.x, max.y, min.z   // left-front-bottom vertex
    };

    GLuint rectangleIndices[] = {
        0, 2, 1,  // left side triangle 1
        1, 2, 3   // left side triangle 2
    };

    // Create VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw the rectangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
        std::cout << "Failed to create window" << std::endl;
        // Terminate GLFW
        glfwTerminate();
        return 1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; glewInit();

    // Make ocean level
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < fluidSimulator.numXGrids; i++) {
            for (int k = 0; k < fluidSimulator.numZGrids; k++) {
                fluidSimulator.addParticleFromXYZ(fluidSimulator.minX + i * fluidSimulator.SMOOTHING_LENGTH, 
                    fluidSimulator.minY + j * (fluidSimulator.SMOOTHING_LENGTH + 0.001), fluidSimulator.minZ + k * fluidSimulator.SMOOTHING_LENGTH);
            }
        }
    }

    // Add a blob above
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 25; j++) {
            fluidSimulator.addParticleFromXYZ(-1.5+j*0.15, 2+i*0.15, 0);
        }
    }

    // Generate sphere vertex and index data
    float sphereRadius = fluidSimulator.SMOOTHING_LENGTH / 2.0;
    int sphereSegments = 12;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

     // Camera settings
    glm::vec3 cameraPosition(26.0f, 0.0f, 15.0f);
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

    const char* rectangleVertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* rectangleFragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec3 objectColor;

        void main() {
            FragColor = vec4(objectColor, 1.0);
        }
    )";

    GLuint rectangleShaderProgram = createShaderProgram(rectangleVertexShaderSrc, rectangleFragmentShaderSrc);
    glUseProgram(rectangleShaderProgram);

    // Set the model matrix for the rectangle
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(rectangleShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(rectangleShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(rectangleShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    const char* vertexShaderSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        out vec3 FragPos;
        out vec3 Normal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;

            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core

        in vec3 FragPos;
        in vec3 Normal;

        out vec4 FragColor;

        uniform vec3 lightPos;
        uniform vec3 lightColor;
        uniform vec3 viewPos;

        uniform vec3 objectColor;

        // Attenuation uniforms
        uniform float constant;
        uniform float linear;
        uniform float quadratic;

        void main() {
            // Ambient lighting
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * lightColor;

            // Diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            // Specular lighting
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;

            // Attenuation
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;

            // Final color calculation
            vec3 result = (ambient + diffuse + specular) * objectColor;
            FragColor = vec4(result, 1.0);
        }
    )";


    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    // Ligh creation
    Light light;
    light.color = glm::vec3(255.0f, 235.0f, 235.0f);
    light.position = cameraPosition;
    
    // Attenuation
    GLuint constantLoc = glGetUniformLocation(shaderProgram, "constant");
    GLuint linearLoc = glGetUniformLocation(shaderProgram, "linear");
    GLuint quadraticLoc = glGetUniformLocation(shaderProgram, "quadratic");

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    glUniform1f(constantLoc, constant);
    glUniform1f(linearLoc, linear);
    glUniform1f(quadraticLoc, quadratic);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(light.color));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPosition));

    glm::vec3 particleColor = glm::vec3(0.0f, 0.1f, 1.0f);
    glm::vec3 boundaryColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 boundaryMin(fluidSimulator.minX, fluidSimulator.minY, fluidSimulator.minZ);
    glm::vec3 boundaryMax(fluidSimulator.maxX, fluidSimulator.maxY, fluidSimulator.maxZ);

    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(particleColor));
    glUniform3fv(glGetUniformLocation(rectangleShaderProgram, "objectColor"), 1, glm::value_ptr(boundaryColor));

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Take time step
        fluidSimulator.updateParticles();

        glUseProgram(shaderProgram);
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

        // glUseProgram(rectangleShaderProgram);
        // drawRectangle(boundaryMin, boundaryMax, rectangleShaderProgram);
        // glUseProgram(shaderProgram);

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