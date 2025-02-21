#ifndef ACTIONS_H
#define ACTIONS_H

#include "draw.h"

enum action_t { ROTATE, SCALE, TRANSPONSE, DRAW };

struct transformation_t {
    const transponse_t transponse_params;
    const scale_t scale_params;
    const rotate_t rotate_params;
};

typedef struct {
    view_t *view;
    action_t action;
    union 
    {
        const transponse_t transponse_params;
        const scale_t scale_params;
        const rotate_t rotate_params;
    };
} connection_t;

void handle_action(connection_t &connection, figure_t &figure);

#endif