#ifndef BASE_SAMPLES_H
#define BASE_SAMPLES_H



typedef struct point
{
    double x, y, z;
} point_t;

typedef struct
{
    double dx, dy, dz;
} transponse_t;

typedef struct
{
    double kx, ky, kz;
} scale_t;

typedef struct
{
    double angle_x, angle_y, angle_z;
} rotate_t;





#endif