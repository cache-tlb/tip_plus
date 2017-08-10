#include "Renderer3D.h"

Renderer3D::Renderer3D(QOpenGLFunctionsType *context, VSMathLibQT *vsml):
    context(context), vsml(vsml),
    scene_shader_(NULL), scene_mesh_(NULL), texture_(NULL),
    focal_length_(1000), near_clip_(1e-2), far_clip_(1e4),
    window_width_(1), window_height_(1),
    eye_(0,0,0), look_at_(0,0,-1), up_(0,1,0)
{

}

void Renderer3D::Init() {
    scene_shader_ = new Shader(context, vsml, "./Shaders/draw_model.vs.glsl", "./Shaders/draw_model.fs.glsl");
}

void Renderer3D::SetTexture(const QImage &qimg) {
    if (texture_) delete texture_;
    Option op;
    texture_ = GLTexture::fromQImage(context, qimg, op);
}

void Renderer3D::SetSceneMesh(const std::vector<std::vector<Vec3d> > &triangles, const std::vector<Vec3i> &faces, const std::vector<std::vector<Vec2d> > &texcoords, const std::vector<std::vector<Vec3d> > &normals) {
    if (scene_mesh_) delete scene_mesh_;
    scene_mesh_ = new GLMesh(context);
    for (int i = 0; i < triangles.size(); i++) {
        for (int j = 0; j < triangles[i].size(); j++) {
            scene_mesh_->add_vertex(triangles[i][j], texcoords[i][j], normals[i][j]);
        }
    }
    for (int i = 0; i < faces.size(); i++) {
        scene_mesh_->add_face(faces[i]);
    }
    scene_mesh_->has_coords = true;
    scene_mesh_->has_normals = true;
    scene_mesh_->compile();
}

void Renderer3D::SetCamera(const Vec3f &eye, const Vec3f &look_at, const Vec3f &up) {
    eye_ = eye;
    look_at_ = look_at;
    up_ = up;
}

void Renderer3D::Render() {
    float fov = 2*atan(window_height_ / focal_length_ / 2.)*180/PI;
    float aspect = (1.0f * window_width_) / window_height_;;

    vsml->loadIdentity(VSMathLibQT::PROJECTION);
    vsml->perspective(fov, aspect, near_clip_, far_clip_);

    vsml->loadIdentity(VSMathLibQT::VIEW);
    vsml->lookAt(eye_.x, eye_.y, eye_.z, look_at_.x, look_at_.y, look_at_.z, up_.x, up_.y, up_.z);

    if (scene_mesh_) {
        // context->glEnable(GL_CULL_FACE);
        // context->glCullFace(GL_FRONT);
        texture_->bind(0);
        scene_shader_->uniforms("t_diffuse", 0);
        scene_shader_->draw_mesh(scene_mesh_);
    }
}
