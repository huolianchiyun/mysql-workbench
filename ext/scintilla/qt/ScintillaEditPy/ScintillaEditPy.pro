TEMPLATE = lib
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScintillaEditPy

# Clear debug & release so that sepbuild.pri can set one or the other
CONFIG -= debug release

include(sepbuild.pri)

VERSION = $$SCINTILLA_VERSION

win32 {
	DebugBuild {
		TARGET_EXT = _d.pyd
	}
	else {
		TARGET_EXT = .pyd
	}
}

INCLUDEPATH += ../ScintillaEdit
INCLUDEPATH += ../ScintillaEditBase
INCLUDEPATH += ../../include ../../lexlib ../../src

INCLUDEPATH += $$PY_INCLUDES

INCLUDEPATH += $$SHIBOKEN_INCLUDES
INCLUDEPATH += $$PYSIDE_INCLUDES
INCLUDEPATH += $$PYSIDE_INCLUDES/QtCore
INCLUDEPATH += $$PYSIDE_INCLUDES/QtGui

unix:!mac {
	LIBS += -ldl
	LIBS += `pkg-config pyside --libs`
	QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-empty-body
}

macx {
	# Only build for x64 for now
	# QMAKE_CFLAGS = -arch i386 -arch x86_64
	# QMAKE_CXXFLAGS = -arch i386 -arch x86_64
	# QMAKE_LFLAGS = -arch i386 -arch x86_64
	LIBS += -L$$PY_LIBDIR -lpython$$PY_VERSION_SUFFIX
	LIBS += -L$$PYSIDE_LIB -L$$SHIBOKEN_LIB
	debug {
		LIBS += -lshiboken-python$$PY_VERSION_SUFFIX-dbg
		LIBS += -lpyside-python$$PY_VERSION_SUFFIX-dbg
	}
	else {
		LIBS += -lshiboken-python$$PY_VERSION_SUFFIX
		LIBS += -lpyside-python$$PY_VERSION_SUFFIX
	}
}

win32 {
	DebugBuild {
		DEFINES += DEBUG
		LIBS += -lQtCored4
	}
	else {
		LIBS += -lQtCore
	}
	LIBS += -L$$PY_PREFIX/libs # Note python lib is pulled in via a #pragma
	LIBS += -L$$PYSIDE_LIB -L$$SHIBOKEN_LIB
	# PySide uses x.y suffix on Windows even though Python uses xy
	DebugBuild {
		LIBS += -lshiboken-python$${PY_VERSION}_d
		LIBS += -lpyside-python$${PY_VERSION}_d
	}
	else {
		LIBS += -lshiboken-python$${PY_VERSION}
		LIBS += -lpyside-python$${PY_VERSION}
	}
}

# Wrapper sources; notifyheader commented out due to shiboken bug
SOURCES += \
	ScintillaEditPy/scintillaeditpy_module_wrapper.cpp \
	ScintillaEditPy/sci_notifyheader_wrapper.cpp \
	ScintillaEditPy/scnotification_wrapper.cpp \
	ScintillaEditPy/scintillaeditbase_wrapper.cpp \
	ScintillaEditPy/scintillaedit_wrapper.cpp \
	ScintillaEditPy/scintilladocument_wrapper.cpp

# ScintillaEdit sources

SOURCES += \
    ../ScintillaEdit/ScintillaEdit.cpp \
    ../ScintillaEdit/ScintillaDocument.cpp \
    ../ScintillaEditBase/PlatQt.cpp \
    ../ScintillaEditBase/ScintillaQt.cpp \
    ../ScintillaEditBase/ScintillaEditBase.cpp \
    ../../src/*.cxx \
    ../../lexlib/*.cxx \
    ../../lexers/*.cxx

# HEADERS is used to find what needs to be run through moc
HEADERS  += \
    ../ScintillaEdit/ScintillaEdit.h \
    ../ScintillaEdit/ScintillaDocument.h \
    ../ScintillaEditBase/ScintillaQt.h \
    ../ScintillaEditBase/ScintillaEditBase.h

DEFINES += SCINTILLA_QT=1 MAKING_LIBRARY=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1

DESTDIR = ../../bin

unix:!mac {
	# Rename to not have 'lib' at start
	QMAKE_POST_LINK += rm -rf ../../bin/ScintillaEditPy.so && ln -s libScintillaEditPy.so ../../bin/ScintillaEditPy.so
}

macx {
	# Rename to .so and not have 'lib' at start
	QMAKE_POST_LINK += rm -rf ../../bin/ScintillaEditPy.so && ln -s libScintillaEditPy.dylib ../../bin/ScintillaEditPy.so
}
