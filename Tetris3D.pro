QT += opengl

TARGET = Tetris3D
TEMPLATE = app
SOURCES += main.cpp glwidget.cpp \
    tetrixboard.cpp \
    tetrixpiece.cpp
HEADERS += glwidget.h \
    tetrixboard.h \
    tetrixpiece.h
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
