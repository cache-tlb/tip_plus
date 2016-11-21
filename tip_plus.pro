#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T16:00:29
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tip_plus
TEMPLATE = app

INCLUDEPATH += 3rdparty/include

SOURCES += src/main.cpp\
        src/TipWindow.cpp \
        src/GLWidget.cpp \
        src/Vec3.cpp \
        src/GLMesh.cpp \
        src/GLTexture.cpp \
        src/VSShaderLibQT.cpp \
        src/Shader.cpp \
        src/VSMathLibQT.cpp \
        src/Renderer2D.cpp \
        src/Renderer3D.cpp \
        src/Cuboid.cpp \
        src/Mat.cpp \
        src/Vec2.cpp \
        src/util.cpp \
        src/CameraController.cpp \
        src/Vec4.cpp \
        src/GLCubeMap.cpp \
        3rdparty/src/json/jsoncpp.cpp

HEADERS  += src/TipWindow.h \
        src/GLWidget.h \
        src/Vec3.h \
        src/GLMesh.h \
        src/common.h \
        src/GLTexture.h \
        src/VSShaderLibQT.h \
        src/Shader.h \
        src/VSMathLibQT.h \
        src/OpenGLVersion.h \
        src/Renderer2D.h \
        src/Renderer3D.h \
        src/Cuboid.h \
        src/Mat.h \
        src/Vec2.h \
        src/util.h \
        src/CameraController.h \
        src/Vec4.h \
        src/GLCubeMap.h

win32:LIBS += opengl32.lib

DISTFILES += \
    Shaders/draw_image.fs.glsl \
    Shaders/draw_image.vs.glsl \
    Shaders/draw_model.fs.glsl \
    Shaders/draw_model.vs.glsl \
    Shaders/cubemap.fs.glsl \
    Shaders/cubemap.vs.glsl \
    Shaders/draw_line.fs.glsl \
    Shaders/draw_line.vs.glsl \
    Shaders/envmap.fs.glsl \
    Shaders/envmap.vs.glsl
