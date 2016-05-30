#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "GLMesh.h"
#include "Shader.h"
//#include "Vec2.h"
#include "GLTexture.h"

class Renderer2D
{
public:
    Renderer2D(QOpenGLFunctionsType *context, VSMathLibQT *vsml);

    void SetImageTexture(const QImage &qimg);
    void DrawAnchors();
    void DrawImage();
    void Render();
    void Init();

public:
    GLMesh *film_plane_;
    Shader *draw_image_shader_;
    GLTexture *src_image_texture_;
    Shader *line_shader_;
    GLMesh *line_mesh_;

    QOpenGLFunctionsType *context;
    VSMathLibQT *vsml;

    std::vector<bool> anchor_flags_;
    std::vector<Vec2d> anchor_points_;
    std::vector<Vec2i> edges;
};

#endif // RENDERER2D_H
