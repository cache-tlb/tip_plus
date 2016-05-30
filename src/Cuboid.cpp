#include "Cuboid.h"
#include "util.h"
#include <fstream>
#include <cassert>

Cuboid::Cuboid() {
    faces.resize(5);
    face2d.resize(5);
}

Cuboid::~Cuboid() {}

int Cuboid::GetFaceNumber() const {
    return faces.size();
}

bool Cuboid::HasFace(int face_id) const {
    return faces[face_id].size() >= 3;
}

std::string Cuboid::FaceId2Name(int face_id) {
    switch (face_id) {
    case 0:
        return "front";
        break;
    case 1:
        return "left";
        break;
    case 2:
        return "bottom";
        break;
    case 3:
        return "right";
        break;
    case 4:
        return "top";
        break;
    default:
        printf("wrong face id\n");
        return "";
    }
}

int Cuboid::FaceName2Id(const std::string &face_name) {
    if (face_name == "front") {
        return 0;
    } else if (face_name == "left") {
        return 1;
    } else if (face_name == "bottom") {
        return 2;
    } else if (face_name == "right") {
        return 3;
    } else if (face_name == "top") {
        return 4;
    } else {
        printf("wrong face name\n");
        return -1;
    }
}

void Cuboid::SetPerspectiveMat(const Mat4d& mat) {
    perspect_mat = mat;
}

void Cuboid::SetImageDim(int w, int h) {
    width = w;
    height = h;
}

bool Cuboid::GetUnprojectedPoint(const Vec2d xy, int face_id, Vec3d &res, double &z) {
    // range of xy is [0,w-1]x[0,h-1]
    double P1 = perspect_mat.at(0,0);
    double P2 = perspect_mat.at(1,1);
    double P13 = perspect_mat.at(0,2);
    double P23 = perspect_mat.at(1,2);
    std::vector<Vec3d> &face = faces[face_id];
    Vec3d O = face[0], X = face[1], Y = face[2];
    double x0 = O.x, y0 = O.y, z0 = O.z;
    double x1 = X.x, y1 = X.y, z1 = X.z;
    double x2 = Y.x, y2 = Y.y, z2 = Y.z;
    double A = (y1 - y0)*(z2 - z0) - (y2 - y0)*(z1 - z0);
    double B = (z1 - z0)*(x2 - x0) - (z2 - z0)*(x1 - x0);
    double C = (x1 - x0)*(y2 - y0) - (x2 - x0)*(y1 - y0);
    Mat3d M;
    Vec3d x, b;
    M.at(0,0) = P1; M.at(0,1) = 0; M.at(0,2) = xy.x/(width-1)*2-1 + P13;
    M.at(1,0) = 0; M.at(1,1) = P2; M.at(1,2) = (height - 1 - xy.y)/(height-1)*2-1 + P23;
    M.at(2,0) = A; M.at(2,1) = B; M.at(2,2) = C;
    b.x = 0;  b.y = 0;   b.z = A*x0+B*y0+C*z0;
    x.x = 0; x.y = 0; x.z = 0;
    bool non_singlar = SolveLinear3(M, b, x);
    // std::cout << cv::determinant(M) << std::endl;
    res = x;
    double P33 = perspect_mat.at(2,2);
    double P34 = perspect_mat.at(2,3);
    double Z = res.z;
    z = (P33*Z + P34) / (-Z);
    return non_singlar;
}

bool Cuboid::GetUnprojectedPointAndFaceId(const Vec2d xy, int &face_id, Vec3d &res, double &z) {
    face_id = -1;
    z = std::numeric_limits<double>::infinity();
    for (int i = 0; i < faces.size(); i++) {
        if (face2d[i].empty()) continue;
        double temp_z;
        Vec3d temp_res;
        GetUnprojectedPoint(xy, i, temp_res, temp_z);
        // if (temp_z < 0) continue;
        if (temp_z < z) {
            z = temp_z;
            res = temp_res;
            face_id = i;
        }
    }
    if (face_id < 0 || face_id > face2d.size()) return false;
    return true;
}

void Cuboid::SetFace(int face_id, const Vec3d P1, const Vec3d P2, const Vec3d P3) {
    assert(face_id < faces.size());
    if (faces[face_id].size() < 3) faces[face_id].resize(3);
    faces[face_id][0] = P1;
    faces[face_id][1] = P2;
    faces[face_id][2] = P3;
}

