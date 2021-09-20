QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/OpenGL_1_1/glm/glwidget_1_1.cpp \
    src/OpenGL_1_1/glm/model_1_1.cpp \
    src/OpenGL_1_1/qt/glwidget_1_1.cpp \
    src/OpenGL_1_1/qt/model_1_1.cpp \
    src/OpenGL_2_0/glwidget_2_0.cpp \
    src/OpenGL_2_0/model_2_0.cpp

HEADERS += \
    mainwindow.h \
    src/OpenGL_1_1/glm/glwidget_1_1.h \
    src/OpenGL_1_1/glm/model_1_1.h \
    src/OpenGL_1_1/qt/glwidget_1_1.h \
    src/OpenGL_1_1/qt/model_1_1.h \
    src/OpenGL_2_0/ShaderProgram.h \
    src/OpenGL_2_0/glwidget_2_0.h \
    src/OpenGL_2_0/model_2_0.h \
    src/camera_glm.h \
    src/camera_qt.h \
    src/fps.h \
    src/stl.h \
    stl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/src

DISTFILES += \
    shader.vert

RESOURCES += \
    resource.qrc
