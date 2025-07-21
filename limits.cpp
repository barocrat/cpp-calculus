#include "limits.h"
#include <vector>
using namespace std;

//constants

const int tries = 1E+3;
    //num of checks to see if lim converges
    //before giving up and checking end behavior
const long long def_inf = 1E+10;
    //if k larger than def_inf, k considered to be inf

//functions
rep lim_to_inf(double (*f)(double), enum remark r) {
    /*
    exponentially increments value to check
    if converges, return value it converges to
    otherwise, check how f' behaves to see end behavior
    */

    //constants
    const float x_toinf_init = 1E+5;
    const float toinf_factor = 10;

    //init
    int i;
    bool first_inc;
    double x_tocheck, output_val, curr_out;
    vector<double> func_inputs, func_outputs, raofch;
        //raofch is rate of change

    //testing converging to what
    x_tocheck = (r==POS_INF)? x_toinf_init:-x_toinf_init;
        //no error handling since its only called
        //with a hard set r remark value passed in main.cpp
    for (i=0; i<tries; i++) {
        //get current value
        curr_out = f(x_tocheck);
        //updating values
        func_inputs.push_back(x_tocheck);
        func_outputs.push_back(curr_out);
        x_tocheck*=toinf_factor;

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
        } else if (approx(func_outputs[i-1], func_outputs[i], 1)) {
            output_val = (func_outputs[i-1]+func_outputs[i])/2;
            if (approx(output_val,0,1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{output_val,NONE};};
        }
    }

    //checking if raofch converges at all
    //defining values
    for (i=0; i<tries-1; i++) {
        //filling raofch
        raofch.push_back(
            (func_outputs[i+1]-func_outputs[i]) /
            (func_inputs[i+1]-func_inputs[i])
        );

        //checking if can be considered inf
        if (raofch[i] > def_inf) {
            return rep{0, POS_INF};
        } else if (raofch[i] < -def_inf) {
            return rep{0, NEG_INF};
        }
    }
    //confirming raofch is monotonic
    first_inc = raofch[1]>raofch[0];
    for (i=2; i<tries-1; i++) {
        //otherwise, return dne bc oscillates
        if ((raofch[i]>raofch[i-1])!=first_inc) {
            return rep{0, DNE};
        };
    }
    //return +- infinity as necessary
    return (raofch[0]>0? rep{0, POS_INF}:rep{0, NEG_INF});
}

rep lim_to_side(double (*f)(double), rep x) {
    /*
    exponentially decays an offset from input
    if converges, return value it converges to
    otherwise, check how f' behaves to find how f behaves very close to x
    */

    //constants
    const float dx_init = 1;
    const float dx_decay_factor = .05;

    //init
    int i;
    bool first_inc;
    double x_tocheck_r, dx, output_val, curr_out;
    vector<double> func_inputs, func_outputs, raofch;
        //raofch is rate of change

    dx = (x.r==POS_SIDE)? dx_init:-dx_init;
        //x remark already checked in main.cpp

    for (i=0; i<tries; i++) {
        //get round input, output values
        x_tocheck_r = x.v+dx;
        curr_out = f(x_tocheck_r);
        //updating values
        func_inputs.push_back(x_tocheck_r);
        func_outputs.push_back(curr_out);
        dx *= dx_decay_factor;

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
        } else if (approx(func_outputs[i-1], func_outputs[i], 1)) {
            output_val = (func_outputs[i-1]+func_outputs[i])/2;
            if (approx(output_val,0,1)) {return rep{0, NONE};}
                //clamp to zero if extremely close
            else {return rep{output_val,NONE};};
        }
    }

    //checking if converges at all
    //defining values
    for (i=0; i<tries-1; i++) {
        //filling raofch
        raofch.push_back(
            (func_outputs[i+1]-func_outputs[i]) /
            (func_inputs[i+1]-func_inputs[i])
        );

        //checking if can be considered inf
        if (raofch[i] > def_inf) {
            return rep{0, POS_INF};
        } else if (raofch[i] < -def_inf) {
            return rep{0, NEG_INF};
        }
    }
    //confirming raofch is monotonic
    first_inc = raofch[1]>raofch[0];
    for (i=2; i<tries-1; i++) {
        //otherwise, return dne bc oscillates
        if ((raofch[i]>raofch[i-1])!=first_inc) {
            return rep{0, DNE};
        };
    }
    //return +- infinity as necessary
    return (raofch[0]>0? rep{0, POS_INF}:rep{0, NEG_INF});
};