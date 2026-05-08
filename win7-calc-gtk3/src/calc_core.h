#ifndef CALC_CORE_H
#define CALC_CORE_H

typedef enum { OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV } Op;

typedef struct {
    long double accumulator;
    long double current;
    Op pending_op;
    int entering; /* 1 if typing a number */
} CalcState;

void calc_init(CalcState *s);
void calc_enter_digit(CalcState *s, int digit);
void calc_enter_dot(CalcState *s);
void calc_press_op(CalcState *s, Op op);
void calc_press_equals(CalcState *s);
void calc_clear_entry(CalcState *s);
void calc_clear_all(CalcState *s);
const char* calc_get_display(CalcState *s);

#endif
