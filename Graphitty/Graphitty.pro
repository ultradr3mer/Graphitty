QT       += core gui \
            charts \
            widgets \
            core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FunctionParser/functionnode.cpp \
    Math/lineqsolver.cpp \
    Math/polynomialfinder.cpp \
  Models/functionstablemodel.cpp \
    Models/recentviewmodel.cpp \
    Models/sheetviewmodel.cpp \
    Models/mainviewmodel.cpp \
  Models/thresholdstablemodel.cpp \
    main.cpp \
    mainview.cpp \
    polyeditchartview.cpp \
    polyeditview.cpp \
    sheetmanager.cpp \
    startview.cpp \
    viewarea.cpp

HEADERS += \
  Data/chartdata.h \
    Data/functiondata.h \
    Data/functionstablemodel.h \
  Data/thresholddata.h \
  Exceptions/DerivationException.h \
  Exceptions/derivationexception.h \
  Exceptions/functionparserexception.h \
  Exceptions/mathoperationexception.h \
  Exceptions/thresholdexception.h \
  Exceptions/variablenotdefinedexception.h \
    FunctionParser/functionnode.h \
    FunctionParser/mathoperation.h \
    FunctionParser/mathoperationexception.h \
    Math/lineqsolver.h \
    Math/polynomialfinder.h \
  Models/functionstablemodel.h \
    Models/recentviewmodel.h \
    Models/sheetviewmodel.h \
    Models/mainviewmodel.h \
  Models/thresholdstablemodel.h \
    mainview.h \
    polyeditchartview.h \
    polyeditview.h \
    sheetmanager.h \
    startview.h \
    viewarea.h

FORMS += \
    mainview.ui \
    polyeditview.ui \
    startview.ui

TRANSLATIONS += \
    Graphitty_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    testfile.json \
    testjson.json
