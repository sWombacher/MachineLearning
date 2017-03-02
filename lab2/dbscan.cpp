#include "windowCoordinateSystem.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

struct Object{
    // -2 undefined
    // -1 noise
    // else cluster id
    static constexpr int UNDEFINED = -2;
    static constexpr int NOISE = -1;
    int id;

    float x;
    float y;

    constexpr Object(float X, float Y, int ID = Object::UNDEFINED) : id(ID), x(X), y(Y) {}
    float length() const { return std::sqrt(x * x + y * y); }
    Object operator-(const Object& rhs) const { return Object(x - rhs.x, y - rhs.y); }
    bool operator< (const Object& rhs) const { return id < rhs.id; }
};

static void updateWindow(cf::WindowCoordinateSystem& window, std::vector<Object>& grouped){
    window.clear();
    std::sort(grouped.begin(), grouped.end());

    int currentID = std::numeric_limits<int>::max();
    cf::Color color;
    for (const auto& e : grouped){
        if (e.id < 0)
            continue;

        if (e.id != currentID){
            currentID = e.id;
            std::cout << "New id: " << currentID << std::endl;
            color = cf::Color::RandomColor();
            window.show();
            window.waitKey();
        }
        window.drawPoint({e.x, e.y}, color);
        /*
        const cf::Interval& intervall_x = window.getIntervalX();
        const cf::Interval& intervall_y = window.getIntervalY();
        window.setWindowDisplayScale(2.f);
        if (e.x > intervall_x.min && e.x < intervall_x.max &&
            e.y > intervall_y.min && e.y < intervall_y.max)
        {
            window.setColor(e.x, e.y, color);
        }
        */
    }
    window.show();
    window.waitKey();
}



#ifdef ENABLE_EXPERIMENTAL_CONCEPTS
template<typename T> concept bool SimpleIterator = requires(T t) {
    *t;
    ++t;
    t != t;
};

template<typename T>
concept bool ObjectContainer = requires(T t) {
    { t.begin() } -> SimpleIterator;
    { t.end()   } -> SimpleIterator;
    { t.front() } -> Object&;
};
#endif


struct DBSCAN{
    constexpr DBSCAN(float epsilon, size_t minPoints) : m_Epsilon(epsilon), m_MinPoints(minPoints){}

#ifndef ENABLE_EXPERIMENTAL_CONCEPTS
    template<typename ObjectContainer>
#endif
    void operator() (ObjectContainer& objs) const {
        int clusterID = 0;
        for (Object& e : objs){
            if (e.id == Object::UNDEFINED){
                if (this->_expandCluster(objs, e, clusterID))
                    ++clusterID;
            }
        }
    }

private:
    template<typename _ObjContainer>
    bool _expandCluster(_ObjContainer& objs, Object& startObj, int clusterID) const {
        std::vector<Object*> seeds = this->_getEpsilonDistanceObjets(objs, startObj);
        if (seeds.size() < this->m_MinPoints){
            startObj.id = Object::NOISE;
            return false;
        }
        for (size_t i = 0; i < seeds.size(); ++i){
            seeds[i]->id = clusterID;
            if (seeds[i] == &startObj)
                seeds.erase(seeds.begin() + long(i--));
        }
        while(seeds.size()){
            auto& seed = seeds[0];
            if (seed->id == Object::UNDEFINED){
                auto seeds_ofSeed = this->_getEpsilonDistanceObjets(objs, *seed);
                if (seeds_ofSeed.size() >= this->m_MinPoints){
                    // seed itself we be added again,
                    // we ignore this since it will be removed later on
                    seeds.insert(seeds.end(), seeds_ofSeed.begin(), seeds_ofSeed.end());
                    for (auto& e : seeds)
                        e->id = clusterID;
                }
            }
            seeds.erase(seeds.begin());
        }
        return true;
    }
    template<typename _ObjContainer>
    std::vector<Object*> _getEpsilonDistanceObjets(_ObjContainer& objs, const Object& obj) const {
        std::vector<Object*> result;
        for (auto& e : objs){
            if ((e - obj).length() <= this->m_Epsilon)
                result.emplace_back(&e);
        }
        return result;
    }

    const float m_Epsilon;
    const size_t m_MinPoints;
};

std::vector<Object> readFile(const char* filePath){
    std::vector<Object> result;
    std::fstream file(filePath);
    for (std::string str; std::getline(file, str);){
        int idx;
        float x,y;
        sscanf(str.c_str(), "%d ; %f ; %f", &idx, &x, &y);
        result.emplace_back(x - 53.f, y - 8.f);
    }
    return result;
}

std::pair<float, size_t> getEpsilonAndK(const std::vector<Object>& data){
    cf::WindowCoordinateSystem window(800, {0, 1}, {0.5, 1});
    static constexpr size_t k = 3; // 2* |Dimensions| - 1, here Dimensions = 2

    auto getEpsilonObject = [&data](size_t idx) -> float {
        std::array<float, k> best;
        for (auto& e : best)
            e = std::numeric_limits<float>::max();

        for (size_t i = 0; i < data.size(); ++i){
            if (i == idx)
                continue;

            const float dist = (data[i] - data[idx]).length();
            if (dist < best[0]){
                for (size_t bestIter = 1; bestIter < best.size(); ++bestIter)
                    best[bestIter] = best[bestIter-1];

                best[0] = dist;
            }
        }
        return best.back();
    };

    size_t tmpIter = 0;
    std::vector<float> distances(data.size());
    for (auto& e : distances)
        e = getEpsilonObject(tmpIter++);

    // visualize
    std::sort(distances.begin(), distances.end(), std::greater<float>());
    window.setInterval({0.f, 1.f}, {0.f, 1.f}, 600);
    window.clear();
    window.drawAxis();

    tmpIter = 0;
    for (const auto& e : distances)
        window.drawPoint({float(tmpIter++) / float(distances.size()), e});

    float x,y;
    window.show();
    window.waitMouseInput(x, y);
    return { distances[size_t(x * distances.size())], k + 1 };
}

int main(){
    std::vector<Object> data = readFile(FILE_PATH "Fahrradkurier.csv");
    const auto epsilon_k = getEpsilonAndK(data);
    cf::WindowCoordinateSystem window(800, {0.f, 1.f}, {0.5f, 1.f});
    DBSCAN dbscan(epsilon_k.first, epsilon_k.second);
    dbscan(data);
    updateWindow(window, data);
    return 0;
}
