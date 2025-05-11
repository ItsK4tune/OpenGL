#ifndef AFFINE_H
#define AFFINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 createRotationMatrix(glm::vec3 rotationRadians);
glm::mat4 createTranslationMatrix(glm::vec3 translation);
glm::mat4 createScalingMatrix(glm::vec3 scale);
glm::mat4 createAffineTransformMatrix(
    glm::vec3 scale,
    glm::vec3 rotationRadians,
    glm::vec3 translation);

#endif
