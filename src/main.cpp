#include "triangle_mesh.h"
#include "config.h"

unsigned int make_module(const std::string& filepath, unsigned int module_type) 
{
    std::ifstream file;
    std::string line;
    std::stringstream ss;

    file.open(filepath);
    while(std::getline(file, line)) {
        ss << line << std::endl;
    }

    const std::string& source = ss.str();
    const char* src = source.c_str();

    file.close();

    unsigned int module = glCreateShader(module_type);
    glShaderSource(module, 1, &src, NULL);
    glCompileShader(module);

    int success;
    
    glGetShaderiv(module, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(module, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return module;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) 
{
    unsigned int vertex_module = make_module(vertex_filepath, GL_VERTEX_SHADER);
    unsigned int fragment_module = make_module(fragment_filepath, GL_FRAGMENT_SHADER);

    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vertex_module);
    glAttachShader(shader, fragment_module);
    glLinkProgram(shader);

    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex_module);
    glDeleteShader(fragment_module);

    return shader;
}

TriangleMesh* triangle = nullptr;
unsigned int shader = 0;

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);
    triangle->draw();
    //glFlush();
    glutSwapBuffers();
}

void handleKeyPress(unsigned char key, int x, int y) {
    if (key == 27) {
        //glutLeaveMainLoop();
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("open_gl_glut");

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("Renderer version: %s\n", glGetString(GL_RENDERER));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.9f, 0.1f, 0.1f, 1.0f);

    triangle = new TriangleMesh();
    shader = make_shader(
        "/FULL_PATH_HERE/opengl_glut/src/shaders/vertex.txt",
        "/FULL_PATH_HERE/opengl_glut/src/shaders/fragment.txt"
    );

    glutReshapeFunc([](int w, int h) {
        glViewport(0, 0, w, h);
    });

    glutDisplayFunc(render);


    glutKeyboardFunc(handleKeyPress);

    glutMainLoop();

    glDeleteProgram(shader);
    delete triangle;

    return 0;
}
