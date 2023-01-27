#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define EPSILON 0.000000001
#define PI 3.14159265358979

#define OPTIONS "sctea"

double Sin(double x) {
  x = fmod(x, 2 * PI);
  if (x > PI) {
    x = x - 2 * PI;
  } else if (x < -PI) {
    x = x + 2 * PI;
  }
  double x_sq = x * x;
  double numerator = x
                     * ((x_sq * (52785432 - 479249 * x_sq) - 1640635920) * x_sq
                         + 11511339840);
  double denominator
      = ((18361 * x_sq + 3177720) * x_sq + 277920720) * x_sq + 11511339840;
  return numerator / denominator;
}
double Cos(double x) {
  x = fmod(x, 2 * PI);
  if (x > PI) {
    x = x - 2 * PI;
  } else if (x < -PI) {
    x = x + 2 * PI;
  }
  double x_sq = x * x;
  double numerator
      = (x_sq * (1075032 - 14615 * x_sq) - 18471600) * x_sq + 39251520;
  double denominator
      = ((127 * x_sq + 16632) * x_sq + 1154160) * x_sq + 39251520;
  return numerator / denominator;
}
double Tan(double x) {
  x = fmod(x, 2 * PI);
  double x_sq = x * x;
  double numerator
      = x
        * ((x_sq * (x_sq * (x_sq * (x_sq - 990) + 135135) - 4729725))
            + 34459425);
  double denominator
      = 45
        * ((x_sq * (x_sq * (x_sq * (x_sq - 308) + 21021) - 360360)) + 765765);
  return numerator / denominator;
}
double Exp(double x) {
  double term = 1.0, sum = 0.0; //f(0) = 1
  int n = 1;
  while (term > EPSILON) {
    sum += term;
    term *= (x / n);
    n = n + 1;
  }
  return sum;
}

int main(int argc, char **argv) {
  if (argc
      == 1) // argc[0] is the executable filename, at least argc[1] is not NULL.
  {
    puts("Error: no arguments supplied!");
    return -1;
  }
  int c = 0;
  double testData, maxData, step;
  double x;
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'a':
    case 's':
      printf("x      \t Sin       \t Library   \t Difference\n");
      printf("-      \t ---       \t -------   \t ----------\n");
      testData = -2 * PI;
      maxData = 2 * PI;
      step = PI / 16;
      while (testData < maxData) {
        x = testData;
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Sin(x), sin(x),
            Sin(x)
                - sin(
                    x)); //print as long floating point, at least 6 wide and 4 after decimal point
        testData += step;
      }
      if (c != 'a') {
        break;
      }
    case 'c':
      printf("x      \t Cos       \t Library   \t Difference\n");
      printf("-      \t ---       \t -------   \t ----------\n");
      testData = -2 * PI;
      maxData = 2 * PI;
      step = PI / 16;
      while (testData < maxData) {
        x = testData;
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Cos(x), cos(x),
            Cos(x)
                - cos(
                    x)); //print as long floating point, at least 6 wide and 4 after decimal point
        testData += step;
      }
      if (c != 'a') {
        break;
      }
    case 't':
      printf("x      \t Tan       \t Library   \t Difference\n");
      printf("-      \t ---       \t -------   \t ----------\n");
      testData = -PI / 3;
      maxData = PI / 3;
      step = PI / 16;
      while (testData < maxData) {
        x = testData;
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Tan(x), tan(x),
            Tan(x)
                - tan(
                    x)); //print as long floating point, at least 6 wide and 4 after decimal point
        testData += step;
      }
      if (c != 'a') {
        break;
      }
    case 'e':
      printf("x      \t Exp       \t Library   \t Difference\n");
      printf("-      \t ---       \t -------   \t ----------\n");
      testData = 0.0;
      maxData = 9.0;
      step = 0.1;
      while (testData < maxData) {
        x = testData;
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Exp(x), exp(x),
            Exp(x)
                - exp(
                    x)); //print as long floating point, at least 6 wide and 4 after decimal point
        testData += step;
      }
      if (c != 'a') {
        break;
      }
    }
  }
  return 0;
}
