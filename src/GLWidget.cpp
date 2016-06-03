#include "GLWidget.h"
#include "Vec2.h"
#include "Mat.h"
#include "util.h"
#include "Cuboid.h"
#include <QTimer>
#include "TipWindow.h"

GLWidget::GLWidget(QWidget *parent)
    :  QGLWidget(parent),
       parent_(parent),
       draw_mode_(DRAWIMAGE),
       renderer_2d_(NULL),
       renderer_3d_(NULL),
       set_type(NONE),
       active_anchor_id_(-1)
{
    setMouseTracking(true);
    int e[8][2] = {
        {0,1},
        {0,2},
        {0,3},
        {1,4},
        {2,4},
        {1,7},
        {2,6},
        {4,5}
    };
    for (int i = 0; i < 8; i++) {
        edge[i][0] = e[i][0];
        edge[i][1] = e[i][1];
        anchor_flags[i] = true;
    }
    for (int i = 0; i < 8; i++) {
        anchors_2d[i] = Vec2d(-1,-1);
    }
}

void GLWidget::InitScene() {
    vsml_ = VSMathLibQT::getInstance();
    vsml_->loadIdentity(VSMathLibQT::MODEL);
    vsml_->loadIdentity(VSMathLibQT::VIEW);
    vsml_->loadIdentity(VSMathLibQT::PROJECTION);

    camera_controller_.SetScreenParameter(this->width(), this->height());

    renderer_2d_ = new Renderer2D(this, vsml_);
    renderer_2d_->Init();
    renderer_3d_ = new Renderer3D(this, vsml_);
    renderer_3d_->Init();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), (GLWidget*)(this), SLOT(animate()));
    timer->setSingleShot(false);
    timer->start(25);

    /*shader_ = new Shader(this, vsml_, "./Shaders/cubemap.vs.glsl", "./Shaders/cubemap.fs.glsl");
    mesh_ = GLMesh::cube(this);
    Option op;
    frame_buf_ = new GLTexture(this, 512, 512, op);
    QImage xn = QImage(tr("../tip_plus/Images/cubemap/x-.png"));
    QImage xp = QImage(tr("../tip_plus/Images/cubemap/x+.png"));
    QImage yn = QImage(tr("../tip_plus/Images/cubemap/y-.png"));
    QImage yp = QImage(tr("../tip_plus/Images/cubemap/y+.png"));
    QImage zn = QImage(tr("../tip_plus/Images/cubemap/z-.png"));
    QImage zp = QImage(tr("../tip_plus/Images/cubemap/z+.png"));
    cubemap_ = GLCubeMap::fromQImages(this, xn, xp, yn, yp, zn, zp);
    cubemap_shader_ = new Shader(this, vsml_, "./Shaders/envmap.vs.glsl", "./Shaders/envmap.fs.glsl");
    sphere_mesh_ = GLMesh::sphere(this, 100);*/
}

void GLWidget::initializeGL() {
    // load opengl functions
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    InitScene();
}

