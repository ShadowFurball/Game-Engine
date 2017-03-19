/**
    @file shader-manager.cpp
    @author Tarkan Kemalzade
    @date 19/03/2017
*/

#include <Graphics-Engine\shader-manager.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace ShaderInfo
{
    struct shader_file_extension
    {
        const char *extentions;
        ShaderType type;
    };

    struct shader_file_extension extensions[] =
    {
        { ".vs", ShaderType::VERTEX },
        { ".vert", ShaderType::VERTEX },
        { ".gs", ShaderType::GEOMETRY },
        { ".geom", ShaderType::GEOMETRY },
        { ".tcs", ShaderType::TESS_CONTROL },
        { ".tes", ShaderType::TESS_EVALUATION },
        { ".fs", ShaderType::FRAGMENT },
        { ".frag", ShaderType::FRAGMENT },
        { ".cs", ShaderType::COMPUTE }
    };
}

ShaderManager::ShaderManager() : handle(0), linked(false)
{

}

ShaderManager::~ShaderManager()
{
    if (handle == 0)
    {
        return;
    }

    // Query the number of attached shaders
    GLint numShaders = 0;
    gl::GetProgramiv(handle, gl::ATTACHED_SHADERS, &numShaders);

    // Get the shader names
    GLuint * shaderNames = new GLuint[numShaders];
    gl::GetAttachedShaders(handle, numShaders, NULL, shaderNames);

    // Delete the shaders
    for (int i = 0; i < numShaders; i++)
    {
        gl::DeleteShader(shaderNames[i]);
    }

    // Delete the program
    gl::DeleteProgram(handle);
    delete[] shaderNames;
}

void ShaderManager::compileShader(const char * fileName)
throw(ShaderProgramException)
{
    int numExts = sizeof(ShaderInfo::extensions) / sizeof(ShaderInfo::shader_file_extension);

    // Check the file name's extension to determine the shader type
    std::string extension = getExtension(fileName);
    ShaderType type = VERTEX;
    bool matchFound = false;
    for (int i = 0; i < numExts; i++)
    {
        if (extension == ShaderInfo::extensions[i].extentions)
        {
            matchFound = true;
            type = ShaderInfo::extensions[i].type;
            break;
        }
    }

    // If we didn't find a match, throw an exception
    if (!matchFound)
    {
        std::string messgage = "Unrecognized extension: " + extension;
        throw ShaderProgramException(messgage);
    }

    // Pass the discovered shader type along
    compileShader(fileName, type);
}

std::string ShaderManager::getExtension(const char * name)
{
    std::string nameStr(name);

    size_t loc = nameStr.find_last_of('.');
    if (loc != std::string::npos)
    {
        return nameStr.substr(loc, std::string::npos);
    }
    return "";
}

void ShaderManager::compileShader(const char * fileName, ShaderType type)
throw(ShaderProgramException)
{
    if (!fileExists(fileName))
    {
        std::string message = std::string("Shader: ") + fileName + " not found.";
        throw ShaderProgramException(message);
    }

    if (handle <= 0)
    {
        handle = gl::CreateProgram();
        if (handle == 0)
        {
            throw ShaderProgramException("Unable to create shader program.");
        }
    }

    std::ifstream inFile(fileName, std::ios::in);
    if (!inFile)
    {
        std::string message = std::string("Unable to open: ") + fileName;
        throw ShaderProgramException(message);
    }

    // Get file contents
    std::stringstream code;
    code << inFile.rdbuf();
    inFile.close();

    compileShader(code.str(), type, fileName);
}

void ShaderManager::compileShader(const std::string & source, ShaderType type, const char * fileName)
throw(ShaderProgramException)
{
    if (handle <= 0)
    {
        handle = gl::CreateProgram();
        if (handle == 0)
        {
            throw ShaderProgramException("Unable to create shader program.");
        }
    }

    GLuint shaderHandle = gl::CreateShader(type);

    const char * c_code = source.c_str();
    gl::ShaderSource(shaderHandle, 1, &c_code, NULL);

    // Compile the shader
    gl::CompileShader(shaderHandle);

    // Check for errors
    int result;
    gl::GetShaderiv(shaderHandle, gl::COMPILE_STATUS, &result);
    if (FALSE == result)
    {
        // Compile failed, get log
        int length = 0;
        std::string logString;
        gl::GetShaderiv(shaderHandle, gl::INFO_LOG_LENGTH, &length);
        if (length > 0)
        {
            char * c_log = new char[length];
            int written = 0;
            gl::GetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }
        std::string message;
        if (fileName)
        {
            message = std::string(fileName) + ": shader compliation failed\n";
        }
        else
        {
            message = "Shader compilation failed.\n";
        }
        message += logString;

        throw ShaderProgramException(message);

    }
    else
    {
        // Compile succeeded, attach shader
        gl::AttachShader(handle, shaderHandle);
    }
}

