#-------------------------------------------------
#
# Project created by QtCreator 2017-05-03T11:21:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XUXUCAM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



LIBS += -LE:\\X_HSU\\xuxu_CAM\\XUXUCAM\\exLibs\\libdxflib.a
 #LIBS += -LE:\\X_HSU\\xuxu_CAM\\XUXUCAM\\exLibs\\debug\\dxflib.lib
 #LIBS += -LE:\\X_HSU\\xuxu_CAM\\XUXUCAM\\exLibs\\release\\dxflib.lib
INCLUDEPATH += E:\\X_HSU\\xuxu_CAM\\XUXUCAM\\dxfSrc
INCLUDEPATH += D:\\programsInstallatons\\boost_1_63_0


SOURCES += main.cpp\
        camexpert.cpp \
    cam_newmaterial.cpp \
    cam_importmat.cpp \
    FileIO/cam_fileio.cpp \
    StringOperation/stringoperation.cpp \
    dxfSrc/dl_dxf.cpp \
    dxfSrc/dl_writer_ascii.cpp \
    CamDxf/camdxf.cpp \
    CamDxf/qpointfwithparent.cpp \
    CamDxf/campart.cpp \
    CamDxf/camleads.cpp \
    CamDxf/camswap.cpp \
    CamDxf/camscene.cpp \
    cam_newprt.cpp \
    cam_importprt.cpp \
    dxfSrc/new/dl_dxf.cpp \
    dxfSrc/new/dl_writer_ascii.cpp \
    dxfSrc/old/dl_dxf.cpp \
    dxfSrc/old/dl_writer_ascii.cpp \
    CamUi/CamUiBasic/cam_workshop.cpp \
    CamUi/CamUiForms/cam_blockdlg.cpp \
    CamCores/CamEng/cam_document.cpp \
    CamCores/CamInfo/cam_position.cpp \
    CamCores/CamGeneration/cam_xmloutput.cpp \
    CamCores/CamSci/cam_sci.cpp \
    CamCores/CamEng/cam_block.cpp \
    CamCores/CamEng/cam_blocklist.cpp \
    CamCores/CamEng/cam_layer.cpp \
    CamCores/CamEng/cam_pen.cpp \
    CamCores/CamEng/cam_layerlist.cpp \
    CamCores/CamEng/cam_entity.cpp \
    CamCores/CamEng/cam_entity_container.cpp \
    CamCores/CamEng/cam_vector.cpp \
    CamCores/CamEng/cam_math.cpp \
    CamThirdParty/muparser/src/muParser.cpp \
    CamThirdParty/muparser/src/muParserBase.cpp \
    CamThirdParty/muparser/src/muParserBytecode.cpp \
    CamThirdParty/muparser/src/muParserCallback.cpp \
    CamThirdParty/muparser/src/muParserError.cpp \
    CamThirdParty/muparser/src/muParserTokenReader.cpp \
    CamCores/CamDebug/cam_debug.cpp

HEADERS  += camexpert.h \
    cam_newmaterial.h \
    cam_importmat.h \
    FileIO/cam_fileio.h \
    StringOperation/stringoperation.h \
    dxfSrc/dl_attributes.h \
    dxfSrc/dl_codes.h \
    dxfSrc/dl_creationadapter.h \
    dxfSrc/dl_creationinterface.h \
    dxfSrc/dl_dxf.h \
    dxfSrc/dl_entities.h \
    dxfSrc/dl_exception.h \
    dxfSrc/dl_extrusion.h \
    dxfSrc/dl_writer.h \
    dxfSrc/dl_writer_ascii.h \
    CamDxf/camdxf.h \
    CamDxf/qpointfwithparent.h \
    CamDxf/campart.h \
    CamDxf/camleads.h \
    CamDxf/camswap.h \
    CamDxf/camscene.h \
    cam_newprt.h \
    cam_importprt.h \
    dxfSrc/new/dl_attributes.h \
    dxfSrc/new/dl_codes.h \
    dxfSrc/new/dl_creationadapter.h \
    dxfSrc/new/dl_creationinterface.h \
    dxfSrc/new/dl_dxf.h \
    dxfSrc/new/dl_entities.h \
    dxfSrc/new/dl_exception.h \
    dxfSrc/new/dl_extrusion.h \
    dxfSrc/new/dl_global.h \
    dxfSrc/new/dl_writer.h \
    dxfSrc/new/dl_writer_ascii.h \
    dxfSrc/old/dl_attributes.h \
    dxfSrc/old/dl_codes.h \
    dxfSrc/old/dl_creationadapter.h \
    dxfSrc/old/dl_creationinterface.h \
    dxfSrc/old/dl_dxf.h \
    dxfSrc/old/dl_entities.h \
    dxfSrc/old/dl_exception.h \
    dxfSrc/old/dl_extrusion.h \
    dxfSrc/old/dl_writer.h \
    dxfSrc/old/dl_writer_ascii.h \
    CamUi/CamUiBasic/cam_workshop.h \
    CamUi/CamUiForms/cam_blockdlg.h \
    CamCores/CamEng/cam_document.h \
    CamCores/CamInfo/cam_position.h \
    CamCores/CamGeneration/cam_xmloutput.h \
    CamCores/CamSci/cam_sci.h \
    CamCores/CamEng/cam_pen.h \
    CamCores/CamEng/cam_layer.h \
    CamCores/CamEng/cam_block.h \
    CamCores/CamEng/cam_blocklist.h \
    CamCores/CamEng/cam_layerlist.h \
    CamCores/CamEng/cam_entity.h \
    CamCores/CamEng/cam_entity_container.h \
    CamCores/CamEng/cam_vector.h \
    CamCores/CamEng/cam_math.h \
    CamThirdParty/muparser/include/muParser.h \
    CamThirdParty/muparser/include/muParserBase.h \
    CamThirdParty/muparser/include/muParserBytecode.h \
    CamThirdParty/muparser/include/muParserCallback.h \
    CamThirdParty/muparser/include/muParserDef.h \
    CamThirdParty/muparser/include/muParserError.h \
    CamThirdParty/muparser/include/muParserFixes.h \
    CamThirdParty/muparser/include/muParserStack.h \
    CamThirdParty/muparser/include/muParserTemplateMagic.h \
    CamThirdParty/muparser/include/muParserToken.h \
    CamThirdParty/muparser/include/muParserTokenReader.h \
    CamCores/CamDebug/cam_debug.h

FORMS    += camexpert.ui \
    cam_newmaterial.ui \
    cam_importmat.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    Scripts/Edits/App/App.js \
    Scripts/Files/File.js

SUBDIRS += \
    CamThirdParty/muparser/muparser.pro
