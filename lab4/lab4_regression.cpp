#include "windowCoordinateSystem.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>


typedef std::array<std::array<float, 12>, 9> Mat9x12;
enum Year{ Y1996, Y1997, Y1998, Y1999, Y2000, Y2001, Y2002, Y2003, Y2004, NUM_YEARS };
enum Month{ JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEZ, NUM_MONTHS };

Mat9x12 getMatrix(){
    Mat9x12 data;
    data[Y1996][JAN] = 138.2f; data[Y1996][FEB] = 137.5f; data[Y1996][MAR] = 138.4f; data[Y1996][APR] = 148.2f;
    data[Y1996][MAY] = 151.8f; data[Y1996][JUN] = 146.4f; data[Y1996][JUL] = 141.4f; data[Y1996][AUG] = 135.4f;
    data[Y1996][SEP] = 130.2f; data[Y1996][OCT] = 123.7f; data[Y1996][NOV] = 121.7f; data[Y1996][DEZ] = 122.6f;

    data[Y1997][JAN] = 123.4f; data[Y1997][FEB] = 124.3f; data[Y1997][MAR] = 125.3f; data[Y1997][APR] = 126.9f;
    data[Y1997][MAY] = 125.4f; data[Y1997][JUN] = 123.2f; data[Y1997][JUL] = 121.7f; data[Y1997][AUG] = 123.2f;
    data[Y1997][SEP] = 120.9f; data[Y1997][OCT] = 119.3f; data[Y1997][NOV] = 120.1f; data[Y1997][DEZ] = 120.8f;

    data[Y1998][JAN] = 120.2f; data[Y1998][FEB] = 118.3f; data[Y1998][MAR] = 117.9f; data[Y1998][APR] = 115.3f;
    data[Y1998][MAY] = 114.7f; data[Y1998][JUN] = 113.8f; data[Y1998][JUL] = 111.4f; data[Y1998][AUG] = 111.1f;
    data[Y1998][SEP] = 107.6f; data[Y1998][OCT] = 105.6f; data[Y1998][NOV] = 108.5f; data[Y1998][DEZ] = 108.1f;

    data[Y1999][JAN] = 108.1f; data[Y1999][FEB] = 108.3f; data[Y1999][MAR] = 109.5f; data[Y1999][APR] = 112.2f;
    data[Y1999][MAY] = 113.7f; data[Y1999][JUN] = 115.5f; data[Y1999][JUL] = 112.5f; data[Y1999][AUG] = 113.1f;
    data[Y1999][SEP] = 110.8f; data[Y1999][OCT] = 109.7f; data[Y1999][NOV] = 110.6f; data[Y1999][DEZ] = 111.8f;

    data[Y2000][JAN] = 112.8f; data[Y2000][FEB] = 114.0f; data[Y2000][MAR] = 115.5f; data[Y2000][APR] = 117.8f;
    data[Y2000][MAY] = 117.2f; data[Y2000][JUN] = 116.2f; data[Y2000][JUL] = 116.8f; data[Y2000][AUG] = 117.1f;
    data[Y2000][SEP] = 116.6f; data[Y2000][OCT] = 116.6f; data[Y2000][NOV] = 116.5f; data[Y2000][DEZ] = 116.6f;

    data[Y2001][JAN] = 117.5f; data[Y2001][FEB] = 117.5f; data[Y2001][MAR] = 116.6f; data[Y2001][APR] = 115.8f;
    data[Y2001][MAY] = 116.9f; data[Y2001][JUN] = 117.9f; data[Y2001][JUL] = 117.6f; data[Y2001][AUG] = 116.1f;
    data[Y2001][SEP] = 114.4f; data[Y2001][OCT] = 113.5f; data[Y2001][NOV] = 115.1f; data[Y2001][DEZ] = 115.4f;

    data[Y2002][JAN] = 115.4f; data[Y2002][FEB] = 115.2f; data[Y2002][MAR] = 114.5f; data[Y2002][APR] = 113.7f;
    data[Y2002][MAY] = 111.7f; data[Y2002][JUN] = 109.0f; data[Y2002][JUL] = 107.9f; data[Y2002][AUG] = 110.9f;
    data[Y2002][SEP] = 110.9f; data[Y2002][OCT] = 111.6f; data[Y2002][NOV] = 110.9f; data[Y2002][DEZ] = 110.0f;

    data[Y2003][JAN] = 109.3f; data[Y2003][FEB] = 108.8f; data[Y2003][MAR] = 107.3f; data[Y2003][APR] = 107.4f;
    data[Y2003][MAY] = 106.1f; data[Y2003][JUN] = 106.8f; data[Y2003][JUL] = 109.6f; data[Y2003][AUG] = 112.2f;
    data[Y2003][SEP] = 119.1f; data[Y2003][OCT] = 121.9f; data[Y2003][NOV] = 130.4f; data[Y2003][DEZ] = 131.8;

    data[Y2004][JAN] = 136.2f; data[Y2004][FEB] = 134.0f; data[Y2004][MAR] = 132.3f; data[Y2004][APR] = 132.4f;
    data[Y2004][MAY] = 122.4f; data[Y2004][JUN] = 119.7f; data[Y2004][JUL] = 115.2f; data[Y2004][AUG] = 108.3f;
    data[Y2004][SEP] = 104.7f; data[Y2004][OCT] = 103.6f; data[Y2004][NOV] = 103.9f; data[Y2004][DEZ] =  99.4f;
    return data;
}

