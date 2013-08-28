# my files
SOURCES = segmenter.cpp
#HEADERS += 
#SOURCES += extra_file.cpp

### if running inside the source tree
### adjust as necessary
#MARSYAS_INSTALL_DIR = ../../
#INCLUDEPATH += $$MARSYAS_INSTALL_DIR/src/marsyas/
#LIBPATH += $$MARSYAS_INSTALL_DIR/build/lib/

### if installed elsewhere
#CHANGE - src/marsyas/ in Marsyas package depending on this file location
MARSYAS_INSTALL_DIR = ../../../Programs/Marsyas/marsyas-0.4.8/
INCLUDEPATH += $$MARSYAS_INSTALL_DIR/src/marsyas/
LIBPATH += $$MARSYAS_INSTALL_DIR/build/lib/

### basic OS stuff: do not change!
#win32-msvc2005:LIBS += marsyas.lib
unix:LIBS += -lmarsyas -L$$MARSYAS_INSTALL_DIR/build/lib
#!macx:LIBS += -lasound
#macx:LIBS += -framework CoreAudio -framework CoreMide -framework CoreFoundation
