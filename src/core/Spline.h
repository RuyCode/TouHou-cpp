#ifndef SPLINE_H
#define SPLINE_H

#include <glm/glm.hpp>
#include <vector>

class Spline {
private:
    std::vector<glm::vec2> points;
    glm::mat4 charactersticsMatrix;

    unsigned int numSamples;
    std::vector<glm::vec2> sampledPoints;

    float computeArcLength(float t1, float t2, unsigned int steps);

    glm::vec2 getDerivative(float t);

    void computeEvenlySpacedPoints();

public:
    Spline(const std::vector<glm::vec2>& points, glm::mat4 charactersticsMatrix, unsigned int numSamples = 100);

    void SetCharacteristicsMatrix(glm::mat4 matrix);

    glm::vec2 GetPoint(float u);

    glm::vec2 GetUniformPoint(float u);
};

#endif // SPLINE_H