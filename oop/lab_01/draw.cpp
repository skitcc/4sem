#include "draw.h"


void convert_coord(point_t &point, double width, double height)
{
    point.x += width / 2;
    point.y += height / 2;
}


void draw_line(const point_t &p1, const point_t &p2, QGraphicsScene *scene)
{
    QPen pen(Qt::white);
    scene->addLine(p1.x, p1.y, p2.x, p2.y);

}


errors clear_scene(const view_t &view)
{
    if (!view.scene)
        return ERR_WRONG_SCENE;

    view.scene->clear();
    return SUCCESS;
}


void draw_lines(const points_t &points, const edges_t &edges, const view_t &view)
{
    if (view.scene == nullptr) {
        qDebug() << "Error: Scene is not initialized!";
        return;
    }
    // qDebug() << "edges size : " << edges.size;

    // qDebug() << "received size : " << view.width << view.height;
    for (int i = 0; i < edges.size; i++)
    {
        point_t p1 = points.array_of_points[edges.array_of_edges[i].start_edge];
        point_t p2 = points.array_of_points[edges.array_of_edges[i].end_edge];


        convert_coord(p1, view.width, view.height);
        convert_coord(p2, view.width, view.height);

        draw_line(p1, p2, view.scene);
    }
}


errors draw_figure(const figure_t &figure, view_t &view)
{
    errors rc = clear_scene(view);
    
    if (!rc)
        draw_lines(figure.points, figure.edges, view);

    return rc;
}