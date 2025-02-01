#include "window/window.hpp"
#include <iostream>
#include "shader/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int main()
{
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;
    std::string title = "Example 4: Textures";
    Window window(SCR_WIDTH, SCR_HEIGHT, title);
    // Build & compile shader program
    // ------------------------------------

    Shader ourShader("resources/shaders/3.3/star_shader.vs", "resources/shaders/3.3/star_shader.fs");

    // set up vertex data + buffers & config vertex attributes
    float vertices[] = {
        // positions            // colors           // texture coords
        0.0f, 0.0f, 0.0f,       1.0f, 1.0f, 0.0f,   0.5f, 0.5f,      // center
        0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,   0.5f, 0.0f,      // top
        -0.15f, 0.15f, 0.0f,    1.0f, 1.0f, 0.0f,   0.35f, 0.35f, // inner top left
        -0.4f, 0.15f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 0.35f,   // outer left
        -0.2f, -0.15f, 0.0f,    1.0f, 1.0f, 0.0f,   0.3f, 0.65f,  // inner left
        -0.25f, -0.4f, 0.0f,    1.0f, 1.0f, 0.0f,   0.25f, 1.0f,  // bottom left
        0.0f, -0.25f, 0.0f,     1.0f, 1.0f, 0.0f,   0.5f, 0.75f,   // inner bottom
        0.25f, -0.4f, 0.0f,     1.0f, 1.0f, 0.0f,   0.75f, 1.0f,   // bottom right
        0.2f, -0.15f, 0.0f,     1.0f, 1.0f, 0.0f,   0.7f, 0.65f,   // inner right
        0.4f, 0.15f, 0.0f,      1.0f, 1.0f, 0.0f,   1.0f, 0.35f,    // outer right
        0.15f, 0.15f, 0.0f,     1.0f, 1.0f, 0.0f,   0.65f, 0.35f,  // inner top right
        0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,   0.5f, 0.0f       // top
    };

    unsigned int indices[] = {
        0, 1, 2,  // top triangle
        0, 2, 3,  // top left
        0, 3, 4,  // left
        0, 4, 5,  // bottom left
        0, 5, 6,  // bottom
        0, 6, 7,  // bottom right
        0, 7, 8,  // right
        0, 8, 9,  // top right
        0, 9, 10, // top
        0, 10, 11 // closing triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // // texture coord attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    // unsigned int texture1;
    // // texture 1
    // // ---------
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    // unsigned char *data = stbi_load("resources/textures/starman.png", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // // -------------------------------------------------------------------------------------------
    // ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // // either set it manually like so:
    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    try
    {

        while (!window.shouldClose())
        {
            window.processInput();

            // render
            // clear the colorbuffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // bind textures on corresponding texture units
            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, texture1);

            // now render the triangle
            ourShader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

            window.swapBuffers();
            window.pollEvents();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
