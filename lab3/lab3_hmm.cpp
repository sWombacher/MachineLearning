#include <iostream>
#include <iomanip>
#include <vector>

struct Vec3 {
    union {
        struct { float x, y, z; };
        float data[3];
    };
    constexpr Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    float& operator[](size_t idx) { return data[idx]; }
    const float& operator[](size_t idx) const { return data[idx]; }
    Vec3& operator+=(const Vec3& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    Vec3& operator*=(float value) {
        this->x *= value;
        this->y *= value;
        this->z *= value;
        return *this;
    }
};
typedef float Mat3x3[3][3];

enum CHANGE { NO_GOAL, GOAL_OP, GOAL_DA };
enum NOISE { OLEE, TOOR, OHHH };
static std::vector<NOISE> noises = { OLEE, OLEE, OHHH, OLEE, TOOR };

Vec3 hiddenMarcovModel(const Mat3x3& trans, const Mat3x3& sensor,
                       const Vec3& probabilities, size_t noiseIdx = 0)
{
    std::cout << "Time point: " << noiseIdx + 1 << std::endl;
    Vec3 newProb(0, 0, 0);
    for (size_t i = 0; i < 3; ++i) {
        std::cout << '(';
        for (size_t iter = 0; iter < 3; ++iter) {
            newProb[i] += probabilities[iter] * trans[iter][i];
            std::cout << probabilities[iter] << " * " << trans[iter][i]
                      << " + ";
        }
        newProb[i] *= sensor[noises[noiseIdx]][i];
        std::cout << "\b\b\b) * " << sensor[noises[noiseIdx]][i] << std::endl;
    }
    std::cout << "\n\n";
    newProb *= 1.f / (newProb.x + newProb.y + newProb.z);

    if (++noiseIdx < noises.size())
        return hiddenMarcovModel(trans, sensor, newProb, noiseIdx);

    return newProb;
}

int main() {
    Mat3x3 sensor;
    sensor[NOISE::OLEE][CHANGE::NO_GOAL] = 0.80f; sensor[NOISE::OLEE][CHANGE::GOAL_OP] = 0.1f; sensor[NOISE::OLEE][CHANGE::GOAL_DA] = 0.1f;
    sensor[NOISE::TOOR][CHANGE::NO_GOAL] = 0.05f; sensor[NOISE::TOOR][CHANGE::GOAL_OP] = 0.2f; sensor[NOISE::TOOR][CHANGE::GOAL_DA] = 0.8f;
    sensor[NOISE::OHHH][CHANGE::NO_GOAL] = 0.15f; sensor[NOISE::OHHH][CHANGE::GOAL_OP] = 0.7f; sensor[NOISE::OHHH][CHANGE::GOAL_DA] = 0.1f;

    Mat3x3 trans;
    trans[CHANGE::NO_GOAL][CHANGE::NO_GOAL] = 0.6f; trans[CHANGE::NO_GOAL][CHANGE::GOAL_OP] = 0.2f; trans[CHANGE::NO_GOAL][CHANGE::GOAL_DA] = 0.2f;
    trans[CHANGE::GOAL_OP][CHANGE::NO_GOAL] = 0.4f; trans[CHANGE::GOAL_OP][CHANGE::GOAL_OP] = 0.3f; trans[CHANGE::GOAL_OP][CHANGE::GOAL_DA] = 0.3f;
    trans[CHANGE::GOAL_DA][CHANGE::NO_GOAL] = 0.4f; trans[CHANGE::GOAL_DA][CHANGE::GOAL_OP] = 0.2f; trans[CHANGE::GOAL_DA][CHANGE::GOAL_DA] = 0.2f;

    std::cout << std::setprecision(3);
    Vec3 vec = hiddenMarcovModel(trans, sensor, {1.f/3.f, 1.f/3.f, 1.f/3.f});
    std::cout << "Probability no change: " << vec[CHANGE::NO_GOAL] << '\n'
              << "Probability goal darm: " << vec[CHANGE::GOAL_DA] << '\n'
              << "Probability goal enem: " << vec[CHANGE::GOAL_OP] << '\n'
              << std::endl;
    return 0;
}
