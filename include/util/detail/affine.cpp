#include "util/rotate.h"

glm::mat4 createRotationMatrix(float angleRadians)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    return model;
}
