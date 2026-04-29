#include "math/Matrix.hpp"

#include <cmath>

namespace Raytracer {

Matrix::Matrix() noexcept {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

Matrix Matrix::operator*(const Matrix& other) const noexcept {
    Matrix result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Vector3D Matrix::operator*(const Vector3D& vec) const noexcept {
    double x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * 1.0;
    double y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * 1.0;
    double z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * 1.0;
    double w = m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * 1.0;

    if (w != 0.0 && w != 1.0) {
        return Vector3D(x / w, y / w, z / w);
    }
    return Vector3D(x, y, z);
}

Matrix Matrix::rotateX(double angle_radians) noexcept {
    Matrix result;
    double c = std::cos(angle_radians);
    double s = std::sin(angle_radians);
    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;
    return result;
}

Matrix Matrix::rotateY(double angle_radians) noexcept {
    Matrix result;
    double c = std::cos(angle_radians);
    double s = std::sin(angle_radians);
    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;
    return result;
}

Matrix Matrix::rotateZ(double angle_radians) noexcept {
    Matrix result;
    double c = std::cos(angle_radians);
    double s = std::sin(angle_radians);
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    return result;
}

Matrix Matrix::lookAt(const Point3D& position, const Point3D& target, const Vector3D& up) noexcept {
    Vector3D forward = (position - target).normalized(); // z-axis
    Vector3D right = up.cross(forward).normalized();     // x-axis
    Vector3D new_up = forward.cross(right);              // y-axis

    Matrix result;
    result.m[0][0] = right.x;
    result.m[0][1] = right.y;
    result.m[0][2] = right.z;
    result.m[0][3] = -right.dot(position);

    result.m[1][0] = new_up.x;
    result.m[1][1] = new_up.y;
    result.m[1][2] = new_up.z;
    result.m[1][3] = -new_up.dot(position);

    result.m[2][0] = forward.x;
    result.m[2][1] = forward.y;
    result.m[2][2] = forward.z;
    result.m[2][3] = -forward.dot(position);

    result.m[3][0] = 0.0;
    result.m[3][1] = 0.0;
    result.m[3][2] = 0.0;
    result.m[3][3] = 1.0;

    return result;
}

Matrix Matrix::translate(double x, double y, double z) noexcept {
    Matrix result;
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

Matrix Matrix::scale(double sx, double sy, double sz) noexcept {
    Matrix result;
    result.m[0][0] = sx;
    result.m[1][1] = sy;
    result.m[2][2] = sz;
    return result;
}

Matrix Matrix::inverse() const noexcept {
    Matrix inv;

    double aug[4][8];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            aug[i][j] = m[i][j];
            aug[i][j + 4] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int i = 0; i < 4; ++i) {
        int pivot = i;
        for (int j = i + 1; j < 4; ++j) {
            if (std::fabs(aug[j][i]) > std::fabs(aug[pivot][i])) {
                pivot = j;
            }
        }

        if (pivot != i) {
            for (int j = 0; j < 8; ++j) {
                std::swap(aug[i][j], aug[pivot][j]);
            }
        }

        if (std::fabs(aug[i][i]) < 1e-15) {
            return Matrix();
        }

        double diag = aug[i][i];
        for (int j = 0; j < 8; ++j) {
            aug[i][j] /= diag;
        }

        for (int j = 0; j < 4; ++j) {
            if (j != i) {
                double factor = aug[j][i];
                for (int k = 0; k < 8; ++k) {
                    aug[j][k] -= factor * aug[i][k];
                }
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            inv.m[i][j] = aug[i][j + 4];
        }
    }

    return inv;
}

} // namespace Raytracer
