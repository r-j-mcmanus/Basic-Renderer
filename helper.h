#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


inline  glm::mat4 createRotationMatrix(const glm::vec3& rotation) {
    // TODO quaterions 
    // Convert degrees to radians
    float pitchRad = glm::radians(rotation[0]);
    float yawRad = glm::radians(rotation[1]);
    float rollRad = glm::radians(rotation[2]);

    // Rotation around X-axis (pitch)
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));

    // Rotation around Y-axis (yaw)
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), yawRad, glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotation around Z-axis (roll)
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), rollRad, glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine rotations (order: ZYX)
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    return rotationMatrix;
}

inline void printFloat(const float& f, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << f << std::endl;
}

inline void printVec2(const glm::vec2& vec, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "(" << vec.x << ", " << vec.y << ")" << std::endl;
}

inline void printVec3(const glm::vec3& vec, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

inline void printVec4(const glm::vec4& vec, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")" << std::endl;
}

inline void printMat4(const glm::mat4& mat, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ":" << std::endl;
    const float* m = glm::value_ptr(mat);
    for (int i = 0; i < 4; i++) {
        std::cout << "[ ";
        for (int j = 0; j < 4; j++) {
            std::cout << m[j * 4 + i] << " ";
        }
        std::cout << "]" << std::endl;
    }
}