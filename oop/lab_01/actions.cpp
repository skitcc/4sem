#include "actions.h"



void handle_action(connection_t &connection, figure_t &figure)
{
    switch (connection.action)
    {
    case DRAW:
        draw_figure(figure, connection.view);
        break;

    case ROTATE:
        rotate_figure(figure, connection.transform_params.rotate_params);
        break;
    case SCALE:
        scale_figure(figure, connection.transform_params.scale_params); 
        break;
    case TRANSPONSE:
        transponse_figure(figure, connection.transform_params.transponse_params);
        break;
    default:
        break;
    }
}