#ifndef UTIL_H
#define UTIL_H

#include "Vec3.h"
#include "Vec2.h"
#include "Mat.h"
#include "common.h"
#include <QImage>

bool plane_normal(const Vec3d P0, const Vec3d P1, const Vec3d P2, Vec3d &normal);

void convex_hull(const std::vector<Vec2d> &p, std::vector<Vec2d> &res);

bool two2three_plus(
    const std::vector<Vec2d> &points, int w, int h, // input
    Mat4d &perspective_mat, std::vector<Vec3d> &anchors, double &near, double &far, double &focal   // output
);

void triangle_interpolate(const Vec2d &A, const Vec2d &B, const Vec2d &C, const Vec2d &P, double &a, double &b, double &c);

void interpolate_image(const QImage &qimg, double i, double j, QRgb &color);

Vec2d ray_rect_intersection(const Vec2d& o, const Vec2d &d, int rx, int ry, int rw, int rh);

#endif // UTIL_H

