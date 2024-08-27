#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

//  The graphics pipeline converts a set of 3D co-ordinates into
//  2D pixels that fits in the screen
//  First, 3D co-ordinates are converted to 2D co-ordinates
//  Secondly, the 2D co-ordinates are converted into colored pixels
class GraphicsPipeline {

    //  Vertex Shader
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location=0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    ;

    //  Fragment Shader
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    ;

private:

    unsigned int compileShader(const char *shaderSource, unsigned int shaderType) {
       
        //  In order for opengl to use the shader, it has to dynamically compile it at run-time from its
        //  source code
        //  the type of shader being created is passed to function below (vertex, fragment, etc)
        unsigned int shader = glCreateShader(shaderType);
        
        // Attach the shader source code to the shader object and compile the shader
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;

            if (shaderType == GL_VERTEX_SHADER) {

                throw std::runtime_error("Vertex Shader compilation failed!");

            } else if (shaderType == GL_FRAGMENT_SHADER) {

                throw std::runtime_error("Fragment Shader compilation failed!");

            }
            
        }

        return shader;
    }   


public:

    unsigned int vertexShader;
    unsigned int fragmentShaders[2];
    unsigned int shaderPrograms[2];
    unsigned int VBOs[0];
    unsigned int VAOs[2];
    unsigned int EBO;

    //  vertices data for the triangle
    //  Vertex data is a collection of vertices
    float vertices1[9] = {
        -0.8f, -0.5f, 0.0f,
        -0.2f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    float vertices2[9] = {
        0.2f, -0.5f, 0.0f,
        0.8f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    //  Unique Vertices for rectangle (to be used for EBO)
    // float vertices[12] = {
    //     0.5f, 0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,
    //     -0.5f, 0.5f, 0.0f
    // };

    //  note that indices starts from 0 (to be used for EBO)
    // unsigned int indices[6] = {
    //     0, 1 , 3,
    //     1, 2, 3
    // };

    void bindVAO(unsigned int objectId) {

        if (objectId == 1) 
            glBindVertexArray(VAOs[0]);
        else 
            glBindVertexArray(VAOs[1]);
    }

    //  Vertex buffer objects
    //  Memory handler to store the vertex data on the GPU
    //  VBO can store a large numer of vetices in the GPU's memory
    void handleVBO(unsigned int bufferId) {

        if (bufferId == 1) {

            bindVAO(VAOs[0]);

            //  Bind buffer to the `GL_ARRAY_BUFFER`
            //  Buffer type for VBO is `GL_ARRAY_BUFFER`
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

            //  copy the defined vertex into memory of the buffer currently binded, in 
            //  this case, the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        } else {

            bindVAO(VAOs[1]);

            //  Bind buffer to the `GL_ARRAY_BUFFER`
            //  Buffer type for VBO is `GL_ARRAY_BUFFER`
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

            //  copy the defined vertex into memory of the buffer currently binded, in 
            //  this case, the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

        }

       

    }

    // void handleEBO() {

    //     //  Generate buffer with an ID
    //     glGenBuffers(1, &EBO);

    //     //  Bind EBO
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //     //  copy indices into the buffer
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // }

    //  Instruct OPENGL on how it should interpret the vertex data (per vertex attribute)
    //  using `glVertexAttribPointer`
    //  float size is 4 bytes, this represents the size of each position in a vertex
    //  The stride for a single 3D vertex will be 3 * 4 =  12. 
    //  A 3D vertex will have 12bytes by implication
    void setVertexAttribute() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //  To use a VAO, bind it using `glBindVertexArray`
    void generateVAOs() {

        //  Generate VAO with an Id
        // std::cout << "VAO: " << _VAO << std::endl; 
        // std::cout << "VAO: " << VAO_1 << std::endl; 

        glGenVertexArrays(2, VAOs); //  generate multiple VAOs
 
    }

    //  Generate VBOs 
    void generateVBOs() {
        glGenBuffers(2, VBOs);
    }


    //  write the vertex shader in the vertex language (GLSL)
    //  Takes as input a single vertex
    //  Transforms 3D co-ordinates into different 3D co-ordinates
    void compileVertexShader() {

        //  With the vertex data defined, it will be sent as the input to the first process
        //  of the Graphics pipelin: the vertex shader
        //  This is done by creating memory on the GPU where we store the vertex data
        //  This memory is handled by the VBO

        //  compile vertex shader
        vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);

    }

    //  Fragment shader is about calculating the color output of the pixels
    //  Colors are represented as an array of 4 values (RGBA)
    //  The strenth of each component is a value between 0.0 and 1.0
    void compileFragmentShader() {

       //   compile fragment shader
       fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    }

    //  Shader Program object is the final linked version of multiple shaders combined
    //  To use the compiled shaders, they need to be linked to a shader program object
    //  Afterwards, the shader program is activated when rendering objects
    //  When linking the shaders into a program, it links the output of each shader to the inputs of the next shader
    void createShaderProgram(unsigned int vtxShader, unsigned int fragShader) {

        //  create shader program
        shaderProgram = glCreateProgram();

        //  attach the compiled shaders to the program
        glAttachShader(shaderProgram, vtxShader);
        glAttachShader(shaderProgram, fragShader);
        
        //  link the shaders via the shaderProgram
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            throw std::runtime_error("Shader Program Linking Failed!");
    
        }

    }

    //  Activate shader program
    void useProgram() {
        glUseProgram(shaderProgram);
    }

    //  Delete shader objects once linked into program object as they are no longer needed
    void deleteShaders() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void drawTriangle() {
        glDrawArrays(GL_TRIANGLES, 0, 3); // for vertices

        // glDrawArrays(GL_TRIANGLES, 0, numOfVertices); // for vertices

        //  For EBO
        //  First argument specifies the mode to draw in
        //  Second: the total number of vertices, since 6 indices were to be drawn
        //  Third: The type of the indices
        //  Fourth: Offset in the EBO
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


};

