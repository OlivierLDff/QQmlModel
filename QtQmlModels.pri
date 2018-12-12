
# Qt QML Models

QT += core qml

INCLUDEPATH += $$PWD/src

HEADERS += \
    $$PWD/src/QQmlObjectListModel.h \
    $$PWD/src/QQmlVariantListModel.h \
    $$PWD/src/QQmlModelShared.h

SOURCES += \
    $$PWD/src/QQmlObjectListModel.cpp \
    $$PWD/src/QQmlModelShared.cpp \
    $$PWD/src/QQmlVariantListModel.cpp

