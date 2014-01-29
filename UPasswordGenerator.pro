QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WikiPassGen
TEMPLATE = app

DESTDIR = ..\bin

SOURCES += main.cpp\
        UPasswordGeneratorForm.cpp \
    UAboutProgram.cpp

HEADERS  += UPasswordGeneratorForm.h \
    UPasswordGenerator.h \
    UAboutProgram.h

FORMS    += UPasswordGeneratorForm.ui \
    UAboutProgram.ui

RESOURCES += \
    resource.qrc

win32 {
    RC_FILE = app.rc
}

OTHER_FILES += \
    app.rc