Mat9x12 readDataSet(const char* filepath){
    Mat9x12 data;
    std::fstream file(filepath, std::fstream::in);

    size_t year = 0;
    for (std::string str; std::getline(file, str);){
        size_t month = 0;
        std::stringstream sstr(str);
        std::getline(sstr, str, ',');
        while (std::getline(sstr, str, ','))
            data[year][month++] = std::stof(str);
        ++year;
    }
    return data;
}


template<typename T>
std::tuple<T, T, T> parabolaFitting(const T* y_values, const T* x_values, size_t dataSize){
    // create parabalo function based on data
    // link to algorithm:   http://www.personal.psu.edu/jhm/f90/lectures/lsq2.html
    // possible implementation (pastebin.com -> license?)
    // http://pastebin.com/tUvKmGPn
    //
    // own implementation, differs from pastebin in last step
    T sum_t1 = T(0.0);
    T sum_t2 = T(0.0);
    T sum_t3 = T(0.0);
    T sum_t4 = T(0.0);

    T sum_dt0 = T(0.0);
    T sum_dt1 = T(0.0);
    T sum_dt2 = T(0.0);

    // calculate vector_d (right hand side)
    // and
    // calculate stum(t), sum(t^2), sum(t^3) and sum(t^4)
    for (size_t i = 0; i < dataSize; ++i){
        T temp_x = x_values[i];
        T temp_x_sq = temp_x * temp_x;

        T temp_y = y_values[i];

        // calculate right handside (vector_d)
        sum_dt0 += temp_y;
        sum_dt1 += temp_y * temp_x;
        sum_dt2 += temp_y * temp_x_sq;

        // calculate matrix values
        sum_t1 += temp_x;
        sum_t2 += temp_x_sq;
        sum_t3 += temp_x_sq * temp_x;
        sum_t4 += temp_x_sq * temp_x_sq;
    }

    T c, b, a;

/* implementation without substitution
    a = (sum_dt2 - sum_t2 * sum_dt0 / dataSize - (sum_dt1 * dataSize - sum_t1 * sum_dt0) / (sum_t2 * dataSize - sum_t1 * sum_t1) * (sum_t3 - sum_t1 * sum_t2 / dataSize))
        /
        ((sum_t1 * sum_t2 - sum_t3 * dataSize) / (sum_t2 * dataSize - sum_t1 * sum_t1) * (sum_t3 - sum_t1 * sum_t2 / dataSize) + sum_t4 - sum_t2 * sum_t2 / dataSize);


    b = (sum_dt1 * dataSize - sum_t1 * sum_dt0 + a * (sum_t1 * sum_t2 - sum_t3 * dataSize) ) / (sum_t2 * dataSize - sum_t1 * sum_t1);
    c = (sum_dt0 - sum_t1 * b - sum_t2 * a) / dataSize;
*/

    /*
    algorithm found at:
    http://www.personal.psu.edu/jhm/f90/lectures/lsq2.html
    http://stackoverflow.com/questions/11233404/simple-curve-fitting-implimentation-in-c-svd-least-sqares-fit-or-similar

    different approach found at:
    http://pastebin.com/tUvKmGPn


    equations to solve:
    n  *  c  + t1 *  b  + t2 *  a   = d0        <=>     c = (d0 - t1 * b - t2 * a) / n  I
    t1 *  c  + t2 *  b  + t3 *  a   = d1    II
    t2 *  c  + t3 *  b  + t4 *  a   = d2    III



    I in II:
    --------

    t1 * (d0 - t1 * b - t2 * a) / n + t2 * b + t3 * a = d1


            -t1^2 * b                     t1 * d0        t1 * t2 * a
    <=>     ----------  + t2 * b = d1 -  ---------  +   -------------    + t2 * a
                n                            n                n


                    d1 * n - t1 * d0                 t1 * t2 - t3 * n
    <=>     b =    ------------------   +   a *     ------------------              IV
                     t2 * n - t1^2                    t2 * n - t1^2




    I in III:
    ---------

    t1 * (d0 - t1 * b - t2 * a / n + t3 * b + t4 * a = d2


             t2 * d0         t1 * t2                 t2^2
    <=>     ---------   -   ----------  * b     -   -------  * a    + t3 * b  + t4 * a = d2
                n               n                       n


                 /           t2^2   \            /           t2^2   \                t2 * d0
    <=>     b *  |  t3  -   ------  |   +   a *  |   t4 -   ------  |   =   d2 -    ----------          V
                 \            n     /            \             n    /                   n




    IV in V:
    --------

        /    d1 * n - t1 * 0            t1 * t2 - t3 * n    \       /            t1 * t2    \            /           t2^2    \
        |   -----------------   + a *  ------------------   |   *   |   t3  -   ---------   |    +   a * |  t4  -   ------   |
        \     t2 * n - t1^2             t2 * n - t1^2       /       \               n       /            \            n      /

                t2 * d0          /    d1 * n -t1 * d0    \       /            t1 * t2    \
    =   d2 -   ---------     -   |   -----------------   |   *   |   t3 -    ---------   |
                   n             \     t2 * n - t1^2     /       \               n       /




                /   /  t1 * t2 - t3 * n   \       /          t1 * t2    \       /            t2^2   \
    <=>     a * |   | ------------------  |   *   |   t3 -  ---------   |   +   |   t4  -   ------  |
                \   \   t2 * n - t1^2     /       \             n       /       \             n     /


                     t2 * d0        /   d1 * n - t1 * d0    \       /            t1 * t2    \
        =   d2  -   ---------   -   |  ------------------   |   *   |   t4  -   ----------  |
                        n           \    t2 * n - t1^2      /       \               n       /



    TODO last step



    variable description:

    a, b and c are the parabola coefficients:
    f(x) = a * x^2 + b * x + c

    n   = number of data points                     (dataSize   in this implementation)

    t1  = sum of x  ,    values from 0 to size-1    (sum_t1     in this implementation)
    t2  = sum of x^2,    values from 0 to size-1    (sum_t2     in this implementation)
    t3  = sum of x^3,    values from 0 to size-1    (sum_t3     in this implementation)
    t4  = sum of x^4,    values from 0 to size-1    (sum_t4     in this implementation)

    dt0 = sum of y*x^0,  values from 0 to size-1    (sum_dt0    in this implementation)
    dt1 = sum of y*x^1,  values from 0 to size-1    (sum_dt1    in this implementation)
    dt2 = sum of y*x^2,  values from 0 to size-1    (sum_dt2    in this implementation)
    */

    T substition_1 = sum_t2 * dataSize - sum_t1 * sum_t1;
    T substition_2 = sum_t3 - sum_t1 * sum_t2 / dataSize;


    a = (sum_dt2 - sum_t2 * sum_dt0 / dataSize - ((sum_dt1 * dataSize - sum_t1 * sum_dt0) / substition_1) * substition_2)
        /
        ( ((sum_t1 * sum_t2 - sum_t3 * dataSize) / substition_1) * substition_2 + sum_t4 - sum_t2 * sum_t2 / dataSize);


    b = (sum_dt1 * dataSize - sum_t1 * sum_dt0 + a * (sum_t1 * sum_t2 - sum_t3 * dataSize) ) / substition_1;
    c = (sum_dt0 - sum_t1 * b - sum_t2 * a) / dataSize;
    return {a, b, c};
}


