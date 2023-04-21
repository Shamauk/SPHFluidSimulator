#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "managers/sceneManager.hpp"
#include "managers/simulatorManager.hpp"

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
std::vector<float> generateCircleVertices(float radius, int segments);
std::vector<unsigned int> generateCircleIndices(int segments);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void renderUI(float);
void initializeImgGui(GLFWwindow *window);

// PROJECTION PARAMS
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Play vars
int stepsToRun = 0;
bool runSimulation = false;
bool enableDebug = true;
bool enableErrorView = false;

// Managers
SceneManager sceneManager;
SimulatorManager simulatorManager;

GLuint shaderProgram;

glm::vec3 waterColor = glm::vec3(0.0,0.1,1.0);
glm::vec3 errorColor = glm::vec3(0.0,0.0,1.0);

glm::vec3 minDensityColor = glm::vec3(0.f, 0.f, 1.0f);
glm::vec3 maxDensityColor = glm::vec3(1.f, 0.f, 0.f);

glm::vec3 lerpColor(float t) {
    return minDensityColor * (1.0f - t) + maxDensityColor * t;
}

int main() {
    // Initialize GLFW
    glfwInit();

    // Define version and compatibility settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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
    simulatorManager.changeSimulator((short) 1, sceneManager.getViewWidth(), 
                sceneManager.getViewHeight(), sceneManager.getParticleRadius(), sceneManager.getBoundary());
    sceneManager.setupSceneConfig();
    
    // Set up UI
    initializeImgGui(window);

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
    glm::mat4 projectionMatrix = glm::ortho(0.f, sceneManager.getViewWidth(), 0.f, 
        sceneManager.getViewHeight(), -1.f, 1.f);

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


    shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);

    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glEnable(GL_DEPTH_TEST);

    double lastTime = glfwGetTime();

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        if (runSimulation || stepsToRun > 0) {
            stepsToRun = stepsToRun < 1 ? 0 : stepsToRun - 1;
            // Run a step
            sceneManager.update();
            simulatorManager.update(sceneManager.getParticles());
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const Particle& p : sceneManager.getParticles()) {
            glm::dvec3 position3D(p.getPosition(), 0.0);
            glm::mat4 model = glm::translate(glm::mat<4,4,double>(1.0), position3D);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            if (enableErrorView) {
                errorColor = lerpColor(p.getDensityError());
                glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, 
                    glm::value_ptr(errorColor));
            } else {
                glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(waterColor));
            }

            // Draw the circle
            glDrawElements(GL_TRIANGLES, circleIndices.size(), GL_UNSIGNED_INT, 0);
        }

        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - lastTime;
        lastTime = currentTime;
        float fps = 1.f / elapsedTime;
        renderUI(fps);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Clean up UI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureKeyboard) 
        return;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
        else if (key == GLFW_KEY_SPACE)
            runSimulation = !runSimulation;
        else if (key == GLFW_KEY_S)
            stepsToRun++;
        else if (key == GLFW_KEY_D)
            enableDebug = !enableDebug;
        else if (key == GLFW_KEY_E)
            enableErrorView = !enableErrorView;
        else if (key == GLFW_KEY_R) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.reset();
            sceneManager.setupSceneConfig();
        } else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && (mods & GLFW_MOD_SHIFT)) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.reset();
            simulatorManager.changeSimulator(key - GLFW_KEY_1 + 1, sceneManager.getPlayWidth(), 
                sceneManager.getPlayHeight(), sceneManager.getParticleRadius(), sceneManager.getBoundary());
            sceneManager.setupSceneConfig();
        } else if (key == GLFW_KEY_1 && (mods & GLFW_MOD_CONTROL)) {
            simulatorManager.setDiscretization(new BruteDiscretization(sceneManager.getPlayWidth(), 
                sceneManager.getPlayHeight(), simulatorManager.getKernelRange()));
        } else if (key == GLFW_KEY_2 && (mods & GLFW_MOD_CONTROL)) {
            simulatorManager.setDiscretization(new GridDiscretization(sceneManager.getPlayWidth(), 
                sceneManager.getPlayHeight(), simulatorManager.getKernelRange()));
        } else if (key == GLFW_KEY_3 && (mods & GLFW_MOD_CONTROL)) {
            simulatorManager.setDiscretization(new CompactHashingDiscretization(sceneManager.getPlayWidth(), 
                sceneManager.getPlayHeight(), simulatorManager.getKernelRange()));
        } else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
            runSimulation = false;
            stepsToRun = 0;
            sceneManager.changeScene(key - GLFW_KEY_1 + 1);
            simulatorManager.changeSimulator(simulatorManager.getID(), sceneManager.getPlayWidth(), 
                sceneManager.getPlayHeight(), sceneManager.getParticleRadius(), sceneManager.getBoundary());
            sceneManager.setupSceneConfig();
            glUseProgram(shaderProgram);
            glm::mat4 projectionMatrix = glm::ortho(0.f, sceneManager.getViewWidth(), 
                0.f, sceneManager.getViewHeight(), -1.f, 1.f);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        } 
    }
}

