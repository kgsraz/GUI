QT += core gui sql widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

# Подключение PostgreSQL
LIBS += -lws2_32
QT += sql
