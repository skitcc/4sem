#include "draw.h"

static void convert_coord(point_t &point, double width, double height) {
    point.x += width / 2;
    point.y += height / 2;
}

static void draw_line(const point_t &p1, const point_t &p2, QGraphicsScene *scene) {
    scene->addLine(p1.x, p1.y, p2.x, p2.y);
}

static errors clear_scene(const view_t *view) {
    if (!view->scene)
        return ERR_WRONG_SCENE;

    view->scene->clear();
    return SUCCESS;
}

static edge_points_t get_edge_points(const points_t &points, const edge_t &edge) {
    return {points.array_of_points[edge.start_edge], points.array_of_points[edge.end_edge]};
}

static void draw_transformed_line(const edge_points_t &edge_points, const view_t *view) {
    point_t p1_transformed = edge_points.p1;
    point_t p2_transformed = edge_points.p2;

    convert_coord(p1_transformed, view->width, view->height);
    convert_coord(p2_transformed, view->width, view->height);

    draw_line(p1_transformed, p2_transformed, view->scene);
}

static errors draw_lines(const points_t &points, const edges_t &edges, const view_t *view) {
    if (!view || !view->scene) {
        return ERR_WRONG_SCENE;
    }

    for (int i = 0; i < edges.size; i++) {
        edge_points_t edge_points = get_edge_points(points, edges.array_of_edges[i]);
        draw_transformed_line(edge_points, view);
    }
    return SUCCESS;
}

errors draw_figure(const figure_t &figure, view_t *view) {
    errors rc = clear_scene(view);

    if (!rc)
        draw_lines(figure.points, figure.edges, view);

    return rc;
}