void Cuboid::RemoveFace(int face_id) {
    assert(face_id < faces.size());
    faces[face_id].clear();
    face2d[face_id].clear();
}

void Cuboid::Add2DPointToFace(int face_id, const Vec2d P) {
    assert(face_id < face2d.size());
    face2d[face_id].push_back(P);
}

void Cuboid::SingleFaceTo2DMesh(int face_id, std::vector<std::vector<Vec2d> > &meshes, bool is_pixelwise, int triangle_or_quad) {
    // just do the triangulation
    assert(face_id < face2d.size());
    meshes.clear();
    std::vector<Vec2d> face_vector(face2d[face_id].size());
    for (int i = 0; i < face2d[face_id].size(); i++) {
        face_vector[i].x = face2d[face_id][i].x;
        face_vector[i].y = face2d[face_id][i].y;
    }
    if (face_vector.empty()) return;
    std::vector<Vec2d> face_hull;
    convex_hull(face_vector, face_hull);
    int xmax = 0, ymax = 0;
    for (int i = 0; i < face_hull.size(); i++) {
        xmax = std::max<int>(xmax, face_hull[i].x + 1);
        ymax = std::max<int>(ymax, face_hull[i].y + 1);
    }
    // cv::Mat mask = cv::Mat::zeros(ymax + 1, xmax + 1, CV_8UC3);
    // std::vector<std::vector<cv::Point> > contour_vector;
    // contour_vector.push_back(face_hull);

    if (!is_pixelwise) {
        if (face_hull.size() < 3) return;
        for (int i = 2; i < face_hull.size(); i++) {
            // i-2, i-1, i
            std::vector<Vec2d> triangle;
            triangle.push_back(Vec2d(face_hull[0].x, face_hull[0].y));
            triangle.push_back(Vec2d(face_hull[i-1].x, face_hull[i-1].y));
            triangle.push_back(Vec2d(face_hull[i].x, face_hull[i].y));
            // debug() << triangle[0].x << triangle[0].y<< '|' << triangle[1].x << triangle[1].y << '|' << triangle[2].x << triangle[2].y;
            meshes.push_back(triangle);
        }
    } else {
        /*cv::drawContours(mask, contour_vector, 0, cv::Scalar(255,255,255), CV_FILLED);
        for (int i = 0; i < mask.rows; i++) {
            for (int j = 0; j < mask.cols; j++) {
                if (mask.at<cv::Vec3b>(i,j)[0] == 0) continue;
                if (triangle_or_quad == 3) {
                    std::vector<cv::Point2d> triangle, triangle2;
                    triangle.push_back(cv::Point2d(j, i));
                    triangle.push_back(cv::Point2d(j + 1, i));
                    triangle.push_back(cv::Point2d(j, i + 1));

                    triangle2.push_back(cv::Point2d(j+1, i+1));
                    triangle2.push_back(cv::Point2d(j, i + 1));
                    triangle2.push_back(cv::Point2d(j + 1, i));
                    meshes.push_back(triangle);
                    meshes.push_back(triangle2);
                } else if (triangle_or_quad == 4) {
                    std::vector<cv::Point2d> triangle;
                    triangle.push_back(cv::Point2d(j, i));
                    triangle.push_back(cv::Point2d(j + 1, i));
                    triangle.push_back(cv::Point2d(j+1, i+1));
                    triangle.push_back(cv::Point2d(j, i + 1));
                    meshes.push_back(triangle);
                }
            }
        }*/
    }
}

void Cuboid::Gen3DMesh(bool is_pixelwise /* = true */) {
    triangles.clear();
    for (int i = 0; i < GetFaceNumber(); i++) {
        std::vector<std::vector<Vec2d> > mesh_2d;
        SingleFaceTo2DMesh(i, mesh_2d, is_pixelwise);
        for (int j = 0; j < mesh_2d.size(); j++) {
            std::vector<Vec3d> triangle;
            std::vector<Vec3d> normal;
            std::vector<Vec2d> texcoord;
            for (int k = 0; k < mesh_2d[j].size(); k++) {
                Vec2d p = mesh_2d[j][k];
                Vec3d pos;
                Vec2d uv;
                double z;
                GetUnprojectedPoint(p, i, pos, z);
                uv.x = p.x/(width - 1);
                uv.y = p.y/(height - 1);
                triangle.push_back(pos);
                texcoord.push_back(uv);
            }
            Vec3d n;
            plane_normal(triangle[0], triangle[1], triangle[2], n);
            for (int k = 0; k < mesh_2d[j].size(); k++) {
                normal.push_back(n);
            }
            triangles.push_back(triangle);
            texcoords.push_back(texcoord);
            normals.push_back(normal);
        }
    }
}

