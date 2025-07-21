#include "limits.h" //includes helpers
#include <stdexcept>

/*
limits
*/
rep lim(double (*f)(double), rep x) {
    switch (x.r) {
        case 0: return lim_to_inf(f, POS_INF); //pos inf
        case 1: return lim_to_inf(f, NEG_INF); //neg inf
        case 2: return lim_to_side(f, x); //pos side
        case 3: return lim_to_side(f, x); //neg side
        case 4: throw invalid_argument( //dne (invalid input)
            "x passed to lim can not have remark of dne"
        );
        case 5: //normal limit
            rep neg_side, pos_side;

            pos_side = lim(f, rep{x.v, POS_SIDE});
            neg_side = lim(f, rep{x.v, NEG_SIDE});

            if (neg_side.r != pos_side.r) {
                //if the remarks dont match
                return rep{0, DNE};
            } else if (neg_side.r != NONE) {
                //if remarks match but still exist
                return neg_side;
            } else if (approx(neg_side.v, pos_side.v, 0)) {
                //if values do match and no remark exists
                return rep{
                    (neg_side.v+pos_side.v)/2,
                    NONE
                };
            } else {
                //if values don't match and no remark exists
                return rep{0, DNE};
            }
    }
    //so gpp doesnt attack me
    return rep{0, NONE};
}

int main() { //just testing
    cout.precision(20);

    rep evald_lim = lim([](double x) -> double {
        return pow(1+1/x, x);
    }, rep{0, POS_INF});

    cout << evald_lim.v << ' ' << evald_lim.r;

    return 0;
}