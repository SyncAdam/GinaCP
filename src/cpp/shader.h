#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    //the programs ID
    unsigned int shaderProgram;
  
    //constructor reads and builds the shader
    Shader(const char* vertexSourcePath, const char* fragmentSourcePath)
    {
        std::string vertexShaderSource;
        std::string fragmentShaderSource;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;

        try{

            std::filesystem::path currentPath = std::filesystem::current_path();

            // Convert the path to a string and print it
            std::cout << "Current Working Directory: " << currentPath.string() << std::endl;

            std::cout << "Opening ifstream for vertex shader from: " << vertexSourcePath << std::endl;
            std::cout << "Opening ifstream for fragment shader from: " << fragmentSourcePath << std::endl;

            vertexShaderFile.open(vertexSourcePath);
            fragmentShaderFile.open(fragmentSourcePath);

            //create stringstreams
            std::stringstream vertexShaderStream, fragmentShaderStream;
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexShaderSource = vertexShaderStream.str();
            fragmentShaderSource = fragmentShaderStream.str();
        }

        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR: SHADER FILES NOT SUCCESFULLY READ" << std::endl;
        }

        const char* vertexShaderCode = vertexShaderSource.c_str();
        const char* fragmentShaderCode = fragmentShaderSource.c_str();

        unsigned int vertexShader, fragmentShader;
        int success;
        char infoLog[512];
        
        //create and compile vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);

        //check if compilation was successful
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR: SHADER VERTEX COMPILATION FAILED\n" << infoLog << std::endl;
        };        

        //create and compile fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);

        //check if compilation was successful
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR  SHADER FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
        };
        
        //creating a shaderprogram and linking shaders to it.
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        //print linking errors if any
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
        }
          
        // delete the shaders as they're linkedshaderProgram into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    };
    
    void use()
    {
        glUseProgram(shaderProgram);
    };

    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }

    void setMat4(const std::string &name, glm::mat4 projectionMat) const
    {
         glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(projectionMat));
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); 
    }
    
};
  
#endif