#include "TipWindow.h"
#include "Vec3.h"
TipWindow::TipWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

TipWindow::~TipWindow()
{

}

void TipWindow::init() {
    // center widget
    // todo: solve size
    gl_widget_ = new GLWidget(this);
    this->setCentralWidget(gl_widget_);
    gl_widget_->setFixedSize(512, 512);
//    gl_widget_->resize(512,512);
//    this->adjustSize();

    status_label = new QLabel;
    QStatusBar *status_bar = this->statusBar();
    status_bar->addWidget(status_label);
    status_label->setText(tr("Ready"));

    QMenuBar *menu_bar = new QMenuBar(this);
    this->setMenuBar(menu_bar);

    QMenu* file_menu = menu_bar->addMenu("File");
    QAction* load_file_act = file_menu->addAction("Load Image");
    load_file_act->setShortcut(QKeySequence("Ctrl+O"));
    QAction* exit_act = file_menu->addAction("Exit");
    exit_act->setShortcut(QKeySequence("Ctrl+Q"));
    connect(load_file_act, SIGNAL(triggered(bool)), this, SLOT(OnLoadFile()));
    connect(exit_act, SIGNAL(triggered(bool)), this, SLOT(OnExit()));

    QMenu *edit_menu = menu_bar->addMenu("Edit");
    QMenu *set_face = edit_menu->addMenu("Set Face");
    QAction *set_front_face_act = set_face->addAction("Set Front Face");
    set_front_face_act->setShortcut(QKeySequence("Ctrl+1"));
    QAction *set_left_face_act = set_face->addAction("Set Left Face");
    set_left_face_act->setShortcut(QKeySequence("Ctrl+2"));
    QAction *set_bottom_face_act = set_face->addAction("Set Bottom Face");
    set_bottom_face_act->setShortcut(QKeySequence("Ctrl+3"));
    QAction *set_right_face_act = set_face->addAction("Set Right Face");
    set_right_face_act->setShortcut(QKeySequence("Ctrl+4"));
    QAction *set_top_face_act = set_face->addAction("Set Top Face");
    set_top_face_act->setShortcut(QKeySequence("Ctrl+5"));
    QAction *compute_3d_act = edit_menu->addAction("Compute 3D");
    compute_3d_act->setShortcut(QKeySequence("Ctrl+R"));
    QAction *move_anchor_act = edit_menu->addAction("Move Anchor");
    move_anchor_act->setShortcut(QKeySequence("Ctrl+M"));
    QAction *delete_anchor_act = edit_menu->addAction("Delete Anchor");
    delete_anchor_act->setShortcut(QKeySequence("Ctrl+D"));
    QAction *flush_axis_act = edit_menu->addAction("Flush Axis");
    flush_axis_act->setShortcut(QKeySequence("Ctrl+F"));
    connect(compute_3d_act, SIGNAL(triggered(bool)), this, SLOT(OnCompute3D()));
    connect(set_front_face_act, SIGNAL(triggered(bool)), this, SLOT(OnSetFrontFace()));
    connect(set_left_face_act, SIGNAL(triggered(bool)), this, SLOT(OnSetLeftFace()));
    connect(set_right_face_act, SIGNAL(triggered(bool)), this, SLOT(OnSetRightFace()));
    connect(set_top_face_act, SIGNAL(triggered(bool)), this, SLOT(OnSetTopFace()));
    connect(set_bottom_face_act, SIGNAL(triggered(bool)), this, SLOT(OnSetBottomFace()));
    connect(move_anchor_act, SIGNAL(triggered(bool)), this, SLOT(OnMoveAnchor()));
    connect(delete_anchor_act, SIGNAL(triggered(bool)), this, SLOT(OnDeleteAnchor()));
    connect(flush_axis_act, SIGNAL(triggered(bool)), this, SLOT(OnFlushAxis()));

    QMenu *view_menu = menu_bar->addMenu("View");
    QAction *set_2d_view = view_menu->addAction("Set 2D View");
    set_2d_view->setShortcut(QKeySequence("Alt+2"));
    QAction *set_3d_view = view_menu->addAction("Set 3D View");
    set_3d_view->setShortcut(QKeySequence("Alt+3"));
    QAction *set_debug_view = view_menu->addAction("Set Debug View");
    connect(set_2d_view, SIGNAL(triggered(bool)), this, SLOT(OnSet2DView()));
    connect(set_3d_view, SIGNAL(triggered(bool)), this, SLOT(OnSet3DView()));
    connect(set_debug_view, SIGNAL(triggered(bool)), this, SLOT(OnSetDebugView()));
}


// below is ui response

void TipWindow::OnLoadFile() {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    if (!file_name.isEmpty()) {
        // QMessageBox::information(NULL, tr("Path"), tr("You selected ") + file_name);
        LoadImage(file_name);
    }
}

void TipWindow::OnExit() {
    this->close();
}

void TipWindow::OnCompute3D() {
    gl_widget_->Compute3D();
}

void TipWindow::OnSet3DView() {
    gl_widget_->SetViewMode(GLWidget::DrawMode::DRAW3D);
}

void TipWindow::OnSet2DView() {
    gl_widget_->SetViewMode(GLWidget::DRAWIMAGE);
}

void TipWindow::OnSetDebugView() {
    gl_widget_->SetViewMode(GLWidget::DRAWDEBUG);
}

// below is action functions.
void TipWindow::LoadImage(const QString &path) {
    QImage qimg;
    qimg.load(path);
    gl_widget_->setFixedSize(qimg.width(), qimg.height());
    this->adjustSize();

    gl_widget_->SetImage(qimg);
}

void TipWindow::SetStatusText(QString text) {
    status_label->setText(text);
}


void TipWindow::OnSetFrontFace() {
    gl_widget_->SetSelectType(GLWidget::FRONTFACE);
}

void TipWindow::OnSetTopFace() {
    gl_widget_->SetSelectType(GLWidget::TOPFACE);
}

void TipWindow::OnSetBottomFace() {
    gl_widget_->SetSelectType(GLWidget::BOTTOMFACE);
}

void TipWindow::OnSetLeftFace() {
    gl_widget_->SetSelectType(GLWidget::LEFTFACE);
}

void TipWindow::OnSetRightFace() {
    gl_widget_->SetSelectType(GLWidget::RIGHTFACE);
}

void TipWindow::OnMoveAnchor() {
    gl_widget_->SetSelectType(GLWidget::MOVEANCHOR);
}

void TipWindow::OnDeleteAnchor() {
    gl_widget_->SetSelectType(GLWidget::DELETEANCHOR);
}

void TipWindow::OnFlushAxis() {
    gl_widget_->FlushAxis();
}
