#pragma once

#include "helpers.h"

rep deriv_to_inf(double (*)(double), enum remark);
rep deriv_to_side(double (*)(double), rep);