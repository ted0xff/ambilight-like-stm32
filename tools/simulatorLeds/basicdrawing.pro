#
# Simulateur pour test animations ambilight
# outil de test
# rendu screen
# ATTENTION : PAS MAL BUGUE..................


QT += widgets

# udp socket de commandes
QT += network

requires(qtConfig(combobox))


# si défini : fullscreen   + borderless
# sinon     : border/title + resizeable
#DEFINES += M_RELEASE

HEADERS       = renderarea.h \
                udplistener.h \
                window.h

SOURCES       = main.cpp \
                renderarea.cpp \
                udplistener.cpp \
                window.cpp
