#include "single_point_operations.h"
#include <cmath>

static double to_radians(const double &angle) { return angle * (M_PI / 180); }

static angle_components_t calculate_angles(const double angle) {
    return (angle_components_t){cos(to_radians(angle)), sin(to_radians(angle))};
}

static void rotate_x_axis(point_t &point, const point_t &center, const double angle) {
    angle_components_t angles = calculate_angles(angle);
    
    double y = point.y - center.y;
    double z = point.z - center.z;

    point.y = y * angles.r_cos + z * angles.r_sin + center.y;
    point.z = -y * angles.r_sin + z * angles.r_cos + center.z;
}

static void rotate_y_axis(point_t &point, const point_t &center, const double angle) {
    angle_components_t angles = calculate_angles(angle);
    
    double x = point.x - center.x;
    double z = point.z - center.z;

    point.x = x * angles.r_cos + z * angles.r_sin + center.x;
    point.z = -x * angles.r_sin + z * angles.r_cos + center.z;
}

static void rotate_z_axis(point_t &point, const point_t &center, const double angle) {
    angle_components_t angles = calculate_angles(angle);
    
    double x = point.x - center.x;
    double y = point.y - center.y;

    point.x = x * angles.r_cos + y * angles.r_sin + center.x;
    point.y = -x * angles.r_sin + y * angles.r_cos + center.y;
}


void rotate_point(point_t &point, const point_t &center, const rotate_t &rotate_params) {
    rotate_x_axis(point, center, rotate_params.angle_x);
    rotate_y_axis(point, center, rotate_params.angle_y);
    rotate_z_axis(point, center, rotate_params.angle_z);
}

void transponse_point(point_t &point, const transponse_t &transponse_params) {
    point.x += transponse_params.dx;
    point.y += transponse_params.dy;
    point.z += transponse_params.dz;
}

void scale_point(point_t &point, const point_t &center, const scale_t &scale_params) {
    point.x = (point.x - center.x) * scale_params.kx + center.x;
    point.y = (point.y - center.y) * scale_params.ky + center.y;
    point.z = (point.z - center.z) * scale_params.kz + center.z;
}

void set_default_point(point_t &point) {
     point = {0, 0, 0};
}