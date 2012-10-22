CONFIG += debug_and_release

CONFIG(release, debug|release) {
	DBGNAME = release
}
else {
	DBGNAME = debug
}
DESTDIR = $${DBGNAME}

win32 {
    TEMPLATE = vcapp
    UNAME = Win32
    CONFIG(debug) {
        TRIMESH = trimeshd
    } else {
        TRIMESH = trimesh
    }
    
}
else {
    TEMPLATE = app
    TRIMESH = trimesh
    macx {
        DEFINES += DARWIN
        UNAME = Darwin
        CONFIG(debug, debug|release) {
            CONFIG -= app_bundle
        }
        LIBS += -framework CoreFoundation -framework OpenGL -framework QGLViewer
        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -fopenmp
    }
    else {
        DEFINES += LINUX
        UNAME = Linux
    }
}

QT += opengl xml
TARGET = qviewer

PRE_TARGETDEPS += ../libgq/$${DBGNAME}/libgq.a
DEPENDPATH += ../libgq/include
INCLUDEPATH += ../libgq/include
LIBS += -L../libgq/$${DBGNAME} -lgq

PRE_TARGETDEPS += ../demoutils/$${DBGNAME}/libdemoutils.a
DEPENDPATH += ../demoutils/include
INCLUDEPATH += ../demoutils/include
LIBS += -L../demoutils/$${DBGNAME} -ldemoutils

PRE_TARGETDEPS += ../trimesh2/$${DBGNAME}/libtrimesh.a
DEPENDPATH += ../trimesh2/include
INCLUDEPATH += ../trimesh2/include
LIBS += -L../trimesh2/$${DBGNAME} -l$${TRIMESH}

# Input
HEADERS += src/*.h
SOURCES += src/*.cc

OTHER_FILES += \
    shaders/programs.xml \
    shaders/*.vert \
    shaders/*.frag \
    shaders/*.glsl \
    shaders/warp.vert \
    shaders/warp.frag \
    shaders/deferredShading.vert \
    shaders/deferredShading.frag \
    shaders/wf.vert \
    shaders/wf.frag \
    shaders/finalColor.vert \
    shaders/finalColor.frag
