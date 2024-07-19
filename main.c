#include <stdio.h>
#include <math.h>
#include <nlopt.h>

int underlying_function(double *x, double uv[4])
{
    for (int i = 0; i < 4; i++)
	*x += 0.5 * i * (uv[i] + *x) * (uv[i] - *x) + 1;

    return 0;
}

extern double __enzyme_autodiff(void*, double); 
int enzyme_const, enzyme_dup, enzyme_out;

double calc(double x)
{
    double underlying_values[4];
    for (int i = 0; i < 4; i++)
        underlying_values[i] = 0.0;

    underlying_function(&x, underlying_values);

    double factor = 0.0;
    for (int i = 0; i < 4; i++)
	factor += underlying_values[i];

    return factor * (1+x) * (1-x) * 1.0E-03 + x * x;
}

double obj(unsigned n, const double *x, double *grad, void *my_func_data)
{
    double val_x = 0.0;
    for (int i = 0; i < n; i++)
	val_x += calc(x[i]);

    // if (grad)
    // {
	for (int i = 0; i < n; i++)
	    grad[i] = 0.0;

	for (int i = 0; i < n; i++)
	    // Analytical derivative
	    // NotImplemented

	    // EnzymeAD derivative
	    grad[i] = __enzyme_autodiff((void*)calc, x[i]);
    // }

    return val_x;
}

int main()
{
    nlopt_opt opt;
    
    opt = nlopt_create(NLOPT_LD_CCSAQ, 2); /* algorithm and dimensionality */
    nlopt_set_min_objective(opt, obj, NULL);
    
    double lb[2] = { -10.0, -10.0 };
    double ub[2] = { +10.0, +10.0 };
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    
    nlopt_set_maxeval(opt, 1024);
    nlopt_set_ftol_rel(opt, 1e-6);
    nlopt_set_xtol_rel(opt, 1e-9);
    
    double x[2] = { 1.234, 5.678 };  /* `*`some` `initial` `guess`*` */
    double minf; /* `*`the` `minimum` `objective` `value,` `upon` `return`*` */
    if (nlopt_optimize(opt, x, &minf) < 0) {
        printf("nlopt failed!\n");
    }
    else {
        printf("found minimum at f(%g,%g) = %0.10g\n", x[0], x[1], minf);
    }

    nlopt_destroy(opt);
    
    return 0;
}

