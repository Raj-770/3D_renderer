#include <glm/glm.hpp>

class ModelMatrix {
public:
    ModelMatrix();

    void setTranslation(const glm::vec3& translation);
    void setRotation(float angleDegrees, const glm::vec3& axis);
    void setScale(const glm::vec3& sclae);

    glm::mat4 getModelMatrix() const;

private:
    glm::vec3 translation_;
    float rotationAngle_;
    glm::vec3 rotationAxis_;
    glm::vec3 scale_;

    mutable glm::mat4 model_;
    mutable bool dirty_;

    void updateModelMatrix() const;
};