void GLWidget::animate() {
    update();           // update() calls paintGL();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (draw_mode_ == DRAWIMAGE) {
        Set2DAnchors();
        renderer_2d_->Render();
    } else if (draw_mode_ == DRAW3D) {
        camera_controller_.Update(0);
        UpdateCamera();
        renderer_3d_->Render();
    } else if (draw_mode_ == DRAWDEBUG) {
        /*camera_controller_.Update(0);
        vsml_->loadIdentity(VSMathLibQT::MODEL);
        vsml_->loadIdentity(VSMathLibQT::VIEW);
        vsml_->lookAt(
                    camera_controller_.eye_pos_.x, camera_controller_.eye_pos_.y, camera_controller_.eye_pos_.z,
                    camera_controller_.look_at_.x, camera_controller_.look_at_.y, camera_controller_.look_at_.z,
                    camera_controller_.eye_up_.x, camera_controller_.eye_up_.y, camera_controller_.eye_up_.z);
        vsml_->loadIdentity(VSMathLibQT::PROJECTION);
        vsml_->perspective(90, 1, 1e-2, 1e4);
        float eye_pos[3];
        camera_controller_.eye_pos_.toArray(eye_pos);
        cubemap_shader_->uniforms("eye_pos", eye_pos);
        cubemap_shader_->uniforms("bg_distance", 100.f);
        cubemap_->bind(0);
        cubemap_shader_->uniforms("cube_texture", 0);
        cubemap_shader_->draw_mesh(sphere_mesh_);*/
//        vsml_->loadIdentity(VSMathLibQT::MODEL);
//        vsml_->loadIdentity(VSMathLibQT::VIEW);
//        vsml_->loadIdentity(VSMathLibQT::PROJECTION);
//        vsml_->lookAt(0,0,0,0,0,1,0,1,0);   // z+
//        vsml_->lookAt(0,0,0,0,0,-1,0,1,0);   // z-
//        vsml_->lookAt(0,0,0,1,0,0,0,1,0);   // x+
//        vsml_->lookAt(0,0,0,-1,0,0,0,1,0);   // x-
//        vsml_->lookAt(0,0,0,0,1,0,0,0,1);   // y+
//        vsml_->lookAt(0,0,0,0,-1,0,0,0,-1);   // y-
//        vsml_->perspective(90, 1, 1e-2, 1e4);
    //    float * f = vsml_->get(VSMathLibQT::PROJECTION);
    //    float * g = vsml_->get(VSMathLibQT::VIEW);
    //    for (int i = 0; i < 4; i++) {
    //        qDebug() << f[i+0*4] << f[i+1*4] << f[i+2*4] << f[i+3*4];
    //    }
    //    qDebug();
    //    for (int i = 0; i < 4; i++) {
    //        qDebug() << g[i+0*4] << g[i+1*4] << g[i+2*4] << g[i+3*4];
    //    }
//        shader_->draw_mesh(mesh_);
//        std::function<void(void)> call_back = [this]() {
//            shader_->draw_mesh(mesh_);
//        };
//        frame_buf_->drawTo(call_back, 3);
//        QImage im;
//        frame_buf_->toQImage(im);
//        im.save(tr("z+.png"));
    }
}

void GLWidget::resizeGL(int w, int h) {
    if(h == 0) h = 1;
    renderer_3d_->set_window_dim(w, h);
    camera_controller_.SetScreenParameter(this->width(), this->height());
    glViewport(0, 0, w, h);
    // set the projection matrix
    float asp = (1.0f * w) / h;
    vsml_->loadIdentity(VSMathLibQT::PROJECTION);
    double fov = 45;
    vsml_->perspective(fov, asp, 1e-2, 1e4);
}

void GLWidget::SetImage(const QImage &qimg) {
    image_ = qimg.copy();
    renderer_2d_->SetImageTexture(image_);
    Init2DAnchors();
    is_axis_flushed = false;
}

void GLWidget::SetViewMode(DrawMode mode) {
    draw_mode_ = mode;
}

void GLWidget::SetSelectType(SetType id) {
    set_type = id;
}

int GLWidget::NearestAnchorId(int x, int y) {
    int id = -1;
    double min_dist = std::numeric_limits<double>::infinity();
    for (int i = 0; i < 8; i++) {
        if (!anchor_flags[i]) continue;
        double dist = (Vec2d(x,y) - anchors_2d[i]).length();
        if (dist < min_dist) {
            min_dist = dist;
            id = i;
        }
    }
    return id;
}


void GLWidget::UpdateCamera() {
    renderer_3d_->SetCamera(camera_controller_.eye_pos_, camera_controller_.look_at_, camera_controller_.eye_up_);
}

void GLWidget::Set2DAnchors() {
    int w = std::max(1, image_.width());
    int h =std::max(1, image_.height());
    for (int i = 0; i < 8; i++) {
        renderer_2d_->anchor_flags_[i] = anchor_flags[i];
        renderer_2d_->anchor_points_[i] = Vec2d(anchors_2d[i].x / w * 2 - 1, anchors_2d[i].y / h * 2 - 1);
        renderer_2d_->edges[i] = Vec2i(edge[i][0], edge[i][1]);
    }
}

void GLWidget::Init2DAnchors() {
    double w = std::max(image_.width(), 1);
    double h = std::max(image_.height(), 1);
    double w1 = w/3, w2 = w*2/3;
    double h1 = h/3, h2 = h*2/3;
    anchors_2d[0] = Vec2d(w1, h2);
    anchors_2d[1] = Vec2d(w2, h2);
    anchors_2d[2] = Vec2d(w1, h1);
    anchors_2d[3] = Vec2d(0, h-1);
    anchors_2d[4] = Vec2d(w2, h1);
    anchors_2d[5] = Vec2d(w-1, 0);
    anchors_2d[6] = Vec2d(0, 0);
    anchors_2d[7] = Vec2d(w-1, h-1);
    for (int i = 0; i < 8; i++ ) {
        anchor_flags[i] = true;
    }
}

