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
  this->projects = new RecentViewModel();
  this->collectRecentProjects();
  this->ui->recentList->setModel(this->projects);
}

StartView::~StartView()
{
  delete ui;
}

void StartView::collectRecentProjects()
{
    QString val;
    QFile file;
    QString fileName = "recent.json";
    QStringList recentProjects = {};

    file.setFileName(fileName);

    if(QFileInfo::exists(fileName)) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject values = doc.object();

        QJsonArray projects = values["projects"].toArray();

        if (!(projects.isEmpty())) {
            foreach (const QJsonValue & project, projects) {
                QString projectPath = project.toString();

                fileName = projectPath;
                if(QFileInfo::exists(fileName)) {
                    recentProjects.append(projectPath);
                }
            }
        }
    } else {
        QJsonDocument* document = new QJsonDocument(QJsonArray());

        auto projectList = QJsonObject({
            qMakePair(QString("projects"), QJsonValue()),
                           });
        document->setObject(projectList);

        file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        file.write(document->toJson());
        file.close();
        delete(document);
    }

    std::reverse(recentProjects.begin(), recentProjects.end());
    this->projects->setRecentFiles(recentProjects);

}

void StartView::on_newProject_clicked()
{
  this->close();
}

void StartView::on_openProject_clicked()
{
  this->fileName =
      QFileDialog::getOpenFileName(this, tr("Open Project"), "/home", tr("Json Files (*.json)"));

  this->close();
}

void StartView::on_recentList_clicked(const QModelIndex &index)
{
  this->fileName = index.data(Qt::DisplayRole).toString();
  this->close();
}