void ShaderManager::link() throw(ShaderProgramException)
{
    if (linked)
    {
        return;
    }

    if (handle <= 0)
    {
        throw ShaderProgramException("Program has not been compiled.");
    }

    gl::LinkProgram(handle);

    int status = 0;
    gl::GetProgramiv(handle, gl::LINK_STATUS, &status);
    if (FALSE == status)
    {
        // Store log and return false
        int length = 0;
        std::string logString;

        gl::GetProgramiv(handle, gl::INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            char * c_log = new char[length];
            int written = 0;
            gl::GetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        throw ShaderProgramException(std::string("Program link failed:\n") + logString);
    }
    else
    {
        uniformLocations.clear();
        linked = true;
    }
}

void ShaderManager::use() throw(ShaderProgramException)
{
    if (handle <= 0 || (!linked))
        throw ShaderProgramException("Shader has not been linked");
    gl::UseProgram(handle);
}

int ShaderManager::getHandle()
{
    return handle;
}

bool ShaderManager::isLinked()
{
    return linked;
}

void ShaderManager::bindAttribLocation(GLuint location, const char * name)
{
    gl::BindAttribLocation(handle, location, name);
}

void ShaderManager::bindFragDataLocation(GLuint location, const char * name)
{
    gl::BindFragDataLocation(handle, location, name);
}

void ShaderManager::setUniform(const char *name, float x, float y, float z)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform3f(loc, x, y, z);
}

void ShaderManager::setUniform(const char *name, const glm::vec3 & v)
{
    this->setUniform(name, v.x, v.y, v.z);
}

void ShaderManager::setUniform(const char *name, const glm::vec4 & v)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderManager::setUniform(const char *name, const glm::vec2 & v)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform2f(loc, v.x, v.y);
}

void ShaderManager::setUniform(const char *name, const glm::mat4 & m)
{
    GLint loc = getUniformLocation(name);
    gl::UniformMatrix4fv(loc, 1, FALSE, &m[0][0]);
}

void ShaderManager::setUniform(const char *name, const glm::mat3 & m)
{
    GLint loc = getUniformLocation(name);
    gl::UniformMatrix3fv(loc, 1, FALSE, &m[0][0]);
}

void ShaderManager::setUniform(const char *name, float val)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform1f(loc, val);
}

void ShaderManager::setUniform(const char *name, int val)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform1i(loc, val);
}

void ShaderManager::setUniform(const char *name, GLuint val)
{
    GLint loc = getUniformLocation(name);
    gl::Uniform1ui(loc, val);
}

void ShaderManager::setUniform(const char *name, bool val)
{
    int loc = getUniformLocation(name);
    gl::Uniform1i(loc, val);
}

void ShaderManager::printActiveUniforms() {
    GLint numUniforms = 0;
    gl::GetProgramInterfaceiv(handle, gl::UNIFORM, gl::ACTIVE_RESOURCES, &numUniforms);

    GLenum properties[] = { gl::NAME_LENGTH, gl::TYPE, gl::LOCATION, gl::BLOCK_INDEX };

    printf("Active uniforms:\n");
    for (int i = 0; i < numUniforms; ++i)
    {
        GLint results[4];
        gl::GetProgramResourceiv(handle, gl::UNIFORM, i, 4, properties, 4, NULL, results);

        if (results[3] != -1) continue;  // Skip uniforms in blocks 
        GLint nameBufSize = results[0] + 1;
        char * name = new char[nameBufSize];
        gl::GetProgramResourceName(handle, gl::UNIFORM, i, nameBufSize, NULL, name);
        printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
        delete[] name;
    }
}

