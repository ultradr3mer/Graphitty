#include <QStringListModel>
#include <Models/mainviewmodel.h>

class SheetViewModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit SheetViewModel(QObject* parent = nullptr);
    void setSheetViews(MainViewModel model);

private:
    QStringList* views;
};
