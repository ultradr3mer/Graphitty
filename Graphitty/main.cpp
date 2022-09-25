/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "startview.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <mainview.h>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QTranslator translator;

  const QStringList uiLanguages = QLocale::system().uiLanguages();

  for (const QString& locale : uiLanguages)
  {
    const QString baseName = "Graphitty_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName))
    {
      a.autoSipEnabled();
      a.installTranslator(&translator);

      break;
    }
  }

  MainView m;
  StartView w(&m);
  w.exec();

  auto fileName = w.getFileName();
  if (!fileName.isEmpty())
  {
    m.openProject(fileName);
  }
  m.show();

  return a.exec();
}