std::pair<float, float> getRegressionLine(const Mat9x12& data){
    const float avg_x = float(Year::NUM_YEARS * Month::NUM_MONTHS) / 2.f;

    float avg_y = 0.f;
    for (size_t month = 0; month < data[0].size(); ++month){
    for (size_t year  = 0; year  < data   .size(); ++year ){
        avg_y += data[year][month];
    }
    }
    avg_y /= float(Year::NUM_YEARS * Month::NUM_MONTHS);

    auto covarianz = [&avg_x, &avg_y, &data](){
        float result = 0.f;
        for (size_t month = 0; month < data[0].size(); ++month){
        for (size_t year  = 0; year  < data   .size(); ++year ){
            result += (data[year][month] - avg_y) * ((year * Month::NUM_MONTHS + month) - avg_x);
        }
        }
        return result;
    };
    auto varianz = [&avg_x](){
        float result = 0.f;
        for (size_t i = 0; i < Year::NUM_YEARS * Month::NUM_MONTHS; ++i)
            result += (i - avg_x) * (i - avg_x);
        return result;
    };
    const float b2 = covarianz() / varianz();
    return {b2, avg_y - b2 * avg_x};
}

int main(){
    //const auto data = getMatrix();
    const auto data = readDataSet(FILE_PATH "regression_dataset.csv");
    const auto line = getRegressionLine(data);
    std::cout << "Found line:\ny = " << line.first << "x + " << line.second << std::endl;

    std::vector<float> x(Year::NUM_YEARS * Month::NUM_MONTHS), y(Year::NUM_YEARS * Month::NUM_MONTHS);
    for (size_t month = 0; month < data[0].size(); ++month){
    for (size_t year  = 0; year  < data   .size(); ++year ){
        const size_t idx = year * Month::NUM_MONTHS + month;
        x[idx] = idx;
        y[idx] = data[year][month];
    }
    }

    const auto parabola = parabolaFitting(y.data(), x.data(), Year::NUM_YEARS * Month::NUM_MONTHS);
    std::cout << "Fitted parabola:\n"
              << std::get<0>(parabola) << "x² + "
              << std::get<1>(parabola) << "x + "
              << std::get<2>(parabola) << std::endl;

    cf::WindowCoordinateSystem system(400, {-1.f, float(Year::NUM_YEARS * Month::NUM_MONTHS)}, {-1.f, 150.f});
    auto draw = [&line, &parabola, &system, &data](){
        system.drawAxis(cf::Color::RED, 100.f, 100.f);
        for (size_t month = 0; month < data[0].size(); ++month){
        for (size_t year  = 0; year  < data   .size(); ++year ){
            system.drawPoint({float(year * Month::NUM_MONTHS + month), data[year][month]});
        }
        }
        system.show();
        system.waitKey();
        system.drawLinearEquation(line.first, line.second, cf::Color::BLUE, cf::WindowCoordinateSystem::LineType::DEFAULT, 2);
        system.show();
        system.waitKey();

        auto calculateY = [&parabola](float x){
            return std::get<0>(parabola) * x * x + std::get<1>(parabola) * x + std::get<2>(parabola);
        };
        cf::Point last(-1.f, calculateY(-1.f));
        for (float x = 0.f; x < Year::NUM_YEARS * Month::NUM_MONTHS + 1.f; ++x){
            const cf::Point p(x, calculateY(x));
            system.drawLine(last, p, cf::Color::GREEN, cf::WindowCoordinateSystem::LineType::DEFAULT, 2);
            last = p;
        }
        system.show();
        system.waitKey();
    };
    draw();
    system.clear();
    system.setInterval({-1.f, float(Year::NUM_YEARS * Month::NUM_MONTHS)}, {95.f, 150.f}, 1280);
    draw();
    return 0;
}




