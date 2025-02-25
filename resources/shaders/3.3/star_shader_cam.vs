#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float time; 

uniform mat4 camera_to_clip;
uniform mat4 world_to_camera;
uniform mat4 local_to_world;

void main()
{
    // Calculate hover motion
    float hover = sin(time * 2.0) * 0.1;
    
    // 3D rotation around Y axis
    float angle = time;
    mat3 rotationY = mat3(
        cos(angle), 0.0, sin(angle),   // first column
        0.0, 1.0, 0.0,                 // second column
        -sin(angle), 0.0, cos(angle)   // third column
    );
    
    // Apply transformations
    vec3 rotatedPos = rotationY * aPos; // Apply 3D rotation
    rotatedPos.y += hover; // Add hover effect
    
    gl_Position = camera_to_clip * world_to_camera * local_to_world * vec4(rotatedPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
