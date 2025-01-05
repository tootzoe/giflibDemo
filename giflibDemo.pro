QT = core widgets

CONFIG += c++17

SOURCES += \
    TMainWnd.cpp \
    giflib522/dgif_lib.c \
    giflib522/egif_lib.c \
    giflib522/gif_err.c \
    giflib522/gif_hash.c \
    giflib522/gifalloc.c \
    giflib522/openbsd-reallocarray.c \
    main.cpp

HEADERS += \
    TMainWnd.h \
    giflib522/gif_hash.h \
    giflib522/gif_lib.h \
    giflib522/gif_lib_private.h


