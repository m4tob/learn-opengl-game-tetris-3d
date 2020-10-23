QT += opengl

TARGET = Tetris3D
TEMPLATE = app
SOURCES += main.cpp glwidget.cpp \
    board.cpp
HEADERS += glwidget.h \
    board.h
RESOURCES += res.qrc

win32 {
    INCLUDEPATH += \

    LIBS += \
        -lopengl32 \
        -lglu32
}

unix {
    LIBS += \
        -lGL \
        -lGLU
}
