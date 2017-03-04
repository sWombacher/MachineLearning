#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

struct Vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float data[3];
    };
    constexpr Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    float& operator[](size_t idx) { return data[idx]; }
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
typedef std::array<std::array<float, 3>, 3> Mat3x3;

enum CHANGE { NO_CHANGE, GOAL_ENEMY, GOAL_DARMSTADT };
enum NOISE { OLE, TOOR, OHHH };
static std::vector<NOISE> noises = {OLE, OLE, OHHH, OLE, TOOR};
static size_t current_noise_idx = 0;

Vec3 hiddenMarcovModel(const Mat3x3& trans, const Mat3x3& sensor,
                       Vec3 probabilities = {1.f / 3.f, 1.f / 3.f, 1.f / 3.f}) {
    Vec3 newProb(0, 0, 0);
    for (size_t i = 0; i < 3; ++i) {
        std::cout << "Calculation " << i << ":\n(";
        for (size_t iter = 0; iter < 3; ++iter) {
            newProb[i] += probabilities[iter] * trans[iter][i];
            std::cout << probabilities[iter] << " * " << trans[iter][i]
                      << " + ";
        }
        newProb[i] *= sensor[noises[current_noise_idx]][i];
        std::cout << "\b\b\b) * " << sensor[noises[current_noise_idx]][i]
                  << '\n'
                  << std::endl;
    }
    newProb *= 1.f / (newProb.x + newProb.y + newProb.z);
    std::cout << "\n\n";

    if (++current_noise_idx < noises.size())
        return hiddenMarcovModel(trans, sensor, newProb);

    std::cout << std::endl;
    return newProb;
}

int main() {
    Mat3x3 sensor;
    sensor[NOISE::OLE][CHANGE::NO_CHANGE] = 0.8f;
    sensor[NOISE::OLE][CHANGE::GOAL_ENEMY] = 0.1f;
    sensor[NOISE::OLE][CHANGE::GOAL_DARMSTADT] = 0.1f;
    sensor[NOISE::OHHH][CHANGE::NO_CHANGE] = 0.15f;
    sensor[NOISE::OHHH][CHANGE::GOAL_ENEMY] = 0.7f;
    sensor[NOISE::OHHH][CHANGE::GOAL_DARMSTADT] = 0.1f;
    sensor[NOISE::TOOR][CHANGE::NO_CHANGE] = 0.05f;
    sensor[NOISE::TOOR][CHANGE::GOAL_ENEMY] = 0.2f;
    sensor[NOISE::TOOR][CHANGE::GOAL_DARMSTADT] = 0.8f;

    Mat3x3 trans;
    trans[CHANGE::NO_CHANGE][CHANGE::NO_CHANGE] = 0.6f;
    trans[CHANGE::NO_CHANGE][CHANGE::GOAL_ENEMY] = 0.2f;
    trans[CHANGE::NO_CHANGE][CHANGE::GOAL_DARMSTADT] = 0.2f;
    trans[CHANGE::GOAL_ENEMY][CHANGE::NO_CHANGE] = 0.4f;
    trans[CHANGE::GOAL_ENEMY][CHANGE::GOAL_ENEMY] = 0.3f;
    trans[CHANGE::GOAL_ENEMY][CHANGE::GOAL_DARMSTADT] = 0.3f;
    trans[CHANGE::GOAL_DARMSTADT][CHANGE::NO_CHANGE] = 0.4f;
    trans[CHANGE::GOAL_DARMSTADT][CHANGE::GOAL_ENEMY] = 0.4f;
    trans[CHANGE::GOAL_DARMSTADT][CHANGE::GOAL_DARMSTADT] = 0.2f;

    std::cout << std::setprecision(3);
    Vec3 vec = hiddenMarcovModel(trans, sensor);
    std::cout << "Probability no change: " << vec[CHANGE::NO_CHANGE] << '\n'
              << "Probability goal darm: " << vec[CHANGE::GOAL_DARMSTADT]
              << '\n'
              << "Probability goal enem: " << vec[CHANGE::GOAL_ENEMY] << '\n'
              << std::endl;
    return 0;
}
