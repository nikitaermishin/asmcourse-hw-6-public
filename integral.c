#include <stdio.h>
#include <getopt.h>
#include <math.h>

typedef double func(double);

double root(func *, func *, double, double, double, int *);
double integral(func *, double, double, double);
double compute_integral(func *, double, double, int);
int test_root(const char *);
int test_integral(const char *);

const char *SHORT_OPTIONS = "hriR:I:";
const struct option LONG_OPTIONS[] = {
    {"help", no_argument, 0, 'h'},
    {"root", no_argument, 0, 'r'},
    {"iterations", no_argument, 0, 'i'},
    {"test-root", required_argument, 0, 'R'},
    {"test-integral", required_argument, 0, 'I'},
    {0, 0, 0,0}
};

const double EPS1 = 0.00003;
const double EPS2 = 0.00005;

const int MAX_FUNC_INDEX = 6;
const int MIN_FUNC_INDEX = 0;

const int TEST_ROOT_PARAM_COUNT = 6;
const int TEST_INTEGRAL_PARAM_COUNT = 5;

func f1;
func f2;
func f3;
func f4;
func f5;
func f6;
func f7;

func *func_arr[] = {f1, f2, f3, f4, f5, f6, f7};

int main(int argc, char* argv[])
{
    int iter_count1, iter_count2, iter_count3, code, show_iterations, show_roots;
    double x12, x23, x13, i1, i2, i3;

    show_iterations = 0;
    show_roots = 0;

    while (1) {
        code = getopt_long(argc, argv, SHORT_OPTIONS,
                           LONG_OPTIONS, NULL);

        if (code == -1) {
            break;
        }

        switch (code) {
            case 'h':
                printf("Usage: ./integral [OPTION]\n");
                printf("    Calculate area of curvilinear triangle, which "
                       "sides are given by 3 single variable equations\n\n");
                printf("    Options:\n");
                printf("      %-20s\tdisplay this help and exit\n", "-h, --help");
                printf("      %-20s\tprint x-coordinate of each intersection "
                       "point for curves\n", "-r, --root");
                printf("      %-20s\tprint number of iterations used for "
                       "solving task of \n%-26c\tapproximate intersection point "
                       "search\n", "-i, --iterations", ' ');
                printf("      %-20s\ttest root function. Require 1 argument "
                       "F1:F2:A:B:E:R,\n%-26c\twhere F1 and F2 - numbers of "
                       "functions, A and B - left and\n%-26c\tright search "
                       "boundaries accordingly, E - margin of error, R - \n%-26c\t"
                       "correct result. Return answer, absolute and relative error\n",
                       "-R, --test-root", ' ', ' ', ' ');
                printf("      %-20s\ttest integral function. Require 1 "
                       "argument F:A:B:E:R,\n%-26c\twhere F - number of function, "
                       "A and B - left and right\n%-26c\tintegration boundaries "
                       "accordingly, E - margin of error,\n%-26c\tR - correct "
                       "result. Return answer, absolute and relative error\n\n",
                       "-I, --test-integral", ' ', ' ', ' ');
                printf("    Call without options will return the area of "
                       "curvilinear triangle\n");
                break;
            case 'r':
                show_roots = 1;
                break;
            case 'i':
                show_iterations = 1;
                break;
            case 'R':
                test_root(optarg);
                break;
            case 'I':
                test_integral(optarg);
                break;
            case '?':
                // prints error message by itself
                return 1;
            default:
                break;
        }
    }

    x12 = root(f1, f2, 0, 2, EPS1, &iter_count1);
    x23 = root(f2, f3, -1, 0, EPS1, &iter_count2);
    x13 = root(f1, f3, -1.9, -1, EPS1, &iter_count3);

    if (show_roots) {
        printf("%lf %lf %lf\n", x12, x23, x13);
    }

    if (show_iterations) {
        printf("%d %d %d\n", iter_count1, iter_count2, iter_count3);
    }

    if (argc == 1) {
        i1 = integral(f1, x13, x12, EPS2);
        i2 = integral(f2, x23, x12, EPS2);
        i3 = integral(f3, x13, x23, EPS2);

        printf("%lf\n", i1 - i2 - i3);
    }

    return 0;
}

/**
 * Parses argstr string and tests root function. Prints value, found by root,
 * absolute and relative error
 * @param[in] argstr string of specific form (F1:F2:A:B:E:R), where F1 and F2 -
 * numbers of functions, A and B - left and right search boundaries accordingly,
 * E - margin of error, R - correct result
 * @return 0 on success, 1 on failure
 */