void GLWidget::FlushAxis() {
    for (int i = 0; i < 4; i++) {
        axis_2d[i] = anchors_2d[i];
    }
    is_axis_flushed = true;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    float x = event->x();
    float y =event->y();
    if (draw_mode_ == DRAW3D || draw_mode_ == DRAWDEBUG) {
        MouseEventArg arg;
        if (event->buttons() == Qt::LeftButton) {
            arg.mouse_button = MouseEventArg::LEFT;
        }
        else if(event->buttons() == Qt::RightButton) {
            arg.mouse_button = MouseEventArg::RIGHT;
        } else if (event->buttons() == Qt::MiddleButton) {
            arg.mouse_button = MouseEventArg::MIDDLE;
        }
        arg.mouse_type = MouseEventArg::PRESS;
        arg.position = Vec3f(x, y, 0.0);
        camera_controller_.HandleMousePressEvent(arg);
    } else if (draw_mode_ == DRAWIMAGE) {
        if (set_type > SetType::NONE && set_type < FACEEND) {
            int set_face_id = set_type - SetType::NONE - 1;
            face_points[set_face_id].push_back(Vec2d(x, y));
        } else if (set_type == SetType::MOVEANCHOR) {
            active_anchor_id_ = NearestAnchorId(x, y);
        } else if (set_type == SetType::DELETEANCHOR) {
            int anchor_id = NearestAnchorId(x, y);
            if (anchor_id >= 0) {
                Vec2d xy = anchors_2d[anchor_id];
                bool reply = QMessageBox::question(
                            this, "Delete Anchor",
                            tr("Delete Anchor %1: (%2, %3)").arg(anchor_id).arg(xy.x).arg(xy.y),
                            QMessageBox::Yes|QMessageBox::No);
                anchor_flags[anchor_id] = false;
            }
        }
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (set_type == SetType::MOVEANCHOR) {
        active_anchor_id_ = -1;
    }

    MouseEventArg arg;
    arg.mouse_type = MouseEventArg::RELEASE;
    camera_controller_.HandleMouseReleaseEvent(arg);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    // for parent
    mouse_xy_ = Vec2i(event->x(), event->y());
    UpdateStatusMsg();
    if (set_type == SetType::MOVEANCHOR) {
        if (active_anchor_id_ >= 0)
        anchors_2d[active_anchor_id_] = Vec2d(mouse_xy_.x, mouse_xy_.y);
    }

    // for camera controller
    float x = event->x();
    float y = event->y();
    MouseEventArg arg;
    arg.mouse_type = MouseEventArg::MOVE;
    arg.position = Vec3f(x, y, 0);
    camera_controller_.HandleMouseMoveEvent(arg);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    KeyEventArg arg;
    arg.key_code = event->key();
    arg.key_type = KeyEventArg::PRESS;
    camera_controller_.HandleKeyPressEvent(arg);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    KeyEventArg arg;
    arg.key_code = event->key();
    arg.key_type = KeyEventArg::RELEASE;
    camera_controller_.HandleKeyReleaseEvent(arg);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    MouseEventArg arg;
    arg.mouse_type = MouseEventArg::WHEEL;
    arg.degree = event->delta() / 100.f;
    camera_controller_.HandleWheelEvent(arg);
}

void GLWidget::UpdateStatusMsg() {
    if (set_type > SetType::NONE && set_type < FACEEND) {
        QString text = "Set %1: (%2, %3)";
        QString type_string[] = {
            tr(""),
            tr("Front face"),
            tr("Left face"),
            tr("Bottom face"),
            tr("Right face"),
            tr("Top Face"),
            tr("")
        };
        TipWindow *ptr = (TipWindow*)(parent_);
        ptr->SetStatusText(text.arg(type_string[set_type]).arg(mouse_xy_.x).arg(mouse_xy_.y));
    } else if (set_type == SetType::MOVEANCHOR && active_anchor_id_ >= 0) {
        QString text = "Moving anchor %1: (%2, %3)";
        TipWindow *ptr = (TipWindow*)(parent_);
        ptr->SetStatusText(text.arg(active_anchor_id_).arg(mouse_xy_.x).arg(mouse_xy_.y));
    }
}

void GLWidget::Compute3D() {
    // todo : first check the points
    int w = image_.width();
    int h = image_.height();
    if (!is_axis_flushed){
        QMessageBox::information(this, "Wrong", "Please flush the axis", QMessageBox::Ok);
        return;
    }

    std::vector<Vec2d> points2d_new;
    for (int i = 0; i < 4; i++) {
        points2d_new.push_back(axis_2d[i]);
    }
    std::vector<Vec3d> anchors;

    double near_, far_, focal_length;
    two2three_plus(points2d_new, w, h,  perspective_mat, anchors, near_, far_, focal_length);
    cuboid = Cuboid();
    cuboid.SetPerspectiveMat(perspective_mat);
    cuboid.SetImageDim(w, h);
    Vec3d A_3d = anchors[0], B_3d = anchors[1], C_3d = anchors[2], D_3d = anchors[3];
    Vec2d A_2d(anchors_2d[0]), B_2d(anchors_2d[1]), C_2d(anchors_2d[2]), D_2d(anchors_2d[3]);
    Vec2d E_2d(anchors_2d[4]), F_2d(anchors_2d[6]), G_2d(anchors_2d[7]), H_2d(anchors_2d[5]);

    // base faces
    cuboid.SetFace(0, A_3d, B_3d, C_3d);
    cuboid.SetFace(1, A_3d, C_3d, D_3d);
    cuboid.SetFace(2, A_3d, D_3d, B_3d);
    Vec3d E_3d, F_3d, G_3d;
    double z;
    cuboid.GetUnprojectedPoint(E_2d, 0, E_3d, z);
    cuboid.GetUnprojectedPoint(F_2d, 1, F_3d, z);
    cuboid.GetUnprojectedPoint(G_2d, 2, G_3d, z);
    cuboid.SetFace(3, B_3d, E_3d, G_3d);
    cuboid.SetFace(4, C_3d, F_3d, E_3d);

    if (anchor_flags[0]) {
        cuboid.Add2DPointToFace(0, A_2d);
        cuboid.Add2DPointToFace(1, A_2d);
        cuboid.Add2DPointToFace(2, A_2d);
    }
    if (anchor_flags[1]) {
        cuboid.Add2DPointToFace(0, B_2d);
        cuboid.Add2DPointToFace(2, B_2d);
        cuboid.Add2DPointToFace(3, B_2d);
    }
    if (anchor_flags[2]) {
        cuboid.Add2DPointToFace(0, C_2d);
        cuboid.Add2DPointToFace(1, C_2d);
        cuboid.Add2DPointToFace(4, C_2d);
    }
    if (anchor_flags[3]) {
        cuboid.Add2DPointToFace(1, D_2d);
        cuboid.Add2DPointToFace(2, D_2d);
    }
    if (anchor_flags[4]) {
        cuboid.Add2DPointToFace(0, E_2d);
        cuboid.Add2DPointToFace(3, E_2d);
        cuboid.Add2DPointToFace(4, E_2d);
    }
    if (anchor_flags[5]) {
        cuboid.Add2DPointToFace(1, F_2d);
        cuboid.Add2DPointToFace(4, F_2d);
    }
    if (anchor_flags[6]) {
        cuboid.Add2DPointToFace(2, G_2d);
        cuboid.Add2DPointToFace(3, G_2d);
    }
    if (anchor_flags[7]) {
        cuboid.Add2DPointToFace(3, H_2d);
        cuboid.Add2DPointToFace(4, H_2d);
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < face_points[i].size(); j++) {
            cuboid.Add2DPointToFace(i, face_points[i][j]);
        }
    }

    std::vector<std::vector<Vec3d> > triangles;
    std::vector<std::vector<Vec2d> > texcoords;
    std::vector<std::vector<Vec3d> > normals;
    std::vector<Vec3d> pos_pool;
    std::map<Vec2d, int> xy2poolid;

    for (int i = 0; i < cuboid.GetFaceNumber(); i++) {
        std::vector<std::vector<Vec2d> > face_triangles_2d;
        cuboid.SingleFaceTo2DMesh(i, face_triangles_2d, false, 3);
        for (int j = 0; j < face_triangles_2d.size(); j++) {
            std::vector<Vec2d> normalized_coords;
            std::vector<Vec3d> vertex_pos;
            Vec3d normal;
            for (int k = 0; k < face_triangles_2d[j].size(); k++) {
                double u = face_triangles_2d[j][k].x / double(w - 1), v = face_triangles_2d[j][k].y / double(h - 1);
                normalized_coords.push_back(Vec2d(u, v));
                Vec3d pos;
                double z;
                cuboid.GetUnprojectedPoint(face_triangles_2d[j][k], i, pos, z);
                bool find_pool = false;
                for (auto it = xy2poolid.begin(); it != xy2poolid.end(); it++) {
                    if (face_triangles_2d[j][k] == it->first) {
                        pos = pos_pool[it->second];
                        find_pool = true;
                        break;
                    }
                }
                vertex_pos.push_back(pos);
                if (!find_pool) {
                    xy2poolid[face_triangles_2d[j][k]] = pos_pool.size();
                    pos_pool.push_back(pos);
                }
                // debug() << u*2-1 << v*2-1 << pos.x << pos.y << pos.z;
            }
            plane_normal(vertex_pos[0], vertex_pos[1], vertex_pos[2], normal);
            std::vector<Vec3d> normal_vec;
            normal_vec.push_back(normal);
            normal_vec.push_back(normal);
            normal_vec.push_back(normal);
            texcoords.push_back(normalized_coords);
            triangles.push_back(vertex_pos);
            normals.push_back(normal_vec);
        }
    }

    // for texture
    int total_triangle_number = triangles.size();
    int single_texture_w = 512, single_texture_h = 512;
    QImage whole_texture(single_texture_w * total_triangle_number, single_texture_h, QImage::Format_RGB888);
    for (int i = 0; i < total_triangle_number; i++) {
        Vec3d n(0,0,0);
        plane_normal(triangles[i][0], triangles[i][1], triangles[i][2], n);
        Vec3d v1 = triangles[i][1] - triangles[i][0];
        v1 = v1 / v1.length();
        Vec3d v2 = v1.cross(n);
        v2 = v2 / v2.length();
        // triangles[i][0] is the origin of local coordinate
        // so A is (0,0)
        Vec3d ab = triangles[i][1] - triangles[i][0], ac = triangles[i][2] - triangles[i][0];
        double bx = ab.dot(v1), by = ab.dot(v2);
        double cx = ac.dot(v1), cy = ac.dot(v2);
        double max_x = std::max(0., std::max(bx, cx)), min_x = std::min(0., std::min(bx, cx));
        double max_y = std::max(0., std::max(by, cy)), min_y = std::min(0., std::min(by, cy));
        double range_x = max_x - min_x, range_y = max_y - min_y;
        double scale_x = single_texture_w / range_x, scale_y = single_texture_h / range_y;
        double scale = std::min(scale_x, scale_y);
        QImage single_texture = QImage(single_texture_w, single_texture_h, QImage::Format_RGB888);
        for (int r = 0; r < single_texture_h; r++) {
            for (int c = 0; c < single_texture_w; c++) {
                double tx = c / scale + min_x, ty = r / scale + min_y;
                double f1,f2,f3;
                triangle_interpolate(Vec2d(0,0), Vec2d(bx, by), Vec2d(cx, cy), Vec2d(tx, ty), f1, f2, f3);
                // compute the 3d coordinate
                Vec3d t3d = f1 * triangles[i][0] + f2 * triangles[i][1] + f3 * triangles[i][2];
                // project back to 2d:
                Vec3d p3d = HomogeneousTransform(perspective_mat, t3d);
                QRgb color;
                double ii = (p3d.y + 1.)/2*(h - 1), jj = (p3d.x + 1.)/2*(w - 1);
                interpolate_image(image_, h - 1 - ii, jj, color);
                single_texture.setPixel(c, r, color);
            }
        }
        for (int r = 0; r < single_texture_h; r++) {
            for (int c = 0; c < single_texture_w; c++) {
                whole_texture.setPixel(single_texture_w*i + c, r, single_texture.pixel(c, r));
            }
        }
        // single_texture.copyTo(whole_texture.colRange(single_texture_w*i, single_texture_w*(i+1)));
        texcoords[i][0].x = double(i) / total_triangle_number + (0 - min_x) * scale / single_texture_w / total_triangle_number;
        texcoords[i][0].y = (0 - min_y) * scale / single_texture_h;
        texcoords[i][1].x = double(i) / total_triangle_number + (bx - min_x) * scale / single_texture_w / total_triangle_number;
        texcoords[i][1].y = (by - min_y) * scale / single_texture_h;
        texcoords[i][2].x = double(i) / total_triangle_number + (cx - min_x) * scale / single_texture_w / total_triangle_number;
        texcoords[i][2].y = (cy - min_y) * scale / single_texture_h;
    }

    std::vector<Vec3i> faces;
    for (int i = 0; i < triangles.size(); i++) {
        faces.push_back(Vec3i(3*i, 3*i+1, 3*i+2));
    }

    renderer_3d_->set_focal(focal_length);
    renderer_3d_->set_window_dim(w, h);
    renderer_3d_->SetTexture(whole_texture);
    renderer_3d_->SetMesh(triangles, faces, texcoords, normals);

    QMessageBox::information(NULL, tr("OK"), tr("OK"));
}


