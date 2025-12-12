#include <glad/glad.h>

#include "fileio.h"
#include "platform.h"

// Move OpenGL code to renderer in engine
void
app_update_and_render(const struct key_events *input)
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    const float vertices[9] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int shader_program = glCreateProgram();

    struct file_content frag_file = platform_get_file_content("../app/temp.frag");
    struct file_content vert_file = platform_get_file_content("../app/temp.vert");
    const char *fragsrc = frag_file.mem;
    const char *vertsrc = vert_file.mem;
    
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragsrc, NULL);
    glCompileShader(frag_shader);

    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertsrc, NULL);
    glCompileShader(vert_shader);

    platform_free_file_memory(&frag_file);
    platform_free_file_memory(&vert_file);

    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    glUseProgram(shader_program);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
