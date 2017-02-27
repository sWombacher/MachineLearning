#ifdef CHAOS_LIB
#include <windowCoordinateSystem.hpp>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <map>

struct Point{
    int x, y;
    Point(int X, int Y) : x(X), y(Y){}
    double length() const { return std::sqrt(x * x + y * y); }
    Point operator-(const Point& rhs) const { return Point(x - rhs.x, y - rhs.y); }
    Point& operator+=(const Point& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    bool operator<(const Point& rhs) const{
        if (x < rhs.x) return true;
        if (x > rhs.x) return false;
        return y < rhs.y;
    }
};

struct Interface{
    virtual ~Interface() = default;
    virtual double distance(const Point& instance, const Point& centroid) = 0;
    virtual Point centroid(const Point& oldCentroid, const std::vector<Point>& instances) = 0;
    virtual Point assign(const Point& instance, const std::vector<Point>& centroids) = 0;
    virtual std::map<Point, std::vector<Point>> cluster
            (const std::vector<Point>& instances, std::vector<Point> centroids) = 0;
};


struct Cluster : public Interface{
    ~Cluster() = default;

    double distance(const Point& instance, const Point& centroid) override { return (instance - centroid).length(); }

    virtual Point centroid(const Point& /*oldCentroid*/, const std::vector<Point>& instances){
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
            CLUSTER newCluster;

            // assign
            for (const auto& e : instances)
                cluster[assign(e, centroids)].push_back(e);

            // calculate mean
            bool changed = false;
            for (auto& e : centroids){
                const auto& points = cluster[e];
                Point newCentroid = centroid(e, points);
                newCluster[newCentroid] = std::vector<Point>();
            }

            // finished?
            if (!changed)
                break;

            cluster = std::move(newCluster);
        }
        return cluster;
    }
};


static std::vector<Point> readMarsFile(const char* filePath){
    std::vector<Point> points;
    std::fstream file(filePath, std::fstream::in);
    std::string str;

    // ignore first line
    std::getline(file, str);
    while (std::getline(file, str)){
        Point p(-1, -1);
        std::stringstream sstr(str);

        std::getline(sstr, str, ',');
        p.x = std::stoi(str);
        std::getline(sstr, str);
        p.y = std::stoi(str);
        points.push_back(p);
    }
    return points;
}

int main(){
    const auto points = readMarsFile(FILE_PATH "Marsroboter-Weka.csv");
    std::vector<Point> centroids = { Point(10, 10), Point(50, 50), Point(80, 50) };

    Cluster cluster;
    auto grouped = cluster.cluster(points, centroids);

    // text output
    std::cout << "Found centroids:\n\n";
    for (const auto& e : grouped){
        std::cout << "Centroid: " << e.first.x << ' ' << e.first.y << "\n\n";
        for (const auto& p : e.second)
            std::cout << p.x << ' ' << p.y << '\n';
        std::cout << "\n\n\n";
    }
    std::cout << std::endl;

    // visual output
    cf::WindowCoordinateSystem window(600, {0, 200}, {0, 200});

    const cf::Color colors[2*3] = {
        cf::Color::BLUE, cf::Color::CYAN,
        cf::Color::PINK, cf::Color::RED,
        cf::Color::GREY, cf::Color::BLACK
    };

    size_t colorIdx = 0;
    for (const auto& e : grouped){
        // visualize point
        window.drawPoint(cf::Point(e.first.x, e.first.y), colors[colorIdx++]);
    }
	return 0;
}
