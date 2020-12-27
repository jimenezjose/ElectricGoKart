/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       Filter.h
Description:     Data smoothing mechanism for data given by analogRead.
*******************************************************************************/
#ifndef FILTER_H
#define FILTER_H /* multiple inclusion guard */

/**
 * Filter:
 * y(k) = w * y(k-1) +  (1-w) * x(k)
 */
class Filter {
private:
  long y;
  double w;
public:
  Filter(double weight, long y_init=0) : y(y_init) {
    w = (weight < 0 || weight > 1) ? 0.5 : weight;
  }

  long read(long x) {
    y = w * y + (1-w) * x;
    return y;
  }
  
  long current() { 
    return y; 
  }
};

#endif
