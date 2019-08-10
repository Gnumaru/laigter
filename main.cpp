/*
 * Laigter: an automatic map generator for lighting effects.
 * Copyright (C) 2019  Pablo Ivan Fonovich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * Contact: azagaya.games@gmail.com
 */

#include "mainwindow.h"
#include "src/imageprocessor.h"
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QCommandLineParser>

static QString presetCodes[30] = {"EnhanceHeight ", "EnhanceSoft ", "BumpHeight ",
                                  "BumpDistance", "BumpSoft ", "BumpCut ", "Tile ", "InvertX ",
                                  "InvertY ", "ParallaxType", "BinaryThreshold ",
                                  "BinaryFocus ", "ParallaxSoft ", "BinaryMinHeight ",
                                  "BinaryErodeDilate ", "HeightMapBrightness ",
                                  "HeightMapContrast ", "InvertParallax ", "SpecularBlur ",
                                  "SpecularBright ", "SpecularContrast ", "SpecularThresh ",
                                  "SpecularInvert ", "OcclusionBlur ", "OcclusionBright ",
                                  "OcclusionInvert ", "OcclusionThresh ", "OcclusionContrast "
                                  "OcclusionDistance ", "OcclusionDistanceMode "};

void applyPreset(QString preset, ImageProcessor *p){
    QFile selected_preset(preset);
    QByteArray settings = selected_preset.readAll();
    QList<QByteArray> settings_list = settings.split('\n');

    for (int i=0; i< settings_list.count(); i++){
        QByteArray setting = settings_list.at(i);
        QList<QByteArray> aux = setting.split('\t');
        if (aux[0] == presetCodes[0]){
            p->set_normal_depth(aux[1].toInt());
        }else if (aux[0] == presetCodes[1]){
            p->set_normal_blur_radius(aux[1].toInt());
        }else if (aux[0] == presetCodes[2]){
            p->set_normal_bisel_depth(aux[1].toInt());
        }else if (aux[0] == presetCodes[3]){
            p->set_normal_bisel_distance(aux[1].toInt());
        }else if (aux[0] == presetCodes[4]){
            p->set_normal_bisel_blur_radius(aux[1].toInt());
        }else if (aux[0] == presetCodes[5]){
            p->set_normal_bisel_soft((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[6]){
            p->set_tileable((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[7]){
            p->set_normal_invert_x((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[8]){
            p->set_normal_invert_y((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[9]){
            p->set_parallax_type((ParallaxType)aux[1].toInt());
        }else if (aux[0] == presetCodes[10]){
            p->set_parallax_thresh(aux[1].toInt());
        }else if (aux[0] == presetCodes[11]){
            p->set_parallax_focus(aux[1].toInt());
        }else if (aux[0] == presetCodes[12]){
            p->set_parallax_soft(aux[1].toInt());
        }else if (aux[0] == presetCodes[13]){
            p->set_parallax_min(aux[1].toInt());
        }else if (aux[0] == presetCodes[14]){
            p->set_parallax_erode_dilate(aux[1].toInt());
        }else if (aux[0] == presetCodes[15]){
            p->set_parallax_brightness(aux[1].toInt());
        }else if (aux[0] == presetCodes[16]){
            p->set_parallax_contrast(aux[1].toInt());
        }else if (aux[0] == presetCodes[17]){
            p->set_parallax_invert((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[18]){
            p->set_specular_blur(aux[1].toInt());
        }else if (aux[0] == presetCodes[19]){
            p->set_specular_bright(aux[1].toInt());
        }else if (aux[0] == presetCodes[20]){
            p->set_specular_contrast(aux[1].toInt());
        }else if (aux[0] == presetCodes[21]){
            p->set_specular_thresh(aux[1].toInt());
        }else if (aux[0] == presetCodes[22]){
            p->set_specular_invert((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[23]){
            p->set_occlusion_blur(aux[1].toInt());
        }else if (aux[0] == presetCodes[24]){
            p->set_occlusion_bright(aux[1].toInt());
        }else if (aux[0] == presetCodes[25]){
            p->set_occlusion_invert((bool)aux[1].toInt());
        }else if (aux[0] == presetCodes[26]){
            p->set_occlusion_thresh(aux[1].toInt());
        }else if (aux[0] == presetCodes[27]){
            p->set_occlusion_contrast(aux[1].toInt());
        }else if (aux[0] == presetCodes[28]){
            p->set_occlusion_distance(aux[1].toInt());
        }else if (aux[0] == presetCodes[29]){
            p->set_occlusion_distance_mode((bool)aux[1].toInt());
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("laigter");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QString locale = QLocale::system().name().split("_").at(0);
    QTranslator translator;
    bool loaded = translator.load(":/laigter_"+locale);
    if (!loaded)
        translator.load(":/laigter_en");

#ifndef PORTABLE
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appData);
    if (!dir.exists())
        dir.mkpath(".");
    dir = QDir(appData+"/presets");
    if (!dir.exists())
        dir.mkpath(".");

#else
    QDir dir("./presets");
    if (!dir.exists())
        dir.mkpath(".");
#endif

    //QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("laigter gnumaru");
    QCoreApplication::setApplicationVersion("1.5.0.0");

    QApplication a(argc, argv);


    QCommandLineParser argsParser;
    argsParser.setApplicationDescription("Test helper");
    argsParser.addHelpOption();
    argsParser.addVersionOption();

    QCommandLineOption softOpenGl(QStringList() << "s" << "software-opengl", QCoreApplication::translate("softOpenGl", "Use software opengl renderer."));
    argsParser.addOption(softOpenGl);

//    QCommandLineOption version(QStringList() << "v" << "version" ,"show version and exit");
//    argsParser.addOption(version);

    QCommandLineOption inputDiffuseTextureOption(QStringList() << "d" << "diffuse",
        "diffuse texture to load",
        "diffuse");
    argsParser.addOption(inputDiffuseTextureOption);

    QCommandLineOption pressetOption(QStringList() << "p" << "preset",
        "presset to load",
        "preset");
    argsParser.addOption(pressetOption);

    argsParser.process(a);

    QString inputDiffuseTextureOptionValue = argsParser.value(inputDiffuseTextureOption);
    QString pressetOptionValue = argsParser.value(pressetOption);
    if(!inputDiffuseTextureOptionValue.trimmed().isEmpty()){
        qDebug() << inputDiffuseTextureOptionValue;
        qDebug() << pressetOptionValue;
        ImageLoader il;
        bool succes;
        //QString fileName;
        QImage auximage;
        auximage = il.loadImage(inputDiffuseTextureOptionValue , &succes);
        auximage = auximage.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
        ImageProcessor *processor = new ImageProcessor();
        processor->loadImage(inputDiffuseTextureOptionValue , auximage);
        if(!pressetOptionValue.trimmed().isEmpty()){
            applyPreset(pressetOptionValue, processor);
        }
        QImage normal;
        normal = processor->get_normal();
        QString name;
        name = inputDiffuseTextureOptionValue .append(".out.png");
        normal.save(name);
        return 0;
    }


    a.installTranslator(&translator);
    if (argc > 1){
        if (*argv[1] == 's'){
            a.setAttribute(Qt::AA_UseSoftwareOpenGL);
            qDebug() << "Soft OpenGL";
        }
    }
    MainWindow w;
    QGuiApplication::setWindowIcon(QIcon(":/images/laigter-icon.png"));
    w.show();
    qRegisterMetaType<ProcessedImage>("ProcessedImage");
    return a.exec();
}
