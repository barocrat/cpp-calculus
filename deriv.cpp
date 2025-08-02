#include <vector>

#include "helpers.h"
#include "limits.h"
#include "deriv.h"

//constants
const int tries = 1E+3; //num of checks for convergence before giving up
const long long def_inf = 1E+10; //if k>def_inf, considered to be inf

//functions
rep deriv_to_inf(double (*f)(double), enum remark r) {
    /*
    generates rate of change w/ exponentially increased 
    if converges, return value it converges to
    otherwise, check how f'' behaves to see end behavior
    */

    //check for easy cases
    switch (lim_to_inf(f, r).r) {
        case 4: return rep{0, DNE};
        case 5: return rep{0, NONE};
    }

    //constants
    const float x_toinf_init = 1E+5;
    const float toinf_factor = 10;

    //init types
    int i;
    double prev_func_output;
    double curr_input, curr_output, curr_rate;
    double deriv_output;
    std::vector<double> func_inputs, rate_of_change;

    //checking what deriv converges to (if at all)
    curr_input = (r==POS_INF)? x_toinf_init:-x_toinf_init;

    for (i=0; i<tries; i++) {
        //get current func output
        curr_output = f(curr_input);
        //continue if first iteration
        if (i==0) {prev_func_output=curr_output; continue;};
        //updating values
        func_inputs.push_back(curr_input);
        rate_of_change.push_back(
            (curr_output/prev_func_output)/
            (curr_input/func_inputs[i-1])
        );
        curr_input *= toinf_factor;
        prev_func_output = curr_output;

        //check if can be considered inf
        curr_rate = rate_of_change[i];
        if (curr_rate>def_inf) {
            return rep{0, POS_INF};
        } else if (curr_rate<-def_inf) {
            return rep{0, NEG_INF};
        }
        //check if curr raofch approx prev
        else if (approx(curr_rate, rate_of_change[i-1], 1)) {
            deriv_output = (curr_rate+rate_of_change[i-1])/2;
            if (approx(deriv_output, 0, 1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{deriv_output, NONE};}
        }
    }

    //no check for concavity behavior since converging, oscillating cases alr handled

    //return +- infinity as necessary
    return (rate_of_change[1]>rate_of_change[0])? rep{0,POS_INF}:rep{0,NEG_INF};
}
rep deriv_to_side(double(*f)(double), rep x) {
    /*
    generates rate of change w/ exponentially decaying offset
    if converges, return value it converges to
    otherwise, check how f'' behaves very close to x
    */

    //check for easy cases (divergence)
    switch (lim_to_side(f, x).r) {
        case 0: return rep{0, POS_INF};
        case 1: return rep{0, NEG_INF};
        case 4: return rep{0, DNE};
    }

    //constants
    const float dx_init = 1;
    const float dx_decay = .05;

    //init types
    int i;
    double prev_func_output;
    double curr_input, dx;
    double curr_output, curr_rate;
    double deriv_output;
    std::vector<double> func_inputs, rate_of_change;

    //checking what deriv converges to
    dx = (x.r==POS_SIDE)? dx_init:-dx_init;

    for (i=0; i<tries; i++) {
        //get current input, output
        curr_input = x.v+dx;
        curr_output = f(curr_input);
        //continue if first iteration
        if (i==0) {prev_func_output=curr_output; continue;};
        //updating values
        func_inputs.push_back(curr_input);
        rate_of_change.push_back(
            (curr_output/prev_func_output)/
            (curr_input/func_inputs[i-1])
        );
        dx *= dx_decay;
        prev_func_output = curr_output;

        //check if can be considered inf
        curr_rate = rate_of_change[i];
        if (curr_rate>def_inf) {
            return rep{0, POS_INF};
        } else if (curr_rate<-def_inf) {
            return rep{0, NEG_INF};
        }
        //check if curr raofch approx prev
        else if (approx(curr_rate, rate_of_change[i-1], 1)) {
            deriv_output = (curr_rate+rate_of_change[i-1])/2;
            if (approx(deriv_output, 0, 1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{deriv_output, NONE};}
        }
    }

    //no check for concavity behavior since converging, oscillating cases alr handled

    //return best approximation for deriv
    if (approx(rate_of_change[tries-1], 0, 1)) {return rep{0, NONE};}
        //clamp to zero if extremely close
    else {return rep{rate_of_change[tries-1], NONE};}
}