#include "Renderer2D.h"

Renderer2D::Renderer2D(QOpenGLFunctionsType *context, VSMathLibQT *vsml)
    : context(context), vsml(vsml),
      src_image_texture_(NULL), film_plane_(NULL), draw_image_shader_(NULL)
{
    anchor_flags_.resize(8);
    anchor_points_.resize(8);
    edges.resize(8);
}

void Renderer2D::Init() {
    draw_image_shader_ = new Shader(context, vsml, "../tip_plus/Shaders/draw_image.vs.glsl", "../tip_plus/Shaders/draw_image.fs.glsl");
    film_plane_ = GLMesh::plane(context);

    line_shader_ = new Shader(context, vsml, "../tip_plus/Shaders/draw_line.vs.glsl", "../tip_plus/Shaders/draw_line.fs.glsl");
    line_mesh_ = new GLMesh(context);
}

void Renderer2D::SetImageTexture(const QImage &qimg) {
    if (src_image_texture_) delete src_image_texture_;
    Option op;
    src_image_texture_ = GLTexture::fromQImage(context, qimg, op);
}

void Renderer2D::DrawImage() {
    vsml->loadIdentity(VSMathLibQT::MODEL);
    vsml->loadIdentity(VSMathLibQT::VIEW);
    vsml->loadIdentity(VSMathLibQT::PROJECTION);
    if (draw_image_shader_ && film_plane_) {
        if (src_image_texture_) src_image_texture_->bind(0);
        draw_image_shader_->uniforms("t_diffuse", 0);
        draw_image_shader_->uniforms("flip_y", 1);
        draw_image_shader_->draw_mesh(film_plane_);
    }
}

void Renderer2D::DrawAnchors() {
    line_mesh_->clear();
    for (int i = 0; i < edges.size(); i++) {
        int id1 = edges[i].x, id2 = edges[i].y;
        line_mesh_->add_vertex(Vec3f(anchor_points_[id1].x,-anchor_points_[id1].y,0.), Vec2f(0,0), Vec3f(0,0,1));
        line_mesh_->add_vertex(Vec3f(anchor_points_[id2].x,-anchor_points_[id2].y,0.), Vec2f(0,0), Vec3f(0,0,1));
        if (!anchor_flags_[id1] || !anchor_flags_[id2]) continue;
        line_mesh_->add_face(Vec3i(i*2, i*2+1, i*2));
    }
    line_mesh_->compile();
    vsml->loadIdentity(VSMathLibQT::MODEL);
    vsml->loadIdentity(VSMathLibQT::VIEW);
    vsml->loadIdentity(VSMathLibQT::PROJECTION);
    float c[3] = {0.5f, 0.5f, 0.9f};
    line_shader_->uniforms("line_color", c);
    line_shader_->draw_wireframe(line_mesh_, 5);

    context->glEnable( GL_POINT_SMOOTH );

    line_mesh_->clear();
    for (int i = 0; i < 4; i++) {
        line_mesh_->add_vertex(Vec3f(anchor_points_[i].x,-anchor_points_[i].y,0.), Vec2f(0,0), Vec3f(0,0,1));
        if (!anchor_flags_[i]) continue;
        line_mesh_->add_face(Vec3i(i, i, i));
    }
    line_mesh_->compile();
    float c2[3] = {0.5f, 0.9f, 0.5f};
    line_shader_->uniforms("line_color", c2);
    line_shader_->draw_point(line_mesh_, 10);

    line_mesh_->clear();
    for (int i = 4; i < anchor_points_.size(); i++) {
        line_mesh_->add_vertex(Vec3f(anchor_points_[i].x,-anchor_points_[i].y,0.), Vec2f(0,0), Vec3f(0,0,1));
        if (!anchor_flags_[i]) continue;
        line_mesh_->add_face(Vec3i(i-4, i-4, i-4));
    }
    line_mesh_->compile();
     float c3[3] = {0.9f, 0.5f, 0.5f};
    line_shader_->uniforms("line_color", c3);
    line_shader_->draw_point(line_mesh_, 10);
}

void Renderer2D::Render() {
    DrawImage();
    DrawAnchors();
}

