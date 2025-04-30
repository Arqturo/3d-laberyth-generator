#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GraphMesh.h"
#include "Player.h"
#include "shader_m.h"
#include "camera.h"
#include <cstdlib>
#include <ctime>
#include <tuple>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Player &player);
void spawnCube(float x, float y, float z, int wallConfig, float textureIndex, float special_wall, float texture_floor_index, Shader &shader);
void spawnSquare(float textureIndex, Shader &shader, const glm::mat4 &view, const Camera &camera);
int **generateRandomWall(int height, int anchor);
int **initArray(int height, int anchor, int value);
glm::vec3 getCoordinates(int height, int anchor, float startX, float startY, float startZ, int row, int column);
void spawn_labyrinth(int **wall_color, int **floor_color_special, int **wall_color_special , int **diagram, int height, int anchor, float startX, float startY, float startZ, Shader &shader);
std::vector<int> generateNumberList(int start, int end);
void fillArrayWithRandomValues(int **array,std::vector<int>& numbers);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera cameratwo(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool victory_flag = false;

float hero = 11;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int p_end_x = 0;
int p_end_y = 0;
int **array_to_use;

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Build and compile shaders
    Shader ourShader("shader.vs", "shader.fs");
    Shader squareShader("square_shader.vs", "shader.fs");

    // Update the total number of textures
    const int NUM_TEXTURES = 16;

    // Array of texture paths
    std::string texturePaths[NUM_TEXTURES] = {
        "resources/textures/portal_floor.jpg",
        "resources/textures/wall1.png",
        "resources/textures/wall2.png",
        "resources/textures/wall3.png",
        "resources/textures/wall4.png",
        "resources/textures/wall5.png",
        "resources/textures/trapdoor.png",
        "resources/textures/grafiti.jpg",
        "resources/textures/glados.jpg",
        "resources/textures/chell.jpg",
        "resources/textures/cake.jpg",
        "resources/textures/CAKKE.jpg",
        };

    // Load textures from file paths
    unsigned int textures[NUM_TEXTURES];

    // Generate and load textures
    for (unsigned int i = 0; i < NUM_TEXTURES; ++i)
    {
        glGenTextures(1, &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        // Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load texture image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // Flip loaded texture's on the y-axis
        unsigned char *data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // Generate texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture: " << texturePaths[i] << std::endl;
            std::cout << "Error: " << stbi_failure_reason() << std::endl; // Print STB Image error
        }
        stbi_image_free(data);
    }

    // Set texture units
    ourShader.use();
    ourShader.setTexture("textures", new int[NUM_TEXTURES]{0, 1, 2, 3, 4, 5, 6,7,8,9,10,11,12,13,14,15}, textures, NUM_TEXTURES);

    squareShader.use();
    squareShader.setTexture("textures", new int[NUM_TEXTURES]{0, 1, 2, 3, 4, 5, 6,7,8,9,10,11,12,13,14,15}, textures, NUM_TEXTURES);

    GraphMesh graph(10, 10);
    graph.fullrandom();
    GraphMesh newgraph = graph.kruskal();
    int **diagram = newgraph.diagram();
    int **test = initArray(10, 10, 0);
    int la_height = 10;
    int la_anchor = 10;
    int **wall_color = generateRandomWall(10, 10);
    int end = newgraph.getRandomSideNodeIndex();
    int start = newgraph.farthestVertex(end);
    int **wall_color_special = initArray(10, 10, 0);
    int **floor_color_special = initArray(10, 10, 0);

    std::vector<int> special_graffitis = generateNumberList(8, 10);

    fillArrayWithRandomValues(wall_color_special, special_graffitis);

    array_to_use = diagram;
    newgraph.show();

    pair p_end = newgraph.getCoordinatesFromIndex(end, 10);
    p_end_x = p_end.first;
    p_end_y = p_end.second;
    pair p_start = newgraph.getCoordinatesFromIndex(start, 10);
    int p_start_x = p_start.first;
    int p_start_y = p_start.second;
    glm::vec3 coordinates = getCoordinates(la_height, la_anchor, 0.0f, 0.0f, -5.0f, p_start_x, p_start_y);

    camera = Camera(coordinates);
    Player player(p_start_x, p_start_y, array_to_use);

    floor_color_special[p_end_x][p_end_y] = 6;

    // Render loop
   while (!glfwWindowShouldClose(window))
{
    // Per-frame time logic
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Input handling
    processInput(window, player);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Laberythm
    {
        // Use the shader program
        ourShader.use();

        // Pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        if (victory_flag == false) {
        spawn_labyrinth(wall_color, floor_color_special, wall_color_special , array_to_use, la_height, la_anchor, 0.0f, 0.0f, -5.0f, ourShader);
        }

    }

    // HUD
    {
        // Use the shader program
        ourShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(cameratwo.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cameratwo.GetViewMatrix();

        // Calculate square position with adjustment for camera's orientation
        glm::vec3 squareOffset = glm::vec3(0.0f, 0.0f, -2.0f);                                          // Offset the square behind the camera
        glm::vec3 squarePosition = cameratwo.Position + cameratwo.Front * squareOffset - cameratwo.Right * 1.0f; // Adjust position based on camera's right vector

        // Use the square shader program
        squareShader.use();
        squareShader.setMat4("view", view);
        squareShader.setMat4("projection", projection);
        squareShader.setVec3("squarePosition", squarePosition); // Pass the square's position as a uniform

        // Render the square
        if (victory_flag == true) {
        spawnSquare(hero, squareShader, view, cameratwo);
        }
    }

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
}



    // Cleanup textures
    for (unsigned int i = 0; i < NUM_TEXTURES; ++i)
    {
        glDeleteTextures(1, &textures[i]);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void pushVertex(float x, float y, float z, float u, float v, float textureIndex, float *vertices, int &index)
{
    vertices[index++] = x;
    vertices[index++] = y;
    vertices[index++] = z;
    vertices[index++] = u;
    vertices[index++] = v;
    vertices[index++] = textureIndex;
}


void spawnCube(float x, float y, float z, int wallConfig, float textureIndex, float special_wall, float texture_floor_index, Shader &shader)
{
    float vertices[216]; // 36 vertices * 6 elements per vertex
    bool special_used = false;
    int actual_wall = 0;

    if (special_wall != 0) {
        actual_wall = special_wall;
    }

    int index = 0; 

    /* south start*/

    if (wallConfig == 2 || wallConfig == 4 || wallConfig == 6 || wallConfig == 8 || wallConfig == 10 || wallConfig == 12 || wallConfig == 14 || wallConfig == 0)
    {
            if (special_used == false && special_wall != 0) {
                actual_wall = special_wall;
                special_used = true;
            } else {
                actual_wall = textureIndex;
            }

        pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
    }

    /* south end*/

    /* north start*/
    if (wallConfig == 1 || wallConfig == 4 || wallConfig == 5 || wallConfig == 8 || wallConfig == 9 || wallConfig == 12 || wallConfig == 13 || wallConfig == 0)
    {
                    if (special_used == false && special_wall != 0) {
                actual_wall = special_wall;
                special_used = true;
            } else {
                actual_wall = textureIndex;
            }

        pushVertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
    }

    /* north end*/

    /* east start*/
    if (wallConfig == 1 || wallConfig == 2 || wallConfig == 3 || wallConfig == 8 || wallConfig == 9 || wallConfig == 10 || wallConfig == 11 || wallConfig == 0)
    {
                    if (special_used == false && special_wall != 0) {
                actual_wall = special_wall;
                special_used = true;
            } else {
                actual_wall = textureIndex;
            }

        pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
    }

    /* east end*/

    /* west start*/
    if (wallConfig < 8 || wallConfig == 0)
    {
                    if (special_used == false && special_wall != 0) {
                actual_wall = special_wall;
                special_used = true;
            } else {
                actual_wall = textureIndex;
            }

        pushVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, actual_wall, vertices, index);
        pushVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, actual_wall, vertices, index);
    }
    /* west end*/

    /* floor*/

    pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, texture_floor_index, vertices, index);
    pushVertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, texture_floor_index, vertices, index);
    pushVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, texture_floor_index, vertices, index);
    pushVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, texture_floor_index, vertices, index);
    pushVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, texture_floor_index, vertices, index);
    pushVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, texture_floor_index, vertices, index);

    /* floor end*/

    pushVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0, vertices, index);
    pushVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0, vertices, index);
    pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0, vertices, index);
    pushVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0, vertices, index);
    pushVertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, vertices, index);
    pushVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0, vertices, index);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture index attribute
    glVertexAttribIPointer(2, 1, GL_INT, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Draw cube
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void spawnSquare(float textureIndex, Shader &shader, const glm::mat4 &view, const Camera &camera) {
    float vertices[] = {
        // Positions           // Texture Coords   // Texture Index
        -1.2f, -1.2f, 0.0f,    0.0f, 0.0f,        textureIndex,   // Bottom Left
         1.2f, -1.2f, 0.0f,    1.0f, 0.0f,        textureIndex,   // Bottom Right
         1.2f,  1.2f, 0.0f,    1.f, 1.0f,        textureIndex,   // Top Right
        -1.2f,  1.2f, 0.0f,    0.0f, 1.0f,        textureIndex    // Top Left 
    };

    unsigned int indices[] = {
        0, 1, 2,  // First Triangle
        2, 3, 0   // Second Triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and set vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind EBO and set index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture index attribute
    glVertexAttribIPointer(2, 1, GL_INT, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Draw square
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 squareOffset = glm::vec3(0.0f, 0.0f, 0.0f); 
    glm::vec3 squarePosition = camera.Position + camera.Front * 2.0f; 
    model = glm::translate(model, squarePosition); 
    shader.setMat4("model", model);
    shader.setMat4("view", view); 
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}



int **generateRandomWall(int height, int anchor)
{
    int **array = new int *[height];
    srand(time(NULL));

    for (int i = 0; i < height; ++i)
    {
        array[i] = new int[anchor];
        for (int j = 0; j < anchor; ++j)
        {
            array[i][j] = rand() % 5 + 1; // Generate a random number between 1 and 5
        }
    }
    return array;
}

int **initArray(int height, int anchor, int value)
{
    int **array = new int *[height];
    srand(time(NULL));

    for (int i = 0; i < height; ++i)
    {
        array[i] = new int[anchor];
        for (int j = 0; j < anchor; ++j)
        {
            array[i][j] = value;
        }
    }
    return array;
}

glm::vec3 getCoordinates(int height, int anchor, float startX, float startY, float startZ, int row, int column)
{
    float x = startX + column * 2.0f;
    float y = startY;
    float z = startZ - row * 2.0f;
    return glm::vec3(x, y, z);
}

void spawn_labyrinth(int **wall_color, int **floor_color_special, int **wall_color_special, int **diagram, int height, int anchor, float startX, float startY, float startZ, Shader &shader)
{
    float x = startX;
    float y = startY;
    float z = startZ;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < anchor; ++j)
        {
            float textureIndex = wall_color[i][j];
            float floorIndex = floor_color_special[i][j];
            float lespecialwall = wall_color_special[i][j];
            spawnCube(x, y, z, diagram[i][j], textureIndex, lespecialwall , floorIndex, shader);
            x += 2.0f;
        }
        x = startX;
        z -= 2.0f;
    }
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Set viewport size
    glViewport(0, 0, width, height);
}

// Callback function for mouse movement
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    if (camera.locked == false)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// Callback function for mouse scroll wheel
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

static double lastKeyPressTime = 0.0;

void victory(Player &player)
{
    if (player.didiwon(p_end_x, p_end_y) == true)
    {
        array_to_use[p_end_x][p_end_y] = 16;
        victory_flag =true;
    }
}

std::vector<int> generateNumberList(int start, int end)
{
    std::vector<int> numbers;
    for (int i = start; i <= end; ++i)
    {
        numbers.push_back(i);
    }
    return numbers;
}

void fillArrayWithRandomValues(int **array, std::vector<int>& numbers)
{
    srand(time(NULL)); 

    for (int k = 0; k < numbers.size(); ++k)
    {
        int value = numbers[k];
        int i = rand() % 10; 
        int j = rand() % 10; 

        
        while (array[i][j] != 0)
        {
            i = rand() % 10;
            j = rand() % 10;
        }
        
        array[i][j] = value; 
    }
}

// Function to handle keyboard input
void processInput(GLFWwindow *window, Player &player)
{
    double currentTime = glfwGetTime();
    double timeElapsed = currentTime - lastKeyPressTime;
    // Check for key presses

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        lastKeyPressTime = currentTime;
    }

    if (!camera.isTurning && !camera.isMoving)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (player.move(true))
            {
                camera.ProcessKeyboard(FORWARD, deltaTime);
                lastKeyPressTime = currentTime;
                victory(player);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if (player.move(false))
            {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
                lastKeyPressTime = currentTime;
                victory(player);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
            lastKeyPressTime = currentTime;
            player.turnRight();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            player.turnLeft();
            camera.ProcessKeyboard(RIGHT, deltaTime);
            lastKeyPressTime = currentTime;
        }
    }

    if (camera.isMoving)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
            lastKeyPressTime = currentTime;
        }
    }

    if (timeElapsed >= 0.5)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.toggle_lock();
            lastKeyPressTime = currentTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
    {
        if (camera.isTurning)
        {
            camera.ProcessKeyboard(FORWARD, deltaTime); // Process turning
        }
        if (camera.isMoving)
        {
            camera.ProcessKeyboard(LEFT, deltaTime); // Process turning
        }
    }
}
