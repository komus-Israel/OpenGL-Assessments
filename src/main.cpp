#include "windowHandler.hpp"
#include "pipeline.hpp"

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
        windowHandler.renderLoop();
    }

    //  clean GLFW resources upon render loop exit
    void clean() {
        glfwTerminate();
    }

    void handlePipeline() {
        pipeline.handleVBO();
        pipeline.setVertexAttribute();
        pipeline.compileVertexShader();     //  compile vertex shader
        pipeline.compileFragmentShader();   //  compile fragment shader
        pipeline.createShaderProgram(       //  create program and link shaders
            pipeline.vertexShader,
            pipeline.fragmentShader
        );
        pipeline.useProgram();              //  activate shader program
        pipeline.deleteShaders();           //  delete shaders once linked
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