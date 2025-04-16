#include "core/Spline.h"
#include <iostream>

Spline::Spline(const std::vector<glm::vec2>& points, glm::mat4 charactersticsMatrix, unsigned int numSamples)
  : points(points),
    charactersticsMatrix(glm::transpose(charactersticsMatrix)),
    numSamples(numSamples) {

    computeEvenlySpacedPoints();
}

float Spline::computeArcLength(float t1, float t2, unsigned int steps) {
    float dt = (t2 - t1) / steps;
    float length = 0.f;

    glm::vec2 previousDerivative = getDerivative(t1);

    for (unsigned int i = 1; i <= steps; ++i) {
        float t = t1 + i * dt;
        glm::vec2 derivative = getDerivative(t);

        length += 0.5f * (glm::length(previousDerivative) + glm::length(derivative)) * dt;
        previousDerivative = derivative;
    }

    return length;
}

glm::vec2 Spline::getDerivative(float u) {
    if (points.size() < 4) {
        return {0.f, 0.f};
    }

    u *= points.size() - 3;

    float t;
    unsigned int n;

    if (u == 0.f) {
        t = 0.f;
        n = 0;
    } else if (std::fmod(u, 1.f) == 0.f) {
        t = 1.f;
        n = static_cast<unsigned int>(u) - 1;
    } else {
        t = std::fmod(u, 1.f);
        n = static_cast<unsigned int>(u);
    }

    glm::vec4 parameterVector(0.f, 1.f, 2.f * t, 3.f * t * t);

    glm::vec2 p0 = points[n];
    glm::vec2 p1 = points[n + 1];
    glm::vec2 p2 = points[n + 2];
    glm::vec2 p3 = points[n + 3];

    glm::mat4x2 pointVector(p0, p1, p2, p3);

    return (parameterVector * charactersticsMatrix) * glm::transpose(pointVector);
}

void Spline::computeEvenlySpacedPoints() {
    sampledPoints.clear();

    float totalArcLength = computeArcLength(0.f, 1.f, 200.f);
    float step = totalArcLength / (numSamples - 1);

    float targetArc = 0.f;
    float currentT = 0.f;

    sampledPoints.push_back(GetPoint(0.f));

    for (int i = 1; i < numSamples; ++i) {
        targetArc += step;

        float l  = currentT;
        float r = 1.f;
        float m;

        while (r - l > 1e-4) {
            m = (r + l) * .5f;
            float arc = computeArcLength(0.f, m, 100);
            if (arc < targetArc) {
                l = m;
            } else {
                r = m;
            }
        }

        currentT = (l + r) * 0.5f;
        sampledPoints.push_back(GetPoint(currentT));
    }
}

void Spline::SetCharacteristicsMatrix(glm::mat4 matrix) {
    charactersticsMatrix = matrix;
}

glm::vec2 Spline::GetPoint(float u) {
    if (points.size() < 4) {
        return {0.f, 0.f};
    }

    u *= points.size() - 3;

    float t;
    unsigned int n;

    if (u == 0.f) {
        t = 0.f;
        n = 0;
    } else if (std::fmod(u, 1.f) == 0.f) {
        t = 1.f;
        n = static_cast<unsigned int>(u) - 1;
    } else {
        t = std::fmod(u, 1.f);
        n = static_cast<unsigned int>(u);
    }

    glm::vec4 parameterVector(1.f, t, t * t, t * t * t);

    glm::vec2 p0 = points[n];
    glm::vec2 p1 = points[n + 1];
    glm::vec2 p2 = points[n + 2];
    glm::vec2 p3 = points[n + 3];

    glm::mat4x2 pointVector(p0, p1, p2, p3);

    return (parameterVector * charactersticsMatrix) * glm::transpose(pointVector);
}

glm::vec2 Spline::GetUniformPoint(float u) {
    u *= (sampledPoints.size() - 1);
    unsigned int index = static_cast<unsigned int>(u);
    float t = u - static_cast<float>(index);

    if (index >= sampledPoints.size() - 1) {
        return sampledPoints.back();
    }

    return glm::mix(sampledPoints[index], sampledPoints[index + 1], t);
}