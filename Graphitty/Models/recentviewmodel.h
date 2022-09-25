#include <QStringListModel>

class RecentViewModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit RecentViewModel(QObject* parent = nullptr);
    void setRecentFiles(QStringList projects);

private:
    QStringList* files;
};
