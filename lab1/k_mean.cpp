#ifdef CHAOS_LIB
#include <windowCoordinateSystem.hpp>
#endif
#ifndef FILE_PATH
#define FILE_PATH ""
#endif

#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <cmath>
#include <map>

struct Point{
    int32_t x, y;
    constexpr Point(int32_t X, int32_t Y) : x(X), y(Y){}
    double length() const { return std::sqrt(x * x + y * y); }
    Point operator-(const Point& rhs) const { return Point(x - rhs.x, y - rhs.y); }
    Point& operator+=(const Point& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    bool operator<(const Point& rhs) const{
        return (int64_t(x) << 32) + int64_t(y) < (int64_t(rhs.x) << 32) + int64_t(rhs.y);
    }
    bool operator!=(const Point& rhs) const{ return x != rhs.x || y != rhs.y; }
};

struct Interface{
    virtual ~Interface() = default;
    virtual double distance(const Point& instance, const Point& centroid) = 0;
    virtual Point centroid(const Point& oldCentroid, const std::vector<Point>& instances) = 0;
    virtual Point assign(const Point& instance, const std::vector<Point>& centroids) = 0;
    virtual std::map<Point, std::vector<Point>> cluster(const std::vector<Point>& instances, std::vector<Point> centroids) = 0;
};

#ifdef CHAOS_LIB
static void updateWindow(cf::WindowCoordinateSystem& window, const std::map<Point, std::vector<Point>>& grouped){
    const cf::Color colors[] = {
        cf::Color::CYAN, cf::Color::BLUE,
        cf::Color::PINK, cf::Color::RED,
        cf::Color::GREY, cf::Color::BLACK
    };
    window.clear();

    size_t colorIdx = 0;
    for (const auto& e : grouped){
        if (colorIdx >= sizeof(colors) / sizeof(colors[0]))
            colorIdx = 0;

        // visualize point
        window.drawPoint(cf::Point(e.first.x, e.first.y), colors[colorIdx++], 3);
        for (const auto& p : e.second)
            window.drawPoint(cf::Point(p.x, p.y), colors[colorIdx]);
        ++colorIdx;
    }
    window.show();
    window.waitKey();
}
#endif

struct Cluster : public Interface{
#ifdef CHAOS_LIB
    cf::WindowCoordinateSystem* m_Window;
    Cluster(cf::WindowCoordinateSystem* window = nullptr) : m_Window(window){}
#endif
    ~Cluster() = default;
    double distance(const Point& instance, const Point& centroid) override { return (instance - centroid).length(); }

    virtual Point centroid(const Point& oldCentroid, const std::vector<Point>& instances) override{
        if (!instances.size())
            return oldCentroid;

        Point mean(0, 0);
        for (const auto& point : instances)
            mean += point;

        mean.x /= instances.size();
        mean.y /= instances.size();
        return mean;
    }
    virtual Point assign(const Point& instance, const std::vector<Point>& centroids) override{
        Point point(-1, -1);
        double dist = std::numeric_limits<double>::max();

        for (const auto& e : centroids){
            const double tmpDist = distance(instance, e);
            if (dist > tmpDist){
                dist = tmpDist;
                point = e;
            }
        }
        return point;
    }

    virtual std::map<Point, std::vector<Point>>
    cluster(const std::vector<Point>& instances, std::vector<Point> centroids) override
    {
        using CLUSTER = std::map<Point, std::vector<Point>>;
        CLUSTER cluster;
        for (const auto& e : centroids)
            cluster[e] = std::vector<Point>();

        while (true){
            // assign
            for (const auto& e : instances)
                cluster[assign(e, centroids)].push_back(e);

#ifdef CHAOS_LIB
            // update visuals
            if (this->m_Window)
                updateWindow(*this->m_Window, cluster);
#endif
            // calculate mean
            bool changed = false;
            for (auto& e : centroids){
                const auto& points = cluster[e];
                Point newCentroid = centroid(e, points);
                if (newCentroid != e){
                    e = newCentroid;
                    changed = true;
                }
            }
            // finished?
            if (!changed)
                return cluster;

            cluster.clear();
        }
    }
};

static std::vector<Point> readMarsFile(const char* filePath){
    std::vector<Point> points;
    std::fstream file(filePath, std::fstream::in);
    std::string str;

    // ignore first line
    std::getline(file, str);
    while (std::getline(file, str)){
        int x, y;
        sscanf(str.c_str(), "%d,%d", &x, &y);
        points.emplace_back(x, y);
    }
    return points;
}
int main(){
    const auto points = readMarsFile(FILE_PATH "Marsroboter-Weka.csv");
    //const auto points = readMarsFile(FILE_PATH "Marsroboter-Weka_ordered.csv");

    std::vector<Point> centroids = { Point(10, 10), Point(50, 50), Point(80, 50) };
    //std::vector<Point> centroids = { Point(10, 10), Point(20, 20), Point(30, 30) };
    //std::vector<Point> centroids = { Point(40, 50), Point(100, 50) };

#ifdef CHAOS_LIB
    cf::WindowCoordinateSystem window(600, {0, 120}, {0, 120});
    Cluster cluster(&window);
    auto grouped = cluster.cluster(points, centroids);

    // visual output
    updateWindow(window, grouped);
#else
    Cluster cluster;
    auto grouped = cluster.cluster(points, centroids);
#endif
    // text output
    std::cout << "Found centroids:\n\n";
    for (const auto& e : grouped){
        std::cout << "Centroid: " << e.first.x << ' ' << e.first.y << "\n\n";
        for (const auto& p : e.second)
            std::cout << p.x << ' ' << p.y << '\n';
        std::cout << "\n\n\n";
    }
    std::cout << std::endl;
	return 0;
}