void initializeImgGui(GLFWwindow *window) {
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

std::string formatMemorySize(size_t sizeInBytes) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);

    if (sizeInBytes < 1024) {
        ss << sizeInBytes << " bytes";
    } else if (sizeInBytes < 1024 * 1024) {
        ss << static_cast<float>(sizeInBytes) / 1024 << " KB";
    } else {
        ss << static_cast<float>(sizeInBytes) / (1024 * 1024) << " MB";
    }

    return ss.str();
}

void renderUI(float fps) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec4 helperTextColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

    ImGui::Begin("Configuration");
    if (ImGui::CollapsingHeader("Debug")) { 
        if (enableDebug) {
            ImGui::Text("Currently running: %s", runSimulation ? "YES" : "NO");
            ImGui::TextColored(helperTextColor, "Press SPACE to toggle");
            ImGui::Text("Error view enabled: %s", enableErrorView ? "YES" : "NO");
            ImGui::TextColored(helperTextColor, "Press E to toggle");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Discretization Memory Usage: %s", 
                formatMemorySize(simulatorManager.getDiscretizationMemoryUsage()).c_str());
            ImGui::TextColored(helperTextColor, "Press D to disable debugging");
        } else {
            ImGui::TextColored(helperTextColor, "Press D to enable debugging");
        }
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Simulator Information")) {
        ImGui::Text("Using Simulator: %s", simulatorManager.getName().c_str());
        ImGui::TextColored(helperTextColor, "Press SHIFT + 1-9 to change simulator");
        ImGui::Separator();
        ImGui::Text("Using Discretization Method: %s", simulatorManager.getDiscretizationName().c_str());
        ImGui::TextColored(helperTextColor, "Press CTRL + 1-9 to change discretization technique");
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Scene Information")) {
        ImGui::Text("Running Scene: %s", sceneManager.getName().c_str());
        ImGui::TextColored(helperTextColor, "Press 1-9 to change scenes");
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Simulator Parameters")) {
        float totalHeight = simulatorManager.getParameters().size() * 1.5f * ImGui::GetTextLineHeightWithSpacing();

        ImGui::BeginChildFrame(ImGui::GetID("SimulatorParametersFrame"), ImVec2(0, totalHeight));
        for (auto& parameter : simulatorManager.getParameters()) {
            ImGui::InputFloat(parameter.name.c_str(), parameter.value, parameter.min, parameter.max, "%.6f");
        }
        ImGui::EndChildFrame();
    }

    if (ImGui::CollapsingHeader("Scene Parameters")) {
        float totalHeight = sceneManager.getParameters().size() * 1.5f * ImGui::GetTextLineHeightWithSpacing();

        ImGui::BeginChildFrame(ImGui::GetID("SceneParametersFrame"), ImVec2(0, totalHeight));
        for (auto& parameter : sceneManager.getParameters()) {
            ImGui::InputFloat(parameter.name.c_str(), parameter.value, parameter.min, parameter.max, "%.6f");
        }
        ImGui::EndChildFrame();
    }

    if (ImGui::CollapsingHeader("Additional Keyboard Controls")) {
        ImGui::TextColored(helperTextColor, "S - Run a single step");
        ImGui::TextColored(helperTextColor, "R - Reset scene");
        ImGui::TextColored(helperTextColor, "ESCAPE - Close simulation");
    }

    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

