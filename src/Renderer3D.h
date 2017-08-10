#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "GLMesh.h"
#include "Shader.h"
#include "GLTexture.h"

class Renderer3D
{
public:
    Renderer3D(QOpenGLFunctionsType *context, VSMathLibQT *vsml);

    void Render();
    void Init();

    void set_focal(float f) {
        focal_length_ = f;
    }

    void set_near_clip(float near_clip) {
        near_clip_ = near_clip;
    }

    void set_far_clip(float far_clip) {
        far_clip_ = far_clip;
    }

    void set_window_dim(int width, int height) {
        window_height_ = height;
        window_width_ = width;
    }

    void SetTexture(const QImage &qimg);
    void SetSceneMesh(const std::vector<std::vector<Vec3d> > &triangles, const std::vector<Vec3i> &faces, const std::vector<std::vector<Vec2d> > &texcoords, const std::vector<std::vector<Vec3d> > &normals);
    void SetCamera(const Vec3f &eye, const Vec3f &look_at, const Vec3f &up);

protected:

    QOpenGLFunctionsType *context;
    VSMathLibQT *vsml;

    Shader *scene_shader_;

    float focal_length_, near_clip_, far_clip_;
    int window_width_, window_height_;

    Vec3f eye_, look_at_, up_;

    GLMesh *scene_mesh_;
    GLTexture *texture_;
};

#endif // RENDERER3D_H
