#include "startview.h"

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
  StartView w;
  w.show();
  return a.exec();
}