void ShaderManager::printActiveUniformBlocks() {
    GLint numBlocks = 0;

    gl::GetProgramInterfaceiv(handle, gl::UNIFORM_BLOCK, gl::ACTIVE_RESOURCES, &numBlocks);
    GLenum blockProps[] = { gl::NUM_ACTIVE_VARIABLES, gl::NAME_LENGTH };
    GLenum blockIndex[] = { gl::ACTIVE_VARIABLES };
    GLenum props[] = { gl::NAME_LENGTH, gl::TYPE, gl::BLOCK_INDEX };

    for (int block = 0; block < numBlocks; ++block)
    {
        GLint blockInfo[2];
        gl::GetProgramResourceiv(handle, gl::UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
        GLint numUnis = blockInfo[0];

        char * blockName = new char[blockInfo[1] + 1];
        gl::GetProgramResourceName(handle, gl::UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);
        printf("Uniform block \"%s\":\n", blockName);
        delete[] blockName;

        GLint * unifIndexes = new GLint[numUnis];
        gl::GetProgramResourceiv(handle, gl::UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

        for (int unif = 0; unif < numUnis; ++unif)
        {
            GLint uniIndex = unifIndexes[unif];
            GLint results[3];
            gl::GetProgramResourceiv(handle, gl::UNIFORM, uniIndex, 3, props, 3, NULL, results);

            GLint nameBufSize = results[0] + 1;
            char * name = new char[nameBufSize];
            gl::GetProgramResourceName(handle, gl::UNIFORM, uniIndex, nameBufSize, NULL, name);
            printf("    %s (%s)\n", name, getTypeString(results[1]));
            delete[] name;
        }

        delete[] unifIndexes;
    }
}

void ShaderManager::printActiveAttribs()
{
    GLint numAttribs;
    gl::GetProgramInterfaceiv(handle, gl::PROGRAM_INPUT, gl::ACTIVE_RESOURCES, &numAttribs);

    GLenum properties[] = { gl::NAME_LENGTH, gl::TYPE, gl::LOCATION };

    printf("Active attributes:\n");
    for (int i = 0; i < numAttribs; ++i) {
        GLint results[3];
        gl::GetProgramResourceiv(handle, gl::PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

        GLint nameBufSize = results[0] + 1;
        char * name = new char[nameBufSize];
        gl::GetProgramResourceName(handle, gl::PROGRAM_INPUT, i, nameBufSize, NULL, name);
        printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
        delete[] name;
    }
}

const char * ShaderManager::getTypeString(GLenum type)
{
    switch (type) {
    case gl::FLOAT:
        return "float";
    case gl::FLOAT_VEC2:
        return "vec2";
    case gl::FLOAT_VEC3:
        return "vec3";
    case gl::FLOAT_VEC4:
        return "vec4";
    case gl::DOUBLE:
        return "double";
    case gl::INT:
        return "int";
    case gl::UNSIGNED_INT:
        return "unsigned int";
    case gl::BOOL:
        return "bool";
    case gl::FLOAT_MAT2:
        return "mat2";
    case gl::FLOAT_MAT3:
        return "mat3";
    case gl::FLOAT_MAT4:
        return "mat4";
    default:
        return "?";
    }
}

void ShaderManager::validate() throw(ShaderProgramException)
{
    if (!isLinked())
    {
        throw ShaderProgramException("Program is not linked");
    }

    GLint status;
    gl::ValidateProgram(handle);
    gl::GetProgramiv(handle, gl::VALIDATE_STATUS, &status);

    if (FALSE == status)
    {
        // Store log and return false
        int length = 0;
        std::string logString;

        gl::GetProgramiv(handle, gl::INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            char * c_log = new char[length];
            int written = 0;
            gl::GetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        throw ShaderProgramException(std::string("Program failed to validate\n") + logString);
    }
}

int ShaderManager::getUniformLocation(const char * name)
{
    std::map<std::string, int>::iterator pos;
    pos = uniformLocations.find(name);

    if (pos == uniformLocations.end())
    {
        uniformLocations[name] = gl::GetUniformLocation(handle, name);
    }

    return uniformLocations[name];
}

bool ShaderManager::fileExists(const std::string & fileName)
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}