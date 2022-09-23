#include "startview.h"
#include "ui_startview.h"
#include <QFileDialog>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

StartView::StartView(QWidget* parent) : QDialog(parent), ui(new Ui::StartView)
{
  ui->setupUi(this);
}

StartView::~StartView()
{
  delete ui;
}

QJsonArray StartView::collectRecentProjects()
{
    QString val;
    QFile file;
    QString fileName = "recent.json";
    QJsonArray lastProjects = {};

    file.setFileName(fileName);

    if(QFileInfo::exists(fileName)) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject values = doc.object();

        QJsonArray projects = values["projects"].toArray();

        if (!(projects.isEmpty())) {

            // projects found
            foreach (const QJsonValue & project, projects) {
                QString projectPath = project.toString();

                fileName = projectPath;
                if(QFileInfo::exists(fileName)) {
                    lastProjects.append(projectPath);
                }
            }
        }
    }

    return lastProjects;

}

void StartView::on_newProject_clicked()
{
  //  auto* mainView = new MainView(this);
  this->close();
  //  mainView->show();
}

void StartView::on_openProject_clicked()
{
  this->fileName =
      QFileDialog::getOpenFileName(this, tr("Open Project"), "/home", tr("Json Files (*.json)"));

  this->close();
  //  MainView mainView;
  //  mainView.openProject(fileName);
  //  this->hide();
  //  mainView.show();
}




