#include "points_operations.h"
#include <cstdlib>


errors allocate_points(points_t &points) {
    if (points.size <= 0)
        return ERR_AMOUNT;

    errors rc = SUCCESS;
    point_t *temp_array_of_points = (point_t *)malloc(points.size * sizeof(point_t));
    if (!temp_array_of_points)
        rc = ERR_ALLOCATION;

    if (rc == SUCCESS)
        points.array_of_points = temp_array_of_points;

    return rc;
}

void set_default_points(points_t &points) {
    points.array_of_points = NULL;
    points.size = 0;
}

void free_points(points_t &points) {
    free(points.array_of_points);
    set_default_points(points);
}

errors read_amount_of_points(FILE *file, size_t &size) {
    if (!file)
        return ERR_RECEIVE_FILE;

    errors rc = SUCCESS;
    if (fscanf(file, "%zu", &size) != 1)
        rc = ERR_AMOUNT;

    return rc;
}

errors read_point(FILE *file, point_t &point) {
    if (!file)
        return ERR_RECEIVE_FILE;

    point_t temp_point;
    errors rc = SUCCESS;
    if (fscanf(file, "%lf%lf%lf", &temp_point.x, &temp_point.y, &temp_point.z) != 3) {
        rc = ERR_POINTS_DATA;
    } else {
        point = temp_point;
    }
    return rc;
}

errors read_all_points(FILE *file, points_t &points) {
    if (!file)
        return ERR_RECEIVE_FILE;

    errors rc = SUCCESS;
    points_t temp_points;
    if (read_amount_of_points(file, temp_points.size) != SUCCESS)
        rc = ERR_AMOUNT;
    else {
        if (allocate_points(temp_points))
            rc = ERR_ALLOCATION;
        else {
            for (size_t i = 0; i < temp_points.size && rc == SUCCESS; i++) {
                rc = read_point(file, temp_points.array_of_points[i]);
            }
            if (rc != SUCCESS)
                free_points(temp_points);
        }
    }

    if (rc == SUCCESS)
        points = temp_points;
    return rc;
}

errors rotate_points(points_t &points, const point_t &center, const rotate_t &rotate_params) {
    if (!points.array_of_points)
        return ERR_LOAD_DATA;

    for (size_t i = 0; i < points.size; i++)
        rotate_point(points.array_of_points[i], center, rotate_params);

    return SUCCESS;
}

errors transponse_points(points_t &points, const transponse_t &transponse_params) {
    if (!points.array_of_points)
        return ERR_LOAD_DATA;

    for (size_t i = 0; i < points.size; i++)
        transponse_point(points.array_of_points[i], transponse_params);

    return SUCCESS;
}

errors scale_points(points_t &points, const point_t &center, const scale_t &scale_params) {
    if (!points.array_of_points)
        return ERR_LOAD_DATA;

    for (size_t i = 0; i < points.size; i++)
        scale_point(points.array_of_points[i], center, scale_params);

    return SUCCESS;
}
