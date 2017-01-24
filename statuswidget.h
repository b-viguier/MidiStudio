#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QDockWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();

private:
    Ui::StatusWidget *ui;
};

#endif // STATUSWIDGET_H
