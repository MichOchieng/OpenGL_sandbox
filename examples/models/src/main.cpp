#include "window/window.hpp"
#include <iostream>
#include "shader/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "modelLoading.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "camera/camera.hpp"


int main()
{
    
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;
    std::string title = "Example 6: Model Loading";

    Window window(SCR_WIDTH, SCR_HEIGHT, title,true);
    Camera camera(glm::vec3(0.0f,0.0f,0.0f));

    camera.registerCallbacks(window.window);

    std::vector<ivpt> star = parse_model_into_ivpts("resources/assets/star/star.obj");
    // Build & compile shader program

    Shader ourShader("resources/shaders/5.1/shader.vs", "resources/shaders/5.1/shader.fs");

    // IBO - Index Buffer Object 
    // TBO - Texture Buffer Object
    unsigned int VBO, VAO, TBO, IBO;
    // Create and bind Vertex Array Object (VAO)
    // Stores all of the state needed to supply vertex data
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Create and bind Vertex Buffer Object (VBO)
    // Stores vertices in GPU memory
    glGenBuffers(1, &VBO);
    // Bind the VBO to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // static draw because the data will not be changed
    glBufferData(GL_ARRAY_BUFFER, star[0].vert.size()*sizeof(glm::vec3), star[0].vert.data(), GL_STATIC_DRAW);
    // Set up position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    // Create and bind Texture Coordinate Buffer Object (TBO)
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);  // Switch to TBO as the active array buffer
    glBufferData(GL_ARRAY_BUFFER, star[0].tex_cords.size()*sizeof(glm::vec2), star[0].tex_cords.data(), GL_STATIC_DRAW);
    // Set up texture coordinate attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    
    // Create and bind Index Buffer Object (IBO)
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, star[0].indices.size()*sizeof(unsigned int), star[0].indices.data(), GL_STATIC_DRAW);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // Fragments with lower depth values pass (closer to camera)

    // Unbind VAO to prevent accidental modification
    // Note: We do NOT unbind the element array buffer (IBO) because VAO stores that binding
    glBindVertexArray(0);
    // Unbind other buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    unsigned char *data = stbi_load("resources/assets/star/star.png", &width, &height, &nrChannels, 0);

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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, texture1);

            // Use shader and update time uniform
            ourShader.use();
            ourShader.setFloat("time", currentTime);
            ourShader.setMat4("camera_to_clip", camera.getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT));
            ourShader.setMat4("local_to_world", glm::mat4(1.0f));
            ourShader.setMat4("world_to_camera", camera.getViewMatrix());

            // now render the model
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, star[0].indices.size(), GL_UNSIGNED_INT, 0);

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
