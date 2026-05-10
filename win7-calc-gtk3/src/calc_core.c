#include "calc_core.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static char dispbuf[128];

void calc_init(CalcState *s){
    s->accumulator = 0.0L;
    s->current = 0.0L;
    s->pending_op = OP_NONE;
    s->entering = 0;
    strcpy(dispbuf, "0");
}

static void update_disp_from_current(CalcState *s){
    if (s->entering) {
        return;
    }
    snprintf(dispbuf, sizeof(dispbuf), "%.15Lg", s->current);
    char *p = dispbuf;
    if (strchr(p, '.')) {
        char *end = p + strlen(p) - 1;
        while (end > p && *end == '0') *end-- = '\0';
        if (end > p && *end == '.') *end = '\0';
    }
}

void calc_enter_digit(CalcState *s, int digit){
    if (!s->entering) {
        strcpy(dispbuf, "");
        s->entering = 1;
    }
    size_t len = strlen(dispbuf);
    if (len + 2 < sizeof(dispbuf)) {
        if (!(len==1 && dispbuf[0]=='0'))
            ; 
        if (len==1 && dispbuf[0]=='0') dispbuf[0] = '\0';
        char tmp[2] = { '0' + (digit%10), '\0' };
        strncat(dispbuf, tmp, sizeof(dispbuf)-strlen(dispbuf)-1);
    }
    s->current = strtold(dispbuf, NULL);
}

void calc_enter_dot(CalcState *s){
    if (!s->entering) {
        strcpy(dispbuf, "0.");
        s->entering = 1;
        s->current = 0.0L;
        return;
    }
    if (!strchr(dispbuf, '.')) {
        strncat(dispbuf, ".", sizeof(dispbuf)-strlen(dispbuf)-1);
    }
}

static void apply_pending(CalcState *s){
    if (s->pending_op == OP_NONE) {
        s->accumulator = s->current;
        return;
    }
    switch (s->pending_op){
        case OP_ADD: s->accumulator += s->current; break;
        case OP_SUB: s->accumulator -= s->current; break;
        case OP_MUL: s->accumulator *= s->current; break;
        case OP_DIV:
            if (s->current == 0.0L) {
                strcpy(dispbuf, "Error");
                s->accumulator = NAN;
                return;
            } else s->accumulator /= s->current;
            break;
        default: break;
    }
}

void calc_press_op(CalcState *s, Op op){
    if (s->entering) {
        s->entering = 0;
    }
    if (s->pending_op == OP_NONE){
        s->accumulator = s->current;
    } else {
        apply_pending(s);
    }
    s->pending_op = op;
    s->current = s->accumulator;
    update_disp_from_current(s);
}

void calc_press_equals(CalcState *s){
    if (s->entering) s->entering = 0;
    apply_pending(s);
    s->pending_op = OP_NONE;
    s->current = s->accumulator;
    update_disp_from_current(s);
}

void calc_clear_entry(CalcState *s){
    s->current = 0.0L;
    s->entering = 0;
    strcpy(dispbuf, "0");
}

void calc_clear_all(CalcState *s){
    calc_init(s);
}

const char* calc_get_display(CalcState *s){
    if (s->entering) return dispbuf;
    update_disp_from_current(s);
    return dispbuf;
}
void calc_negate(CalcState *s){
    s->current = -s->current;
    s->entering = 0;
}
void calc_sqrt(CalcState *s){
    if (s->current < 0.0L){
        s->current = 0.0L;
    } else {
        s->current = sqrtl(s->current);
    }
    s->entering = 0;
}

void calc_percent(CalcState *s){
    if (s->pending_op != OP_NONE){
        s->current = s->accumulator * (s->current / 100.0L);
    } else {
        s->current = s->current / 100.0L;
    }
    s->entering = 0;
}

void calc_reciprocal(CalcState *s){
    if (s->current == 0.0L){
        return;
    }
    s->current = 1.0L / s->current;
    s->entering = 0;
}
void calc_backspace(CalcState *s){
    if (!s->entering) return;  
    char *buf = dispbuf;
    size_t len = strlen(buf);
    if (len > 1){
        buf[len - 1] = '\0';
        s->current = strtold(buf, NULL);
    } else {
        s->current = 0.0L;
        s->entering = 0;
    }
}
