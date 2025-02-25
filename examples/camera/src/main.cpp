#include "window/window.hpp"
#include <iostream>
#include "shader/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool up, down, left, right, forward, backward = false;

auto position = glm::vec3(0.0f, 0.0f, 0.0f);

float yaw = 0.0f;
float pitch = 0.0f;
float speed = 10.0f;
float two_pi = 2.0f * 3.14159265359f;
float selected_speed = 10.0f;
float fov = 60.0f;
float near_plane = 0.1f;
float far_plane = 100.0f;

float deltaTime = 0.0f;

float mouseSensitivity = 0.001f;

double prevXPos = 0.0;
double prevYPos = 0.0;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        forward = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        forward = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        backward = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        backward = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        left = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        left = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        right = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        right = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        up = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        up = false;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        down = true;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        down = false;
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{

    float xoffset = (xpos - prevXPos) * mouseSensitivity;
    float yoffset = (prevYPos - ypos) * mouseSensitivity; // reversed since y-coordinates go from bottom to top
    yaw += xoffset;
    pitch += yoffset;
    prevXPos = xpos;
    prevYPos = ypos;
}

glm::vec3 get_look_vector()
{
    glm::vec3 forward;
    forward.x = cos(pitch * two_pi) * cos(yaw * two_pi);
    forward.y = sin(pitch * two_pi);
    forward.z = cos(pitch * two_pi) * sin(yaw * two_pi);
    return glm::normalize(forward);
}

void process_input()
{

    // float delta_pos = selected_speed * deltaTime;

    glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

    // Compute forward and right vectors
    glm::vec3 forward_vec = get_look_vector();
    glm::vec3 right_vec = glm::normalize(glm::cross(forward_vec, up_vec)); // Up vector is (0, 1, 0)
    // std::cout << forward_vec.x << std::endl;
    // std::cout << forward_vec.y << std::endl;
    // std::cout << "----" << std::endl;
    glm::vec3 movement(0.0f);

    if (forward)
    {
        movement += forward_vec;
    }
    if (backward)
        movement -= forward_vec;
    if (left)
        movement -= right_vec;
    if (right)
        movement += right_vec;
    if (up)
        movement += up_vec;
    if (down)
        movement -= up_vec;

    if (glm::length(movement) > 0.0f)
    {
        movement = glm::normalize(movement);
    }

    position += movement * deltaTime;
}

glm::mat4 get_view_matrix()
{
    return glm::lookAt(position, position + get_look_vector(),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 get_projection_matrix(int screen_width_px,
                                           int screen_height_px)
{

    return glm::perspective(glm::radians(fov),
                            static_cast<float>(screen_width_px) / static_cast<float>(screen_height_px), near_plane,
                            far_plane);
}

int main()
{
    // Create a 800 x 600 window titled "Example 4: Textures"
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;
    std::string title = "Example 4: Textures";
    Window window(SCR_WIDTH, SCR_HEIGHT, title,true);

    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, cursor_position_callback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Build & compile shader program

    Shader ourShader("resources/shaders/3.3/star_shader_cam.vs", "resources/shaders/3.3/star_shader.fs");

    // set up vertex data + buffers & config vertex attributes

    /*
        Each vertex has 8 components: 3 for position, 3 for color, and 2 for texture coordinates
        Positon: x, y, z (z is always 0 in 2D)
        Color: r, g, b (unused since we are using a texture here but otherwise would be black in this case)
        Texture coordinates: s, t (where on the image to sample from)
    */
    float vertices[] = {
        // positions            // colors (unused)  // texture coords
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f,    // center
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f,    // top
        -0.15f, 0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 0.7f, // inner top left
        -0.4f, 0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.1f, 0.6f,  // outer left
        -0.2f, -0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.2f, 0.4f, // inner left
        -0.25f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.3f, 0.1f, // bottom left
        0.0f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.2f,  // inner bottom
        0.25f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.7f, 0.1f,  // bottom right
        0.2f, -0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.8f, 0.4f,  // inner right
        0.4f, 0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.9f, 0.6f,   // outer right
        0.15f, 0.15f, 0.0f, 1.0f, 1.0f, 0.0f, 0.7f, 0.7f,  // inner top right
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f     // back to top
    };

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    unsigned int VBO, VAO;
    // Create and bind Vertex Array Object (VAO)
    // Stores all of the state needed to supply vertex data
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind Vertex Buffer Object (VBO)
    // Stores vertices in GPU memory
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // static draw because the data will not be changed
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //  Configure vertex attributes(s).
    //  Tell OpenGL how to interpret the vertex data
    /*
        First parameter (0,1,2): Corresponds to layout(location = X) in vertex shader
        Second parameter (3,3,2): Number of components per attribute
        Third parameter: Data type
        Fourth parameter: Whether to normalize data
        Fifth parameter: Stride (total size of one vertex)
        Sixth parameter: Offset of this attribute in the vertex
    */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load and create a texture
    // -------------------------
    unsigned int texture1;
    float currentTime = 0.0f; // for animation

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("resources/textures/starman.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        // If the image has an alpha channel (PNG usually does), use GL_RGBA
        if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // // -------------------------------------------------------------------------------------------
    // ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // // either set it manually like so:
    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    // auto position = glm::vec3(0.0f, 0.0f, 0.0f);
    // float yaw = 0.0f;
    // float pitch = 0.0f;
    // float speed = 10.0f;

    float prevTime = glfwGetTime();

    // bool up, down, left, right, forward, backward = false;

    try
    {

        while (!window.shouldClose())
        {
            window.processInput();
            process_input();

            // Update time
            currentTime = glfwGetTime(); // Gets time since GLFW initialized
            deltaTime = currentTime - prevTime;

            // clear the colorbuffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, texture1);

            // Use shader and update time uniform
            ourShader.use();
            ourShader.setFloat("time", currentTime);
            ourShader.setMat4("camera_to_clip", get_projection_matrix(SCR_WIDTH, SCR_HEIGHT));
            ourShader.setMat4("local_to_world", glm::mat4(1.0f));
            ourShader.setMat4("world_to_camera", get_view_matrix());

            // now render the triangle
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

            window.swapBuffers();
            window.pollEvents();

            prevTime = currentTime;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
