#include "points.h"


error_t allocate_points(points_t &points)
{
    if (points.size <= 0)
        return ERR_AMOUNT;
    
    error_t rc = SUCCESS;
    
    point_t *temp_array_of_points = (point_t *)malloc(points.size * sizeof(point_t));
    if (!temp_array_of_points)
        rc = ERR_ALLOCATION;
    else
        points.array_of_points = temp_array_of_points;

    return rc;
}

void set_default_points(points_t &points)
{
    points.array_of_points = NULL;
    points.size = 0;
}


void free_points(points_t &points)
{
    free(points.array_of_points);
    set_default_points(points);
}



error_t read_amount_of_points(FILE *file, points_t &points)
{
    if (!file)
        return ERR_RECEIVE_FILE;
    
    error_t rc = SUCCESS;
    if (fscanf(file, "%d", points.size) != 1)
        rc = ERR_AMOUNT;
    return rc;
}


error_t read_point(FILE *file, point_t &point) 
{
    if (!file)
        return ERR_RECEIVE_FILE;
    
    point_t temp_point;
    error_t rc = SUCCESS;
    if (fscanf(file, "%ld %ld %ld", &temp_point.x, &temp_point.y, &temp_point.z) != 3)
        rc = ERR_POINTS_DATA;
    else
        point = temp_point;
    return rc;
}


error_t read_all_points(FILE *file, points_t &points)
{
    if (!file)
        return ERR_RECEIVE_FILE;

    error_t rc = SUCCESS;
    if (read_amount_of_points(file, points))
        rc = ERR_AMOUNT;
    else
    {
        if (allocate_points(points))
            rc = ERR_ALLOCATION;
        else
        {
            for (size_t i = 0; i < points.size; i++)
            {
                rc = read_point(file, points.array_of_points[i]); 
                if (rc)
                    free_points(points);
            }
        }
    }
    return rc;
}


error_t rotate_points(points_t &points, const point_t &center, const rotate_t &rotate_params)
{
    if (!points.array_of_points)
        return ERR_LOAD_DATA;

    for (size_t i = 0; i < points.size; i++)
        rotate_point(points.array_of_points[i], center, rotate_params);

    return SUCCESS;
}


error_t transponse_points(points_t &points, const transponse_t &transponse_params)
{
    if (!points.array_of_points)
        return ERR_LOAD_DATA;

    for (size_t i = 0; i < points.size; i++)
        transponse_point(points.array_of_points[i], transponse_params);
    
    return SUCCESS;
}

error_t scale_points(points_t &points, const point_t &center, const scale_t &scale_params)
{
    if (!points.array_of_points)
        return ERR_LOAD_DATA;
    
    for (size_t i = 0; i < points.size; i++)
        scale_point(points.array_of_points[i], center, scale_params);
        
    return SUCCESS;
}

