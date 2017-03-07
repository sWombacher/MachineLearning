#include "windowCoordinateSystem.hpp"
#include <iostream>
#include <array>


typedef std::array<std::array<double, 12>, 9> Mat9x12;
enum Year{ Y1996, Y1997, Y1998, Y1999, Y2000, Y2001, Y2002, Y2003, Y2004, NUM_YEARS };
enum Month{ JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEZ, NUM_MONTHS };

Mat9x12 getMatrix(){
    Mat9x12 data;
    data[Y1996][JAN] = 138.2; data[Y1996][FEB] = 137.5; data[Y1996][MAR] = 138.4; data[Y1996][APR] = 148.2;
    data[Y1996][MAY] = 151.8; data[Y1996][JUN] = 146.4; data[Y1996][JUL] = 141.4; data[Y1996][AUG] = 135.4;
    data[Y1996][SEP] = 130.2; data[Y1996][OCT] = 123.7; data[Y1996][NOV] = 121.7; data[Y1996][DEZ] = 122.6;

    data[Y1997][JAN] = 123.4; data[Y1997][FEB] = 124.3; data[Y1997][MAR] = 125.3; data[Y1997][APR] = 126.9;
    data[Y1997][MAY] = 125.4; data[Y1997][JUN] = 123.2; data[Y1997][JUL] = 121.7; data[Y1997][AUG] = 123.2;
    data[Y1997][SEP] = 120.9; data[Y1997][OCT] = 119.3; data[Y1997][NOV] = 120.1; data[Y1997][DEZ] = 120.8;

    data[Y1998][JAN] = 120.2; data[Y1998][FEB] = 118.3; data[Y1998][MAR] = 117.9; data[Y1998][APR] = 115.3;
    data[Y1998][MAY] = 114.7; data[Y1998][JUN] = 113.8; data[Y1998][JUL] = 111.4; data[Y1998][AUG] = 111.1;
    data[Y1998][SEP] = 107.6; data[Y1998][OCT] = 105.6; data[Y1998][NOV] = 108.5; data[Y1998][DEZ] = 108.1;

    data[Y1999][JAN] = 108.1; data[Y1999][FEB] = 108.3; data[Y1999][MAR] = 109.5; data[Y1999][APR] = 112.2;
    data[Y1999][MAY] = 113.7; data[Y1999][JUN] = 115.5; data[Y1999][JUL] = 112.5; data[Y1999][AUG] = 113.1;
    data[Y1999][SEP] = 110.8; data[Y1999][OCT] = 109.7; data[Y1999][NOV] = 110.6; data[Y1999][DEZ] = 111.8;

    data[Y2000][JAN] = 112.8; data[Y2000][FEB] = 114.0; data[Y2000][MAR] = 115.5; data[Y2000][APR] = 117.8;
    data[Y2000][MAY] = 117.2; data[Y2000][JUN] = 116.2; data[Y2000][JUL] = 116.8; data[Y2000][AUG] = 117.1;
    data[Y2000][SEP] = 116.6; data[Y2000][OCT] = 116.6; data[Y2000][NOV] = 116.5; data[Y2000][DEZ] = 116.6;

    data[Y2001][JAN] = 117.5; data[Y2001][FEB] = 117.5; data[Y2001][MAR] = 116.6; data[Y2001][APR] = 115.8;
    data[Y2001][MAY] = 116.9; data[Y2001][JUN] = 117.9; data[Y2001][JUL] = 117.6; data[Y2001][AUG] = 116.1;
    data[Y2001][SEP] = 114.4; data[Y2001][OCT] = 113.5; data[Y2001][NOV] = 115.1; data[Y2001][DEZ] = 115.4;

    data[Y2002][JAN] = 115.4; data[Y2002][FEB] = 115.2; data[Y2002][MAR] = 114.5; data[Y2002][APR] = 113.7;
    data[Y2002][MAY] = 111.7; data[Y2002][JUN] = 109.0; data[Y2002][JUL] = 107.9; data[Y2002][AUG] = 110.9;
    data[Y2002][SEP] = 110.9; data[Y2002][OCT] = 111.6; data[Y2002][NOV] = 110.9; data[Y2002][DEZ] = 110.0;

    data[Y2003][JAN] = 109.3; data[Y2003][FEB] = 108.8; data[Y2003][MAR] = 107.3; data[Y2003][APR] = 107.4;
    data[Y2003][MAY] = 106.1; data[Y2003][JUN] = 106.8; data[Y2003][JUL] = 109.6; data[Y2003][AUG] = 112.2;
    data[Y2003][SEP] = 119.1; data[Y2003][OCT] = 121.9; data[Y2003][NOV] = 130.4; data[Y2003][DEZ] = 131.8;

    data[Y2004][JAN] = 136.2; data[Y2004][FEB] = 134.0; data[Y2004][MAR] = 132.3; data[Y2004][APR] = 132.4;
    data[Y2004][MAY] = 122.4; data[Y2004][JUN] = 119.7; data[Y2004][JUL] = 115.2; data[Y2004][AUG] = 108.3;
    data[Y2004][SEP] = 104.7; data[Y2004][OCT] = 103.6; data[Y2004][NOV] = 103.9; data[Y2004][DEZ] =  99.4;
    return data;
}

std::pair<float, float> getRegressionLine(const Mat9x12& data){
    float m = 0.0, t = 0.0;
    return {m, t};
}

int main(){
    const Mat9x12 data = getMatrix();
    const auto line = getRegressionLine(data);
    std::cout << "Found line:\ny = " << line.first << "x + " << line.second << std::endl;

    cf::WindowCoordinateSystem system(400, {-1.f, float(Year::NUM_YEARS * Month::NUM_MONTHS)}, {-1.f, 150.f});
    auto draw = [&line, &system, &data](){
        system.drawAxis(cf::Color::RED, 100.f, 100.f);
        for (size_t month = 0; month < data[0].size(); ++month){
        for (size_t year  = 0; year  < data   .size(); ++year ){
            system.drawPoint({float(year * Month::NUM_MONTHS + month), float(data[year][month])});
        }
        }
        system.drawLinearEquation(line.first, line.second, cf::Color::BLUE);
        system.show();
        system.waitKey();
    };
    draw();
    system.clear();
    system.setInterval({-1.f, float(Year::NUM_YEARS * Month::NUM_MONTHS)}, {90.f, 150.f}, 600);
    draw();
    return 0;
}




