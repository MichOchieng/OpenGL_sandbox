#include "window/window.hpp"
#include <iostream>
#include "shader/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera/camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


int main()
{
    // Create a 800 x 600 window titled "Example 4: Textures"
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;
    std::string title = "Example 5: Camera";

    Window window(SCR_WIDTH, SCR_HEIGHT, title,true);
    Camera camera(glm::vec3(0.0f,0.0f,0.0f));

    camera.registerCallbacks(window.window);
    
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

            // Update time
            currentTime = glfwGetTime(); // Gets time since GLFW initialized
            float deltaTime = currentTime - prevTime;

            // Update camera
            camera.update(deltaTime);

            // Render - clear the colorbuffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, texture1);

            // Use shader and update time uniform
            ourShader.use();
            ourShader.setFloat("time", currentTime);
            ourShader.setMat4("camera_to_clip", camera.getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT));
            ourShader.setMat4("local_to_world", glm::mat4(1.0f));
            ourShader.setMat4("world_to_camera", camera.getViewMatrix());

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
