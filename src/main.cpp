#include "windowHandler.hpp"


class App {

    
    WindowHandler windowHandler;
    GraphicsPipeline pipeline;

public:

    void run() {

        initWindow();
        initGlad();
        handlePipeline();
        handleLoop();
        clean();

        
    }

    void initWindow() {

        windowHandler.createWindow();

    }

    //  manage function pointers
    void initGlad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to Initialize GLAD");
        }
    }

    //  keep application alive    
    void handleLoop() {
        windowHandler.renderLoop(pipeline);
    }

    //  clean GLFW resources upon render loop exit
    void clean() {
     
        glfwTerminate();
    }

    void handlePipeline() {

        pipeline.compileVertexShader();     //  compile vertex shader
        pipeline.compileFragmentShader();   //  compile fragment shader
        pipeline.shaderPrograms[0] = pipeline.createShaderProgram(       //  create first shader program and link shaders
            pipeline.vertexShader,
            pipeline.fragmentShaders[0] //  links the first fragment shader
        );

        pipeline.shaderPrograms[1] = pipeline.createShaderProgram(       //  create second program and link shaders
            pipeline.vertexShader,
            pipeline.fragmentShaders[1] //  links the second fragment shader
        );

        pipeline.deleteShaders();  
        pipeline.generateVAOs();    //  generate VAOs
        pipeline.generateVBOs();    //  generate VBOs

        pipeline.handleVBO(1);      //  handle the first VBO & VAO
        pipeline.setVertexAttribute();

        pipeline.handleVBO(2);      //  handle the second VBO & VAO
        pipeline.setVertexAttribute();
        
    }


};

int main() {

    App app;
    
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    

}