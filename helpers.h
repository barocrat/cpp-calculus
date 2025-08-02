#pragma once

#include <cmath>
#include <iostream>

//structures
enum remark {
    POS_INF,    //0
    NEG_INF,    //1
    POS_SIDE,   //2
    NEG_SIDE,   //3
    DNE,        //4
    NONE,       //5
};
struct rep{
    double v; //value
    enum remark r; //remark
};

//functions
inline bool approx(double a, double b, int tol_lvl) {
    const float neglig = 1E-10; //negligible value
    const float hard_tolerance = 1E-5; //actual diff to hit

    float soft_tolerance; //percentage diff needed
    switch (tol_lvl) { //more tol_lvl = stricter tolerance
        case 0: soft_tolerance=1E-5; break;
        case 1: soft_tolerance=1E-10; break;
    }

    return
        200*std::abs((a-b)/(a+b+neglig))<soft_tolerance ||
            //using slightly modified midpoint percent
            //to avoid div by zero
        std::abs(a-b)<hard_tolerance;
    ;
}