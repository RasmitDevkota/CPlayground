#include "helper.h"

int underlying_function(double *x, double uv[4])
{
    for (int i = 0; i < 4; i++)
	*x += 0.5 * i * (uv[i] + *x) * (uv[i] - *x) + 1;

    return 0;
}

