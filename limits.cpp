#include <vector>

#include "helpers.h"
#include "limits.h"

//constants
const int tries = 1E+3; //num of checks for convergence before giving up
const long long def_inf = 1E+10; //if k>def_inf, considered to be inf

//functions
rep lim_to_inf(double (*f)(double), enum remark r) {
    /*
    exponentially increments value to check
    if converges, return value it converges to
    otherwise, check how f' behaves to see end behavior
    */

    //constants
    const float input_init = 1E+5;
    const float input_scaling = 10;

    //init types
    int i;
    bool first_inc;
    double curr_input, curr_output;
    double lim_output;
    std::vector<double> func_inputs, func_outputs, rate_of_change;

    //testing converging to what (assuming it does)
    curr_input = (r==POS_INF)? input_init:-input_init;

    for (i=0; i<tries; i++) {
        //get current func output
        curr_output = f(curr_input);
        //updating values
        func_inputs.push_back(curr_input);
        func_outputs.push_back(curr_output);
        curr_input*=input_scaling;

        //checking if can be considered inf
        if (curr_output > def_inf) {
            return rep{0, POS_INF};
        } else if (curr_output < -def_inf) {
            return rep{0, NEG_INF};
        }
        //checking if converges
        else if (i==0) {
            //bc first elem, so nothing before it
            continue;
        } else if (approx(func_outputs[i-1], curr_output, 1)) {
            lim_output = (func_outputs[i-1]+curr_output)/2;
            if (approx(lim_output,0,1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{lim_output,NONE};};
        }
    }

    //checking if lim converges at all
    //defining values
    for (i=0; i<tries-1; i++) {
        //filling rate_of_change
        rate_of_change.push_back(
            (func_outputs[i+1]-func_outputs[i]) /
            (func_inputs[i+1]-func_inputs[i])
        );

        //checking if can be considered inf
        if (rate_of_change[i] > def_inf) {
            return rep{0, POS_INF};
        } else if (rate_of_change[i] < -def_inf) {
            return rep{0, NEG_INF};
        }
    }
    //checking if rate_of_change is monotonic
    first_inc = rate_of_change[1]>rate_of_change[0];
    for (i=2; i<tries-1; i++) {
        //if not, return dne bc oscillates
        if ((rate_of_change[i]>rate_of_change[i-1])!=first_inc) {
            return rep{0, DNE};
        };
    }
    //return +- infinity as necessary
    return (rate_of_change[0]>0? rep{0, POS_INF}:rep{0, NEG_INF});
}
rep lim_to_side(double (*f)(double), rep x) {
    /*
    exponentially decays an offset from input
    if converges, return value it converges to
    otherwise, check how f' behaves to find how f behaves very close to x
    */

    //constants
    const float dx_init = 1;
    const float dx_decay = .05;

    //init types
    int i;
    bool first_inc;
    double curr_input, curr_output, dx;
    double lim_output;
    std::vector<double> func_inputs, func_outputs, rate_of_change;

    //checking converges to what (assuming it does)
    dx = (x.r==POS_SIDE)? dx_init:-dx_init;

    for (i=0; i<tries; i++) {
        //get round input, output values
        curr_input = x.v+dx;
        curr_output = f(curr_input);
        //updating values
        func_inputs.push_back(curr_input);
        func_outputs.push_back(curr_output);
        dx *= dx_decay;

        //checking values
        //checking if can be considered inf
        if (func_outputs[i] > def_inf) {
            return rep{0, POS_INF};
        } else if (func_outputs[i] < -def_inf) {
            return rep{0, NEG_INF};
        }
        //checking if converges
        else if (i==0) {
            //bc first elem, so nothing before it
            continue;
        } else if (approx(func_outputs[i-1], curr_output, 1)) {
            lim_output = (func_outputs[i-1]+curr_output)/2;
            if (approx(lim_output,0,1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{lim_output,NONE};};
        }
    }

    //checking if lim converges at all
    //defining values
    for (i=0; i<tries-1; i++) {
        //filling rate_of_change
        rate_of_change.push_back(
            (func_outputs[i+1]-func_outputs[i]) /
            (func_inputs[i+1]-func_inputs[i])
        );
        std::cout << "lts roc " << rate_of_change[i] << '\n';

        //checking if can be considered inf
        if (rate_of_change[i] > def_inf) {
            return rep{0, POS_INF};
        } else if (rate_of_change[i] < -def_inf) {
            return rep{0, NEG_INF};
        }
    }
    //checking if rate_of_change is monotonic
    first_inc = rate_of_change[1]>rate_of_change[0];
    for (i=2; i<tries-1; i++) {
        //if not, return dne bc oscillates
        std::cout << "lts fi " << first_inc << '\n';
        if ((rate_of_change[i]>rate_of_change[i-1])!=first_inc) {
            return rep{0, DNE};
        };
    }
    //return +- infinity as necessary
    return (rate_of_change[0]>0? rep{0, POS_INF}:rep{0, NEG_INF});
};