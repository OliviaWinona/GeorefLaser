TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += "WIN32"
DEFINES += "_USE_MATH_DEFINES"

SOURCES += main.cpp \
    chantier.cpp \
    panoramique.cpp \
    appariement.cpp \
    point.cpp \
    XConsoleModelProcess.cpp \
    libXBase/ConsoleError.cpp \
    libXBase/XAffinity2D.cpp \
    libXBase/XArchiGeoref.cpp \
    libXBase/XArchiTime.cpp \
    libXBase/XBBox.cpp \
    libXBase/XErrorTxt.cpp \
    libXBase/XErrorXml.cpp \
    libXBase/XFrame.cpp \
    libXBase/XInterpolLin.cpp \
    libXBase/XMat3D.cpp \
    libXBase/XPolygone2D.cpp \
    libXBase/XPt2D.cpp \
    libXBase/XPt3D.cpp \
    libXBase/XQuaternion.cpp \
    libXBase/XRect.cpp \
    libXBase/XStringTools.cpp \
    libXFileSystem/XPath.cpp \
    libXFileSystem/XSystemInfo.cpp

HEADERS += \
    chantier.h \
    panoramique.h \
    appariement.h \
    point.h \
    XConsoleModelProcess.h \
    libXBase/ConsoleError.h \
    libXBase/XAffinity2D.h \
    libXBase/XArchiGeoref.h \
    libXBase/XArchiTime.h \
    libXBase/XBase.h \
    libXBase/XBBox.h \
    libXBase/XColorTools.h \
    libXBase/XError.h \
    libXBase/XErrorConsole.h \
    libXBase/XErrorTxt.h \
    libXBase/XErrorXml.h \
    libXBase/XFInterpol.h \
    libXBase/XFrame.h \
    libXBase/XInterpolLin.h \
    libXBase/XMat3D.h \
    libXBase/XMetaData.h \
    libXBase/XPolygone2D.h \
    libXBase/XPt2D.h \
    libXBase/XPt3D.h \
    libXBase/XQuaternion.h \
    libXBase/XRect.h \
    libXBase/XStringTools.h \
    libXBase/XWait.h \
    libXFileSystem/XPath.h \
    libXFileSystem/XSystemInfo.h

