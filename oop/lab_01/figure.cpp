#include "figure.h"


figure_t init_figure()
{
    figure_t figure;
    set_default_point(figure.center);
    set_default_edges(figure.edges);
    set_default_points(figure.points);
    
    return figure;
}

static errors read_figure(FILE *file, figure_t &figure)
{
    if (!file)
        return ERR_RECEIVE_FILE;

    figure = init_figure();
    errors rc = read_all_points(file, figure.points);
    qDebug() << "rc : " << rc;


    if (rc == SUCCESS)
    {
        rc = read_all_edges(file, figure.edges);

        // for (int i = 0; i < figure.edges.size; i++) {
        //     qDebug() << "figure edge x y : " << figure.edges.array_of_edges[i].start_edge << figure.edges.array_of_edges[i].end_edge;
        // }
        if (rc)   
            free_points(figure.points);
    }


    return rc;
}


void free_figure(figure_t &figure)
{
    free_points(figure.points);
    free_edges(figure.edges);
}


errors set_figure(figure_t &figure, const char *filename)
{
    if (!filename)
        return ERR_RECEIVE_FILE;

    FILE *file = fopen(filename, "r");
    // qDebug() << "alooooo";
    errors rc = SUCCESS;
    if (!file)
    {
        // qDebug() << "alooooo";
        rc = ERR_OPEN_FILE;
    }
    else
    {
        figure_t temp_figure;
        rc = read_figure(file, temp_figure);
        fclose(file);
        if (rc == SUCCESS)
        {
            figure = temp_figure;
        }
        
    }
    return rc;
}



void transponse_figure(figure_t &figure, const transponse_t &transponse_params)
{
    transponse_point(figure.center, transponse_params);
    transponse_points(figure.points, transponse_params);
}

void scale_figure(figure_t &figure, const scale_t &scale_params)
{
    scale_points(figure.points, figure.center, scale_params);
}

void rotate_figure(figure_t &figure, const rotate_t &rotate_params)
{
    rotate_points(figure.points, figure.center, rotate_params);
}


