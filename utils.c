#include "utils.h"

// Check num in [bound1, bound2] or [bound2, bound1]
unsigned char isInRange(int num, int bound1, int bound2){
    if (bound2 > bound1){
        return ( (num >= bound1) && (num <= bound2) );
    }
    else {
        return ( (num >= bound2) && (num <= bound1) );
    }
}