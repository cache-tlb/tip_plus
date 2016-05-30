#ifndef TIPWINDOW_H
#define TIPWINDOW_H

#include "GLWidget.h"
#include <QMainWindow>

class TipWindow : public QMainWindow
{
    Q_OBJECT

public:
    TipWindow(QWidget *parent = 0);
    ~TipWindow();

    void init();

    void SetStatusText(QString text);

private:
    GLWidget *gl_widget_;
    QLabel *status_label;

protected:


private:
    void LoadImage(const QString &path);

private slots:
    void OnLoadFile();
    void OnExit();

    void OnFlushAxis();
    void OnCompute3D();
    void OnSetFrontFace();
    void OnSetTopFace();
    void OnSetBottomFace();
    void OnSetLeftFace();
    void OnSetRightFace();
    void OnMoveAnchor();
    void OnDeleteAnchor();

    void OnSet3DView();
    void OnSet2DView();
    void OnSetDebugView();

};

#endif // TIPWINDOW_H
