#include "limits.h"

//constants ->

/*
NOTE
x_tocheck needs to be so that it samples a (decently) random point
*/

const int tries = 1E+3;
    //num of checks to see if lim converges
    //before giving up and checking end behavior
const long long def_inf = 1E+10;

//functions ->
rep lim_to_inf(double (*f)(double), enum remark r) {
    /*
    exponentially increments value to check
    if converges, return value it converges to
    otherwise, check how f' behaves to see end behavior
    */

    //constants ->
    const float x_toinf_init = 100;
    const float toinf_factor = 10;

    //init ->
    int i;
    bool first_inc;
    double x_tocheck, output;

    //testing converging to what ->
    double* ind_val = new double[2*tries];
        /*
        alternates ind -> value -> next ind ...
        so: ith ind = ind_val[2*i], ith value = ind_val[2*i+1]
        */
    x_tocheck = (r==POS_INF)? x_toinf_init:-x_toinf_init;
        //no error handling since its only called
        //with a hard set r remark value passed in main.cpp
    for (i=0; i<tries; i++) {
        //defining values
        ind_val[2*i] = x_tocheck;
        ind_val[2*i+1] = f(x_tocheck);
        x_tocheck*=toinf_factor;

        //checking values ->
        //checking if can be considered inf
        if (ind_val[2*i+1] > def_inf) {
            delete[] ind_val;
            return rep{0, POS_INF};
        } else if (ind_val[2*i+1] < -def_inf) {
            delete[] ind_val;
            return rep{0, NEG_INF};
        }
        //checking if converges
        else if (i==0) {
            //bc first elem, so nothing before it
            continue;
        } else if (approx(ind_val[2*i-1], ind_val[2*i+1], 1)) {
            output = (ind_val[2*i-1]+ind_val[2*i+1])/2;
            delete[] ind_val;
            return rep{output, NONE};
        }
    }

    //checking if converges at all ->
    //defining values
    double* roc = new double[tries-1]; //rate of change
    for (i=0; i<tries-1; i++) {
        //filling roc
        roc[i] =
            (ind_val[2*i+3]-ind_val[2*i+1]) /
            (ind_val[2*i+2]-ind_val[2*i])
        ;

        //checking if can be considered inf
        if (roc[i] > def_inf) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, POS_INF};
        } else if (roc[i] < -def_inf) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, NEG_INF};
        }
    }
    //confirming roc is monotonic
    first_inc = roc[1]>roc[0];
    for (i=2; i<tries-1; i++) {
        //otherwise, return dne bc oscillates
        if ((roc[i]>roc[i-1])!=first_inc) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, DNE};
        };
    }
    //final cleaning + return
    delete[] ind_val;
    delete[] roc;
    if (roc[0]>0) { //return if increasing
        return rep{0, POS_INF};
    } else { //return if decreasing
        return rep{0, NEG_INF};
    }
}

rep lim_to_side(double (*f)(double), rep x) {
    /*
    exponentially decays an offset from input
    if converges, return value it converges to
    otherwise, check how f' behaves to find end behavior
    */

    //constants ->
    const float dx_init = 1;
    const float dx_decay_factor = .05;

    //init ->
    int i;
    bool first_inc;
    double x_tocheck_r, dx, output;

    //testing converging to what ->
    double* ind_val = new double[2*tries];
        /*
        alternates ind -> value -> next ind ...
        so: ith ind = ind_val[2*i], ith value = ind_val[2*i+1]
        */

    dx = (x.r==POS_SIDE)? dx_init:-dx_init;
        //x remark already checked in main.cpp

    for (i=0; i<tries; i++) {
        //calculate x_tocheck_r
        x_tocheck_r = x.v+dx;

        //defining values
        ind_val[2*i] = x_tocheck_r;
        ind_val[2*i+1] = f(x_tocheck_r);
        dx *= dx_decay_factor;

        //checking values ->
        //checking if can be considered inf
        if (ind_val[2*i+1] > def_inf) {
            delete[] ind_val;
            return rep{0, POS_INF};
        } else if (ind_val[2*i+1] < -def_inf) {
            delete[] ind_val;
            return rep{0, NEG_INF};
        }
        //checking if converges
        else if (i==0) {
            //bc first elem, so nothing before it
            continue;
        } else if (approx(ind_val[2*i-1], ind_val[2*i+1], 1)) {
            output = (ind_val[2*i-1]+ind_val[2*i+1])/2;
            delete[] ind_val;
            return rep{output, NONE};
        }
    }

    //checking if converges at all ->
    //defining values
    double* roc = new double[tries-1]; //rate of change
    for (i=0; i<tries-1; i++) {
        //filling roc
        roc[i] =
            (ind_val[2*i+3]-ind_val[2*i+1]) /
            (ind_val[2*i+2]-ind_val[2*i])
        ;

        //checking if can be considered inf
        if (roc[i] > def_inf) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, POS_INF};
        } else if (roc[i] < -def_inf) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, NEG_INF};
        }
    }
    //confirming roc is monotonic
    first_inc = roc[1]>roc[0];
    for (i=2; i<tries-1; i++) {
        //otherwise, return dne bc oscillates
        if ((roc[i]>roc[i-1])!=first_inc) {
            delete[] ind_val;
            delete[] roc;
            return rep{0, DNE};
        };
    }
    //final cleaning + return
    delete[] ind_val;
    delete[] roc;
    if (roc[0]>0) { //return if increasing
        return rep{0, POS_INF};
    } else { //return if decreasing
        return rep{0, NEG_INF};
    }
};