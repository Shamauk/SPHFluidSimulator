#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "particle.hpp"
#include "sceneManager.hpp"

// "Particle-Based Fluid Simulation for Interactive Applications" by Müller et al.

// rendering projection parameters
const static int WINDOW_WIDTH = 800;
const static int WINDOW_HEIGHT = 600;
const static float VIEW_WIDTH = 1200;
const static float VIEW_HEIGHT = 900;

// solver parameters
const static glm::vec2 G(0.0, -9.81);          
const static float REST_DENS = 300.f;          
const static float PRESSURE_STIFFNESS = 2000.f;          
const static float H = 16.f;
const static float HSQ = H * H;		            
const static float MASS = 2.5f;		           
const static float VISC = 200.f;	           
const static float DT = 0.0007f;	           

// smoothing kernels defined in Müller and their gradients
// adapted to 2D per "SPH Based Shallow Water Simulation" by Solenthaler et al.
const static float POLY6 = 4.f / (M_PI * pow(H, 8.f));
const static float SPIKY_GRAD = -10.f / (M_PI * pow(H, 5.f));
const static float VISC_LAP = 40.f / (M_PI * pow(H, 5.f));

// simulation parameters
const static float EPS = H; // boundary epsilon
const static float BOUND_DAMPING = -0.5f;

// SceneManager
SceneManager sceneManager(VIEW_WIDTH, VIEW_HEIGHT, H);

void Integrate(void)
{
	for (auto &p : sceneManager.getParticles())
	{
		// forward Euler integration
        p.setVelocity(p.getVelocity() + DT * p.getForce() / p.getDensity());
        p.setPosition(p.getPosition() + DT * p.getVelocity());

        if (p.getPosition().x - EPS < 0.f)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(BOUND_DAMPING, 1));
            p.setPosition(glm::vec2(EPS, p.getPosition().y));
		}
		if (p.getPosition().x + EPS > VIEW_WIDTH)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(BOUND_DAMPING, 1));
            p.setPosition(glm::vec2(VIEW_WIDTH - EPS, p.getPosition().y));
		}
		if (p.getPosition().y - EPS < 0.f)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(1, BOUND_DAMPING));
            p.setPosition(glm::vec2(p.getPosition().x, EPS));
		}
		if (p.getPosition().y + EPS > VIEW_HEIGHT)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(1, BOUND_DAMPING));
            p.setPosition(glm::vec2(p.getPosition().x, VIEW_HEIGHT - EPS));
		}
	}
}

void ComputeDensityPressure(void)
{
	for (auto &pi : sceneManager.getParticles())
	{
        pi.setDensity(0.f);
		for (auto &pj : sceneManager.getParticles())
		{
            glm::vec2 rij = pj.getPosition() - pi.getPosition();
            float r2 = glm::dot(rij, rij);

			if (r2 < HSQ)
			{
				// this computation is symmetric
                pi.setDensity(pi.getDensity() + MASS * POLY6 * pow(HSQ - r2, 3.f));
			}
		}
        pi.setPressure(PRESSURE_STIFFNESS * (pi.getDensity() - REST_DENS));
	}
}

void ComputeForces(void)
{
	for (auto &pi : sceneManager.getParticles())
	{
        glm::vec2 fpress(0.f, 0.f);
        glm::vec2 fvisc(0.f, 0.f);
		for (auto &pj : sceneManager.getParticles())
		{
			if (&pi == &pj)
			{
				continue;
			}

            glm::vec2 rij = pj.getPosition() - pi.getPosition();
            float r = glm::length(rij);

			if (r < H)
			{
				// compute pressure force contribution
				fpress += -glm::normalize(rij) * MASS * (pi.getPressure() + pj.getPressure()) 
                    / (2.f * pj.getDensity()) * SPIKY_GRAD * pow(H - r, 3.f);
				// compute viscosity force contribution
				fvisc += VISC * MASS * (pj.getVelocity() - pi.getVelocity()) / pj.getDensity() * VISC_LAP * (H - r);
			}
		}
        glm::vec2 fgrav = G * MASS / pi.getDensity();
        pi.setForce(fpress + fvisc + fgrav);
	}
}

void Update(void)
{
	ComputeDensityPressure();
	ComputeForces();
	Integrate();
}

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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SPH Fluid Simulator", NULL, NULL);
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
    

    // Generate circle vertex and index data
    int circleSegments = 5;
    std::vector<float> circleVertices = generateCircleVertices(H / 2.0, circleSegments);
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

    glm::vec3 waterColor = glm::vec3(0.0,0.5,1.0);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(waterColor));

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);

    sceneManager.changeScene(0);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Run a step
        Update();

        glUseProgram(shaderProgram);
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
