#ifndef ACTIONS_H
#define ACTIONS_H

#include "draw.h"

enum action_t { ROTATE, SCALE, TRANSPONSE, DRAW };

struct transformation_t {
    const transponse_t transponse_params;
    const scale_t scale_params;
    const rotate_t rotate_params;

    transformation_t(const transponse_t &t, const scale_t &s, const rotate_t &r)
        : transponse_params(t), scale_params(s), rotate_params(r) {}
};

typedef struct {
    view_t *view;
    action_t action;
    transformation_t transform_params;
} connection_t;

void handle_action(connection_t &connection, figure_t &figure);

#endif