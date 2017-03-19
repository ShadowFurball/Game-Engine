/**
    @headerfile shader-manager.h
    @author Tarkan Kemalzade
    @date 19/03/2017
*/

#pragma once
#pragma warning(disable : 4290)

#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H


#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdexcept>

class ShaderProgramException : public std::runtime_error
{
    public:
        ShaderProgramException(const std::string & msg) :
            std::runtime_error(msg) { }
};

enum ShaderType
{
    VERTEX = gl::VERTEX_SHADER,
    FRAGMENT = gl::FRAGMENT_SHADER,
    GEOMETRY = gl::GEOMETRY_SHADER,
    TESS_CONTROL = gl::TESS_CONTROL_SHADER,
    TESS_EVALUATION = gl::TESS_EVALUATION_SHADER,
    COMPUTE = gl::COMPUTE_SHADER
};

class ShaderManager
{
    public:
        ShaderManager();
        ~ShaderManager();

        void   compileShader(const char *fileName) throw (ShaderProgramException);
        void   compileShader(const char * fileName, ShaderType type) throw (ShaderProgramException);
        void   compileShader(const std::string & source, ShaderType type,
            const char *fileName = NULL) throw (ShaderProgramException);

        void   link() throw (ShaderProgramException);
        void   validate() throw(ShaderProgramException);
        void   use() throw (ShaderProgramException);

        int    getHandle();
        bool   isLinked();

        void   bindAttribLocation(GLuint location, const char * name);
        void   bindFragDataLocation(GLuint location, const char * name);

        void   setUniform(const char *name, float x, float y, float z);
        void   setUniform(const char *name, const glm::vec2 & v);
        void   setUniform(const char *name, const glm::vec3 & v);
        void   setUniform(const char *name, const glm::vec4 & v);
        void   setUniform(const char *name, const glm::mat4 & m);
        void   setUniform(const char *name, const glm::mat3 & m);
        void   setUniform(const char *name, float val);
        void   setUniform(const char *name, int val);
        void   setUniform(const char *name, bool val);
        void   setUniform(const char *name, GLuint val);

        void   printActiveUniforms();
        void   printActiveUniformBlocks();
        void   printActiveAttribs();

        const char * getTypeString(GLenum type);

    private:
        int  handle;
        bool linked;
        std::map<std::string, int> uniformLocations;

        GLint  getUniformLocation(const char * name);
        bool fileExists(const std::string & fileName);
        std::string getExtension(const char * fileName);

        // Make these private in order to make the object non-copyable
        ShaderManager(const ShaderManager & other) { }
        ShaderManager & operator=(const ShaderManager &other) { return *this; }
};

#endif // !_SHADER_MANAGER_H