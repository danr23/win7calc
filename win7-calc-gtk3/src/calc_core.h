#ifndef CALC_CORE_H
#define CALC_CORE_H
#include <math.h>
typedef enum { OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV } Op;

typedef struct {
    long double accumulator;
    long double current;
    Op pending_op;
    int entering;
} CalcState;
/* List of functions that the calculator uses */
void calc_init(CalcState *s);
void calc_enter_digit(CalcState *s, int digit);
void calc_enter_dot(CalcState *s);
void calc_press_op(CalcState *s, Op op);
void calc_press_equals(CalcState *s);
void calc_clear_entry(CalcState *s);
void calc_clear_all(CalcState *s);
const char* calc_get_display(CalcState *s);
/* useless feature from windows 7 */
void calc_negate(CalcState *s);
/* usefull features from windows 7 */
void calc_sqrt(CalcState *s);
void calc_percent(CalcState *s);
void calc_reciprocal(CalcState *s);

#endif
