//
// Created by mihael on 05.05.22..
//

#include "Bezier.h"
#include "iostream"
#include "math.h"
#include "algorithm"

float binomial_coefficient(int n, int k) {
    return
            (k > n) ? 0.0f :
            (k == 0 || k== n) ? 1.0f :
            (k == 1 || k == n - 1) ? n :
            binomial_coefficient(n - 1, k - 1) * n / (float) k;
}

float Bezier_function(int i, int n, float t) {
    return
            (i == 0 && n == 0) ? 1 :
            (i == -1) ? 1 :
            (i == n + 1) ? 0 :
            (1 - t) * Bezier_function(i, n - 1, t) + t * Bezier_function(i - 1, n - 1, t);
}

std::vector<glm::vec3> Bezier::approximation_Bezier_curve(std::vector<glm::vec3> points, int number_of_points) {
    std::vector<glm::vec3> result;
    int n = points.size() - 1;

    std::vector<float> binomial_coefficients;
    for (int i = 0; i <= n; i++)
        binomial_coefficients.push_back(binomial_coefficient(n, i));

    for (int j = 0; j <= number_of_points; j++) {
        double t = j / (double) number_of_points;
        glm::vec3 p = glm::vec3(0, 0, 0);

        for (int i = 0; i <= n; i++) {
            p += points[i] * binomial_coefficients[i] * (float) pow(t, i) * (float) pow(1 - t, n - i);
        }

        result.push_back(p);
    }

    return result;
}

std::vector<glm::vec3> Bezier::interpolation_Bezier_curve(std::vector<glm::vec3> points, int number_of_points) {
    std::vector<glm::vec3> result;
    if (points.size() == 1) return result;

    int n = std::min(3, (int) points.size() - 1);

    std::vector<glm::vec3> control_points;
    for (int i = 0; i <= n; i++)
        control_points.push_back(points[points.size() - 1 - n + i]);

    glm::mat4 F;
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            F[i][j] = Bezier_function(j, n, i / (double) n);

    for (int i = n; i <= 3; i++)
        F[i][i] = 1;

    glm::mat4 F_ = glm::inverse(F);

    std::vector<glm::vec3> A;

    for (int i = 0; i <= n; i++) {
        glm::vec3 a = glm::vec3(0, 0, 0);
        for (int j = 0; j <= n; j++)
            a += F_[i][j] * control_points[j];
        A.push_back(a);
    }

    std::vector<float> binomial_coefficients;
    for (int i = 0; i <= n; i++)
        binomial_coefficients.push_back(binomial_coefficient(n, i));

    for (int j = 0; j <= number_of_points; j++) {
        float t = j / (double) number_of_points;
        glm::vec3 p = glm::vec3(0, 0, 0);

        for (int i = 0; i <= n; i++)
            p += A[i] * Bezier_function(i, n, t);

        result.push_back(p);
    }

    return result;
}