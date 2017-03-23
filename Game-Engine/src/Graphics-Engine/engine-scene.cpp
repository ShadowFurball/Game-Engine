#include<Graphics-Engine\engine-scene.h>

/**
    Defualt constructor for our scene in an engine
*/
EngineScene::EngineScene()
{

}

/**
    Initialise the scene
    @param camera <Camera> - use the camera as the viewport.
*/
void EngineScene::initScene(Camera camera)
{
    //Compile and link the shader into the initialised scene
    compileAndLinkShader();

    gl::Enable(gl::DEPTH_TEST);

    //Set up the lighting in the initialised scene
    setLightingParameters(camera);

    //Insert Objects Here using m_filename

}

/**
Updates the scene, this will be mainly used fir animation

@param fTime <float> - timer so that the function can update at a specific time rate.
*/
void EngineScene::updateScene(float fTime)
{

}

/**
Sets up the lighting variables, the rest will be set up in the vertex and fragment shader.

@param camera <Camera> - use the camera as the viewport.
*/
void EngineScene::setLightingParameters(Camera camera)
{
    glm::vec3 worldLight = glm::vec3(10.f, 10.f, 10.f);

    /*
    LIGHTING SET UP GOES HERE.
    USE THE SET UNIFORM TO SET UP VARIBLES IN THE SHADER FILES.
    */
    program.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    program.setUniform("n", 100);
    program.setUniform("LightPosition", worldLight);
}

/**
Render the scene

@param camera <Camera> - use the camera as the viewport.
*/
void EngineScene::render(Camera camera)
{
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    /*
    OBJECTS GO HERE.
    REMEBER TO SET THE OBJECTS SHADER MATERIALS HERE AS WELL.
    */

    model = glm::mat4(1.0f);
    setMatrices(camera);

    program.setUniform("Kd", 0.7f, 1.0f, 0.7f);
    program.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    program.setUniform("Ep", camera.getCameraPosition());
    program.setUniform("Ks", 0.7f, 1.0f, 0.7f);

}

/**
Set the matrices so that the MVP matrices are sent to the GPU

@param camera <Camera> - use the camera as the viewport.
*/
void EngineScene::setMatrices(Camera camera)
{
    glm::mat4 modelView = camera.getViewMatrix() * model;

    /*
    SET THE MODEL MATRIX VIEW HERE
    */

    glm::mat4 mv = camera.getViewMatrix() * model;
    program.setUniform("ModelMatrixView", mv);
    program.setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    program.setUniform("MVP", camera.getProjectionMatrix() * mv);
    glm::mat3 normMat = glm::transpose(glm::inverse(glm::mat3(model)));
    program.setUniform("M", model);
    program.setUniform("V", camera.getViewMatrix());
    program.setUniform("P", camera.getProjectionMatrix());

}

/**
Resizes the viewport.

@param camera <Camera> - use the camera as the viewport.
*/
void  EngineScene::resize(Camera camera, int winWidth, int winHeight)
{
    gl::Viewport(0, 0, winWidth, winHeight);
    iWidth = winWidth;
    iHeight = winHeight;
    camera.setAspectRatio((float)winWidth / winHeight);
}

/**
Compile and link the shaders
*/
void EngineScene::compileAndLinkShader()
{
    try
    {
        program.compileShader("resources/Shaders/shader.vs"); // Vertex Shader File Location
        program.compileShader("resources/Shaders/shader.fs"); // Fragment Shader File Location
        program.link();
        program.validate();
        program.use();
    }
    catch (ShaderProgramException & exception)
    {
        std::cerr << exception.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}