QT += opengl

TARGET = Tetris3D
TEMPLATE = app
SOURCES += main.cpp glwidget.cpp
HEADERS += glwidget.h
RESOURCES += res.qrc

win32 {
    INCLUDEPATH += \

    LIBS += \
        -lopengl32 \
        -lglu32
}

unix {
    INCLUDEPATH += /usr/include
    LIBS += /usr/lib/x86_64-linux-gnu/libglut.so \
        -lGL \
        -lGLU
}
