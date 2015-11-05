#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T16:10:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HandVisualizer
TEMPLATE = app


SOURCES += main.cpp\
        handwindow.cpp \
    handrenderer.cpp

HEADERS  += handwindow.h \
    handrenderer.h

FORMS    += handwindow.ui

unix {
LIBS += -L/home/pmulumba/VTK5.10.1/bin -lQVTK -lvtkVolumeRendering -lvtkHybrid -lvtkWidgets   \
        -lvtkInfovis -lvtkGeovis -lvtkViews -lvtkCharts -lvtkFiltering -lvtkImaging \
        -lvtkGraphics -lvtkGenericFiltering -lvtkIO -lvtkRendering -lvtkCommon  \
        -lcfitsio -lLeap

INCLUDEPATH += /usr/local/include/vtk-5.10
}