int test_root(const char *argstr)
{
    int func_i1, func_i2, param_count;
    func *func1, *func2;
    double left_bound, right_bound, eps, ans, res;

    param_count = sscanf(argstr, "%d:%d:%lf:%lf:%lf:%lf", &func_i1,
                         &func_i2, &left_bound, &right_bound, &eps, &ans);

    if (param_count != TEST_ROOT_PARAM_COUNT) {
        fprintf(stderr, "./integral: test_root requires argument "
                        "of form F1:F2:A:B:E:R\n");
        fprintf(stderr, "./integral: run with -h or --help for "
                        "more information\n");
        return 1;
    }

    if (func_i1 < MIN_FUNC_INDEX + 1 || func_i1 > MAX_FUNC_INDEX + 1) {
        fprintf(stderr, "./integral: F1 should be in [%d,%d] "
                        "interval\n", MIN_FUNC_INDEX + 1, MAX_FUNC_INDEX + 1);
        fprintf(stderr, "./integral: run with -h or --help for "
                        "more information\n");
        return 1;
    }

    func1 = func_arr[func_i1 - 1];

    if (func_i2 < MIN_FUNC_INDEX + 1 || func_i2 > MAX_FUNC_INDEX + 1) {
        fprintf(stderr, "./integral: F2 should be in [%d,%d] "
                        "interval\n", MIN_FUNC_INDEX + 1, MAX_FUNC_INDEX + 1);
        fprintf(stderr, "./integral: run with -h or --help for "
                        "more information\n");
        return 1;
    }

    func2 = func_arr[func_i2 - 1];

    res = root(*func1, *func2, left_bound, right_bound, eps, NULL);
    printf("%lf %lf %lf\n", res, res - ans, (res - ans) / ans);
    return 0;
}

/**
 * Finds root (x-coordinate approximation of intersection point) of two functions
 * in specific interval withing given error
 * @param[in] f,g functions to be intersected
 * @param[in] a left bound of root interval
 * @param[in] b right bound of root interval
 * @param[in] eps1 error of root search
 * @param[out] iteration_counter_p pointer to a variable for storing iteration
 * count
 * @return root in interval withing given error
 */
double root(func *f, func *g, double a, double b, double eps1, int *iteration_counter_p)
{
    double left_point, right_point, middle_point;

    if (iteration_counter_p != NULL) {
        *iteration_counter_p = 0;
    }

    left_point = a;
    right_point = b;

    do {
        middle_point = (left_point + right_point) / 2;

        if ((f(left_point) - g(left_point)) *
            (f(middle_point) - g(middle_point)) <= 0) {
            right_point = middle_point;
        } else {
            left_point = middle_point;
        }

        if (iteration_counter_p != NULL) {
            (*iteration_counter_p)++;
        }
    } while (right_point - left_point > eps1);

    return (left_point + right_point) / 2;
}

/**
 * Parses argstr string and tests integral function. Prints value, found by integral,
 * absolute and relative error
 * @param argstr string of specific form (F:A:B:E:R), where F - number of function,
 * A and B - left and right integration boundaries accordingly, E - margin of error,
 * R - correct result
 */
int test_integral(const char *argstr)
{
    int func_i1, param_count;
    func *func1;
    double a, b, eps, ans, res;

    param_count = sscanf(argstr, "%d:%lf:%lf:%lf:%lf", &func_i1,
           &a, &b, &eps, &ans);

    if (param_count != TEST_INTEGRAL_PARAM_COUNT) {
        fprintf(stderr, "./integral: test_integral requires argument of form F:A:B:E:R\n");
        fprintf(stderr, "./integral: run with -h or --help for more information\n");
        return 1;
    }

    if (func_i1 < MIN_FUNC_INDEX + 1 || func_i1 > MAX_FUNC_INDEX + 1) {
        fprintf(stderr, "./integral: F1 should be in [%d,%d] "
                        "interval\n", MIN_FUNC_INDEX + 1, MAX_FUNC_INDEX + 1);
        fprintf(stderr, "./integral: run with -h or --help for "
                        "more information\n");
        return 1;
    }

    func1 = func_arr[func_i1 - 1];

    res = integral(*func1, a, b, eps);
    printf("%lf %lf %lf\n", res, res - ans, (res - ans) / ans);
    return 0;
}

/**
 * Finds numerical approximation for a definite integral of a function with
 * a given error
 * @param[in] f integrand (function to be integrated)
 * @param[in] a left bound of integration
 * @param[in] b right bound of integration
 * @param[in] eps2 integration error
 * @return numerical approximation for a definite integral
 */
double integral(func *f, double a, double b, double eps2)
{
    int n;
    double p, integral_n, integral_2n;

    n = 1;
    p = 1.0 / 3.0;
    integral_2n = compute_integral(f, a, b, n);

    do {
        n *= 2;

        integral_n = integral_2n;
        integral_2n = compute_integral(f, a, b, n);
    } while (fabs(integral_n - integral_2n) * p >= eps2);
    return integral_2n;
}

/**
 * Finds numerical approximation for a definite integral of a function with
 * a given number of rectangles
 * @param[in] f integrand (function to be integrated)
 * @param[in] a left bound of integration
 * @param[in] b right bound of integration
 * @param[in] n number of rectangles
 * @return numerical approximation for a definite integral
 */
double compute_integral(func *f, double a, double b, int n)
{
    double h, result;

    h = (b - a) / n;
    result = 0;

    for (int i = 0; i < n; i++) {
        result += f(a + (i + 0.5) * h);
    }
    return result * h;
}
