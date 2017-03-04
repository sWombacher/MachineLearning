#include <glm/mat3x3.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

enum CHANGE { NO_CHANGE, GOAL_ENEMY, GOAL_DARMSTADT };
enum NOISE { OLE, TOOR, OHHH };
static std::vector<NOISE> noises = {
    OLE, OLE, OHHH, OLE, TOOR
};
static size_t current_noise_idx = 0;

glm::vec3 hiddenMarcovModel(const glm::mat3x3& trans, const glm::mat3x3& sensor,
                    glm::vec3 probabilities = {1.f / 3.f, 1.f / 3.f, 1.f / 3.f})
{
    glm::vec3 newProb(0, 0, 0);
    for (int i = 0; i < 3; ++i) {
        std::cout << "Calculation " << i << ":\n(";
        for (int iter = 0; iter < 3; ++iter) {
            newProb[i] += probabilities[iter] * trans[iter][i];
            std::cout << probabilities[iter] << " * " << trans[iter][i]
                      << " + ";
        }
        newProb[i] *= sensor[noises[current_noise_idx]][i];
        std::cout << "\b\b\b) * "
                  << sensor[noises[current_noise_idx]][i] << '\n'
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
    glm::mat3x3 sensor;
    sensor[NOISE::OLE][CHANGE::NO_CHANGE] = 0.8f;
    sensor[NOISE::OLE][CHANGE::GOAL_ENEMY] = 0.1f;
    sensor[NOISE::OLE][CHANGE::GOAL_DARMSTADT] = 0.1f;
    sensor[NOISE::OHHH][CHANGE::NO_CHANGE] = 0.15f;
    sensor[NOISE::OHHH][CHANGE::GOAL_ENEMY] = 0.7f;
    sensor[NOISE::OHHH][CHANGE::GOAL_DARMSTADT] = 0.1f;
    sensor[NOISE::TOOR][CHANGE::NO_CHANGE] = 0.05f;
    sensor[NOISE::TOOR][CHANGE::GOAL_ENEMY] = 0.2f;
    sensor[NOISE::TOOR][CHANGE::GOAL_DARMSTADT] = 0.8f;

    glm::mat3x3 trans;
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
    glm::vec3 vec = hiddenMarcovModel(trans, sensor);
    std::cout << "Probability no change: " << vec[CHANGE::NO_CHANGE] << '\n'
              << "Probability goal darm: " << vec[CHANGE::GOAL_DARMSTADT] << '\n'
              << "Probability goal enem: " << vec[CHANGE::GOAL_ENEMY] << '\n'
              << std::endl;
    return 0;
}
