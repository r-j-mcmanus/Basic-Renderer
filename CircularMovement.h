//#pragma once
//
//#include "MovementController.h"
//
//#include <glm/glm.hpp>
//
//
//
//class CircularMovement : public MovementController {
//private:
//    float angularVelocity;
//    glm::vec3 center;
//    glm::vec3 tangent;
//public:
//    CircularMovement(float angularVelocity, glm::vec3 center, glm::vec3 tangent)
//        : angularVelocity(angularVelocity), center(center), tangent(glm::normalize(tangent)) {
//    }
//
//    void update(WorldObject& obj, float deltaTime) ;
//};