#include "actions.h"

void handle_action(connection_t &connection, figure_t &figure) {
    switch (connection.action) {
    case DRAW:
        draw_figure(figure, connection.view);
        break;
    case ROTATE:
        rotate_figure(figure, connection.rotate_params);
        break;
    case SCALE:
        scale_figure(figure, connection.scale_params);
        break;
    case TRANSPONSE:
        transponse_figure(figure, connection.transponse_params);
        break;
    default:
        break;
    }
}