bool Cuboid::TryRayIntersection(Vec3d o, Vec3d d, int face_id, double &t) {
    t = 0;
    assert(face_id < faces.size());
    if (faces[face_id].empty()) {
        return false;
    }
    Vec3d normal = GetFaceNormal(face_id);
    double x0 = faces[face_id][0].x, y0 = faces[face_id][0].y, z0 = faces[face_id][0].z;
    double nx = normal.x, ny = normal.y, nz = normal.z;
    double denominator = nx*d.x + ny*d.y + nz*d.z;
    double numerator = - (nx*(o.x - x0) + ny*(o.y - y0) + nz*(o.z - z0));
    if (fabs(denominator) < 1e-10) {
        return false;
    }
    t = numerator / denominator;
    return true;
}

void Cuboid::RayIntersection(Vec3d o, Vec3d d, int &face_id, Vec3d &intersection, double &t_res) {
    face_id = -1;
    t_res = 0;
    double t_min = 1e100;
    int min_id = -1;
    for (int i = 0; i < faces.size(); i++) {
        double t = 0;
        if (!TryRayIntersection(o, d, i, t)) continue;
        if (t < 0) continue;
        if (t < t_min) {
            t_min = t;
            min_id = i;
        }
    }
    face_id = min_id;
    t_res = t_min;
    intersection.x = o.x + d.x*t_res;
    intersection.y = o.y + d.y*t_res;
    intersection.z = o.z + d.z*t_res;
}

Vec3d Cuboid::GetFaceNormal(int face_id) {
    assert(face_id < faces.size());
    Vec3d res;
    if (faces[face_id].size() < 3) {
        printf("face not set\n");
        return res;
    }
    plane_normal(faces[face_id][0], faces[face_id][1], faces[face_id][2], res);
    return res;
}

