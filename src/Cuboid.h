#ifndef CUBOID_H
#define CUBOID_H

#include "common.h"
#include "Mat.h"
#include "Vec3.h"
#include "Vec2.h"

class Cuboid {
public:
    Cuboid();

    ~Cuboid();

    int GetFaceNumber() const;

    bool HasFace(int face_id) const;

    void SetPerspectiveMat(const Mat4d &mat);

    void SetImageDim(int w, int h);

    bool GetUnprojectedPoint(const Vec2d xy, int face_id, Vec3d &res, double &z);

    bool GetUnprojectedPointAndFaceId(const Vec2d xy, int &face_id, Vec3d &res, double &z);

    void Add2DPointToFace(int face_id, const Vec2d P);

    void SetFace(int face_id, const Vec3d P1, const Vec3d P2, const Vec3d P3);

    void RemoveFace(int face_id);

    void SingleFaceTo2DMesh(int face_id, std::vector<std::vector<Vec2d> > &meshes, bool is_pixelwise = true, int triangle_or_quad = 3);

    void Gen3DMesh(bool is_pixelwise = true);

    void RayIntersection(Vec3d o, Vec3d d, int &face_id, Vec3d &intersection, double &t);

    bool TryRayIntersection(Vec3d o, Vec3d d, int face_id, double &t);

    static std::string FaceId2Name(int face_id);

    static int FaceName2Id(const std::string &face_name);

    Vec3d GetFaceNormal(int face_id);

    // void ExportObj(std::string filepath, std::string size, const cv::Mat &im = cv::Mat());

protected:
    std::vector<std::vector<Vec3d> > faces;       // the wall
    std::vector<std::vector<Vec2d> > face2d;      // the wall, x,y on the image

    // 3d mesh
    std::vector<std::vector<Vec3d> > triangles;   // the mesh
    std::vector<std::vector<Vec3d> > normals;   // the mesh normal
    std::vector<std::vector<Vec2d> > texcoords;   // the texcoords
    Mat4d perspect_mat;

    // image related
    int width, height;
};


#endif // CUBOID_H
