QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CutControlWdg.cpp \
    InfiPlot.cpp \
    InfiTracer.cpp \
    PlaybackSlider.cpp \
    main.cpp \
    qcustomplot/qcustomplot.cpp \
    widget.cpp


HEADERS += \
    CutControlWdg.h \
    InfiPlot.h \
    InfiTracer.h \
    PlaybackSlider.h \
    qcustomplot/qcustomplot.h \
    widget.h

FORMS += \
    CutControlWdg.ui \
    InfiPlot.ui \
    PlaybackSlider.ui \
    widget.ui

INCLUDEPATH += ./qcustomplot
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
