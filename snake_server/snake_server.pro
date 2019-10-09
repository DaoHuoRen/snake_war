TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        food.cpp \
        main.cpp \
        map.cpp \
        server.cpp \
        snake.cpp

HEADERS += \
    Timer.hpp \
    config.h \
    food.h \
    map.h \
    server.h \
    snake.h

LIBS += -lpthread