/*void Cuboid::ExportObj(std::string filepath, std::string size, const cv::Mat &im) {
    if (size == "big") Gen3DMesh(true);
    else if (size == "small") Gen3DMesh(false);
    std::ofstream fout(filepath);
    double min_triangle_len = 1e100;
    for (int i = 0; i < triangles.size(); i++) {
        min_triangle_len = std::min(min_triangle_len, cv::norm(triangles[i][0] - triangles[i][1]));
        min_triangle_len = std::min(min_triangle_len, cv::norm(triangles[i][1] - triangles[i][2]));
        min_triangle_len = std::min(min_triangle_len, cv::norm(triangles[i][2] - triangles[i][0]));
    }
    std::vector<cv::Point3d> true_points;
    for (int i = 0; i < triangles.size(); i++) {
        for (int j = 0; j < triangles[i].size(); j++) {
            int tp_index = -1;
            for (int k = 0; k < true_points.size(); k++) {
                if (norm(triangles[i][j] - true_points[k]) < min_triangle_len / 2.) {
                    tp_index = k;
                    break;
                }
            }
            if (tp_index >= 0) {
                triangles[i][j] = true_points[tp_index];
            } else {
                true_points.push_back(triangles[i][j]);
            }
        }
    }

    // vertex
    for (int i = 0; i < triangles.size(); i++) {
        for (int j = 0; j < triangles[i].size(); j++) {
            fout << 'v' << ' ' << triangles[i][j].x << ' ' << triangles[i][j].y << ' ' << triangles[i][j].z << std::endl;
        }
    }
    // normals
    fout << std::endl;
    for (int i = 0; i < triangles.size(); i++) {
        for (int j = 0; j < triangles[i].size(); j++) {
            fout << "vn" << ' ' << normals[i][j][0] << ' ' << normals[i][j][1] << ' ' << normals[i][j][2] << std::endl;
        }
    }
    // uvs
    fout << std::endl;
    if (size == "big") {
        for (int i = 0; i < triangles.size(); i++) {
            for (int j = 0; j < triangles[i].size(); j++) {
                fout << "vt" << ' ' << texcoords[i][j].x << ' ' << texcoords[i][j].y << std::endl;
            }
        }
    } else if (size == "small") {
        int total_triangle_number = triangles.size();
        int max_dim = std::max(im.cols, im.rows);
        max_dim = 10;
        int single_texture_w = max_dim*1.2, single_texture_h = max_dim*1.2;
        cv::Mat whole_texture(single_texture_h, single_texture_w * total_triangle_number, CV_8UC3);
        for (int i = 0; i < total_triangle_number; i++) {
            cv::Vec3d n(0,0,0);
            plane_normal(triangles[i][0], triangles[i][1], triangles[i][2], n);
            cv::Vec3d v1 = triangles[i][1] - triangles[i][0];
            v1 = v1 / sqrt(sqr(v1[0]) + sqr(v1[1]) + sqr(v1[2]));
            cv::Vec3d v2 = cross_product(v1, n);
            v2 = v2 / sqrt(sqr(v2[0]) + sqr(v2[1]) + sqr(v2[2]));
            // triangles[i][0] is the origin of local coordinate
            // so A is (0,0)
            cv::Vec3d ab = triangles[i][1] - triangles[i][0], ac = triangles[i][2] - triangles[i][0];
            double bx = ab.dot(v1), by = ab.dot(v2);
            double cx = ac.dot(v1), cy = ac.dot(v2);
            double max_x = std::max(0., std::max(bx, cx)), min_x = std::min(0., std::min(bx, cx));
            double max_y = std::max(0., std::max(by, cy)), min_y = std::min(0., std::min(by, cy));
            double range_x = max_x - min_x, range_y = max_y - min_y;
            double scale_x = single_texture_w / range_x, scale_y = single_texture_h / range_y;
            double scale = std::min(scale_x, scale_y);
            cv::Mat single_texture = cv::Mat::zeros(single_texture_h, single_texture_w, CV_8UC3);
            for (int r = 0; r < single_texture_h; r++) {
                for (int c = 0; c < single_texture_w; c++) {
                    double tx = c / scale + min_x, ty = r / scale + min_y;
                    double f1,f2,f3;
                    triangle_interpolate(cv::Point2d(0,0), cv::Point2d(bx, by), cv::Point2d(cx, cy), cv::Point2d(tx, ty), f1, f2, f3);
                    // compute the 3d coordinate
                    cv::Point3d t3d = f1 * triangles[i][0] + f2 * triangles[i][1] + f3 * triangles[i][2];
                    // project back to 2d:
                    cv::Point3d p3d;
                    project2plane(t3d, perspect_mat, p3d);
                    cv::Vec3b color;
                    double ii = (p3d.y + 1.)/2*(im.rows - 1), jj = (p3d.x + 1.)/2*(im.cols - 1);
                    interpolate_image(im, ii, jj, color);
                    single_texture.at<cv::Vec3b>(r,c) = color;
                }
            }
            single_texture.copyTo(whole_texture.colRange(single_texture_w*i, single_texture_w*(i+1)));
            texcoords[i][0].x = double(i) / total_triangle_number + (0 - min_x) * scale / single_texture_w / total_triangle_number;
            texcoords[i][0].y = (0 - min_y) * scale / single_texture_h;
            texcoords[i][1].x = double(i) / total_triangle_number + (bx - min_x) * scale / single_texture_w / total_triangle_number;
            texcoords[i][1].y = (by - min_y) * scale / single_texture_h;
            texcoords[i][2].x = double(i) / total_triangle_number + (cx - min_x) * scale / single_texture_w / total_triangle_number;
            texcoords[i][2].y = (cy - min_y) * scale / single_texture_h;
        }
        cv::flip(whole_texture, whole_texture, 0);
        cv::imwrite(filepath + ".png", whole_texture);
        for (int i = 0; i < triangles.size(); i++) {
            for (int j = 0; j < triangles[i].size(); j++) {
                fout << "vt" << ' ' << texcoords[i][j].x << ' ' << texcoords[i][j].y << std::endl;
            }
        }
    }
    fout << std::endl;
    // face index
    for (int i = 0; i < triangles.size(); i++) {
        fout << "f ";
        for (int j = 0; j < 3; j++)  {
            fout << i*3 + j + 1 << '/' << i*3 + j + 1 << '/' << i*3 + j + 1 << ' ';
        }
        fout << std::endl;
    }
    fout.close();
}*/

