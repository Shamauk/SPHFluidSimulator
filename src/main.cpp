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

std::vector<float> generateBoundaryVertices(glm::vec3 min, glm::vec3 max) {
    std::vector<float> vertices;

    // Left boundary
    vertices.push_back(min.x);
    vertices.push_back(min.y);
    vertices.push_back(min.z);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(min.x);
    vertices.push_back(max.y);
    vertices.push_back(min.z);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(min.x);
    vertices.push_back(min.y);
    vertices.push_back(max.z);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(min.x);
    vertices.push_back(max.y);
    vertices.push_back(max.z);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Right boundary
    vertices.push_back(min.x);
    vertices.push_back(min.y);
    vertices.push_back(min.z);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(min.x);
    vertices.push_back(max.y);
    vertices.push_back(min.z);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(max.x);
    vertices.push_back(min.y);
    vertices.push_back(min.z);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(max.x);
    vertices.push_back(max.y);
    vertices.push_back(min.z);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    return vertices;
}

std::vector<unsigned int> generateBoundaryIndices() {
    std::vector<unsigned int> indices;
    // Left boundary
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);

    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(5);

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
        std::cout << "Failed to create window" << std::endl;
        // Terminate GLFW
        glfwTerminate();
        return 1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; glewInit();


    // Let us add force to the bottom back layer


    // Add a large water drop
    // for (int i = 0; i < 5; i++) {
    //     for (int j = 0; j < 5; j++) {
    //         for (int k = 0; k < 5; k++) {
    //             fluidSimulator.addParticleFromXYZ(i*fluidSimulator.SMOOTHING_LENGTH/2, 
    //                 j*fluidSimulator.SMOOTHING_LENGTH/2, k*fluidSimulator.SMOOTHING_LENGTH/2);
    //         }
    //     }
    // }

    // Generate sphere vertex and index data
    float sphereDiameter = fluidSimulator.PARTICLE_RADIUS * 2.0;
    int sphereSegments = 6;
    std::vector<float> sphereVertices = generateSphereVertices(fluidSimulator.PARTICLE_RADIUS, sphereSegments);
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

    // Same thing for boundary
    glm::vec3 boundaryMin = glm::vec3(fluidSimulator.minX, fluidSimulator.minY, fluidSimulator.minZ);
    glm::vec3 boundaryMax = glm::vec3(fluidSimulator.maxX, fluidSimulator.maxY, fluidSimulator.maxZ);
    std::vector<float> boundaryVertices = generateBoundaryVertices(boundaryMin, boundaryMax);
    std::vector<unsigned int> boundaryIndices = generateBoundaryIndices();

    GLuint boundaryVAO, boundaryVBO, boundaryEBO;
    glGenVertexArrays(1, &boundaryVAO);
    glGenBuffers(1, &boundaryVBO);
    glGenBuffers(1, &boundaryEBO);

    glBindVertexArray(boundaryVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boundaryVBO);
    glBufferData(GL_ARRAY_BUFFER, boundaryVertices.size() * sizeof(float), boundaryVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundaryEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundaryIndices.size() * sizeof(unsigned int), boundaryIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

     // Camera settings
    glm::vec3 cameraPosition(4.0f, 0.0f, 3.5f);
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
    light.position = glm::vec3(26.0f, 15.0f, 30.0f);
    
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
    glm::vec3 boundaryColor = glm::vec3(0.25f, 0.25f, 0.25f);

    glm::mat4 identityMatrix = glm::mat4(1.0f);


    // Generate scene
    // Make ocean level
    // for (int j = 0; j < 5; j++) {
    //     for (int i = 0; i < fluidSimulator.numXGrids; i++) {
    //         for (int k = 0; k < fluidSimulator.numZGrids; k++) {
    //             Particle::Particle particle = Particle::ParticlefromXYZ(fluidSimulator.minX + i * fluidSimulator.SMOOTHING_LENGTH, 
    //                 fluidSimulator.minY + j * sphereDiameter, fluidSimulator.minZ + k * fluidSimulator.SMOOTHING_LENGTH);
    //             if (k == 0 && j == 0) {
    //                 particle.initialForce = glm::dvec3(0.0, 0.0, 1000.0); // Add sea floor force
    //             }
    //             fluidSimulator.addParticle(particle);
    //         }
    //     }
    // }

    int numInX = (fluidSimulator.maxX - fluidSimulator.minX) / fluidSimulator.PARTICLE_RADIUS;
    int numInZ = (fluidSimulator.maxZ - fluidSimulator.minZ) / fluidSimulator.PARTICLE_RADIUS;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < numInX; i++) {
            for (int k = 0; k < numInZ; k++) {
                Particle::Particle particle = Particle::ParticlefromXYZ(fluidSimulator.minX + i * fluidSimulator.PARTICLE_RADIUS, 
                    fluidSimulator.minY + j * fluidSimulator.PARTICLE_RADIUS, fluidSimulator.minZ + k * fluidSimulator.PARTICLE_RADIUS);
                if (k == 0 && j == 0) {
                    particle.initialForce = glm::dvec3(0.0, 0.0, 1000.0); // Add sea floor force
                }
                fluidSimulator.addParticle(particle);
            }
        }
    }

    // int iteration = 0;
    // int spewRate = 100;
    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Fountain
        // if (iteration % spewRate == 0) {
        //     Particle::Particle particle = Particle::ParticlefromXYZ(0.0,0.0,0.0);
        //     if (iteration % (4 * spewRate) == 0) {
        //         particle.velocity = glm::dvec3(3.0,0.0,0.0);
        //     } else if (iteration % (3 * spewRate) == 0) {
        //         particle.velocity = glm::dvec3(0.0,0.0,-3.0);
        //     } else if (iteration % (2 * spewRate) == 0) {
        //         particle.velocity = glm::dvec3(-3.0,0.0,0.0);
        //     } else {
        //         particle.velocity = glm::dvec3(0.0,0.0,3.0);
        //     }
        //     fluidSimulator.addParticle(particle);
        // }
        //  iteration++;


        // Take time step
        fluidSimulator.updateParticles();

        glUseProgram(shaderProgram);
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Draw particles
        glBindVertexArray(VAO);

        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(particleColor));
        for (const Particle::Particle& p : fluidSimulator.particles) {
           // std::cout << "Particle at: (" << p.position.x << "," << p.position.y << "," << p.position.z << ")" << std::endl;
            // Create model matrix for the particle
            glm::mat4 model = glm::translate(glm::mat<4,4,double>(1.0), p.position);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Draw the sphere
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        }

        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(boundaryColor));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(identityMatrix));
        glBindVertexArray(boundaryVAO);
        glDrawElements(GL_TRIANGLES, boundaryIndices.size(), GL_UNSIGNED_INT, 0);

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