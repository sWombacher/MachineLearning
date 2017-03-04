#include <glm/mat3x3.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

enum Noise { OLE, TOOR, OHHH };
static std::vector<Noise> noises = {
    OLE//, OLE, OHHH, OLE, TOOR
};
static size_t current_noise_idx = 0;


glm::vec3 recursion(const glm::mat3x3& trans, const glm::mat3x3& sensor,
                    glm::vec3 probabilities = {1.f/3.f, 1.f/3.f, 1.f/3.f})
{
    glm::vec3 newProb(0,0,0);
    for (int i = 0; i < 3; ++i){
        for (int iter = 0; iter < 3; ++iter)
            newProb[i] += probabilities[iter] * trans[iter][i];

        newProb[i] *= sensor[noises[current_noise_idx]][i];
    }
    newProb *= 1.f / (newProb.x + newProb.y + newProb.z);

    if (++current_noise_idx < noises.size())
        return recursion(trans, sensor, newProb);
    return newProb;
}

int main(){
    glm::mat3x3 sensor;
    sensor[0][0] = 0.8f;
    sensor[1][0] = 0.1f;
    sensor[2][0] = 0.1f;
    sensor[0][1] = 0.05f;
    sensor[1][1] = 0.2f;
    sensor[2][1] = 0.8f;
    sensor[0][2] = 0.15f;
    sensor[1][2] = 0.7f;
    sensor[2][2] = 0.1f;

    glm::mat3x3 trans;
    trans[0][0] = 0.6f;
    trans[1][0] = 0.4f;
    trans[2][0] = 0.4f;
    trans[0][1] = 0.2f;
    trans[1][1] = 0.3f;
    trans[2][1] = 0.4f;
    trans[0][2] = 0.2f;
    trans[1][2] = 0.3f;
    trans[2][2] = 0.2f;

    std::cout << std::setprecision(3);
    glm::vec3 vec = recursion(trans, sensor);
    std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;
    return 0;
}

