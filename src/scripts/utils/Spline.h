#ifndef SPLINE_H
#define SPLINE_H

#include <glm/glm.hpp>
#include <vector>

namespace CharacteristicsMatrix {
    inline constexpr glm::mat4 CatmullRom = glm::mat4(
        -0.5f,  1.5f, -1.5f,  0.5f,
         1.0f, -2.5f,  2.0f, -0.5f,
        -0.5f,  0.0f,  0.5f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.0f
    );

    inline glm::mat4 CatmullRomTension(float s) {
        return glm::mat4(
            -s,         2.0f - s,   s - 2.0f,   s,
             2.0f * s,  s - 3.0f,   3.0f - 2.0f * s, -s,
            -s,         0.0f,       s,         0.0f,
             0.0f,      1.0f,       0.0f,       0.0f
        );
    }

    inline constexpr glm::mat4 Bezier = glm::mat4(
        -1.0f,  3.0f, -3.0f,  1.0f,
         3.0f, -6.0f,  3.0f,  0.0f,
        -3.0f,  3.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  0.0f
    );

    inline constexpr glm::mat4 BSpline = glm::mat4(
        -1.0f / 6.0f,  0.5f,       -0.5f,        1.0f / 6.0f,
         0.5f,        -1.0f,        0.5f,        0.0f,
        -0.5f,         0.0f,        0.5f,        0.0f,
         1.0f / 6.0f,  2.0f / 3.0f, 1.0f / 6.0f, 0.0f
    );

    inline constexpr glm::mat4 Hermite = glm::mat4(
        2.0f, -2.0f,  1.0f,  1.0f,
       -3.0f,  3.0f, -2.0f, -1.0f,
        0.0f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.0f,  0.0f,  0.0f
    );
}

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
    Spline();

    Spline(const std::vector<glm::vec2>& points, glm::mat4 charactersticsMatrix, unsigned int numSamples = 100);

    void SetCharacteristicsMatrix(glm::mat4 matrix);

    glm::vec2 GetPoint(float u);

    glm::vec2 GetUniformPoint(float u);
};

#endif // SPLINE_H