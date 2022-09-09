#include "startview.h"

#include <MainView.h>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

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
