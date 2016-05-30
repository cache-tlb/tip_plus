#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "Mat.h"
#include "GLMesh.h"
#include "Shader.h"
#include "GLTexture.h"
#include "Cuboid.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "CameraController.h"
#include "GLCubeMap.h"
#include <QtOpenGL>

class GLWidget : public QGLWidget, protected QOpenGLFunctionsType
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent);

    void InitScene();

    void SetImage(const QImage &qimg);

    void Compute3D();

    void UpdateStatusMsg();

    enum DrawMode{
        DRAWIMAGE,
        DRAW3D,
        DRAWDEBUG
    };
    enum SetType{
        NONE,
        FRONTFACE,
        LEFTFACE,
        BOTTOMFACE,
        RIGHTFACE,
        TOPFACE,
        FACEEND,
        MOVEANCHOR,
        DELETEANCHOR
    };

    void SetViewMode(DrawMode mode);
    void UpdateCamera();
    void SetSelectType(SetType id);
    void Set2DAnchors();
    void Init2DAnchors();
    void FlushAxis();

public slots:
    void animate();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    int NearestAnchorId(int x, int y);

protected:
    QWidget *parent_;

    DrawMode draw_mode_;
    VSMathLibQT *vsml_;

    Renderer2D *renderer_2d_;
    Renderer3D *renderer_3d_;

    QImage image_;
    CameraController camera_controller_;
    Vec2i mouse_xy_;

    Cuboid cuboid;
    Vec2d axis_2d[4];
    bool is_axis_flushed;
    std::vector<Vec2d> face_points[5];
    int edge[8][2];
    Vec2d anchors_2d[8];
    bool anchor_flags[8];
    int set_type;
    int active_anchor_id_;
    Mat4d perspective_mat;  // perspective mat for the cuboid

    // temp
    /*Shader *shader_;
    GLMesh *mesh_;
    GLTexture *frame_buf_;
    Shader *cubemap_shader_;
    GLCubeMap *cubemap_;
    GLMesh *sphere_mesh_;*/

};

#endif // GLWIDGET_H
