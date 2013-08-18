# my files
SOURCES = segmenter.cpp
#HEADERS += 
#SOURCES += extra_file.cpp

### if running inside the source tree
### adjust as necessary
MARSYAS_INSTALL_DIR = ../../
INCLUDEPATH += $$MARSYAS_INSTALL_DIR/src/marsyas/
LIBPATH += $$MARSYAS_INSTALL_DIR/build/lib/

### if installed elsewhere
#MARSYAS_INSTALL_DIR = /usr/local
#MARSYAS_INSTALL_DIR = ${HOME}/usr/
#INCLUDEPATH += $$MARSYAS_INSTALL_DIR/marsyas/
#LIBPATH += $$MARSYAS_INSTALL_DIR/lib/

### basic OS stuff: do not change!
#win32-msvc2005:LIBS += marsyas.lib
unix:LIBS += -lmarsyas -L$$MARSYAS_INSTALL_DIR/build/lib
#!macx:LIBS += -lasound
#macx:LIBS += -framework CoreAudio -framework CoreMide -framework CoreFoundation
