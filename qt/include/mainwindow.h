#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QStandardItemModel>

#include <math.h>
#include <vector>

#include "include/n_digital/n_digital.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_SelectDimesion_valueChanged(int arg1);

    void on_CreateTable_clicked();

    void on_Process_clicked();
    void on_ProcessOnce_clicked();

    void onOpenTableCurrenChange(const QModelIndex current, const QModelIndex &previous);
    void onCloseTableCurrenChange(const QModelIndex current, const QModelIndex &previous);

    void on_Clean_clicked();

private:
    Ui::MainWindow *ui;
    int dimesion_;

    QStandardItemModel *open_table_model_;
    QStandardItemModel *close_table_model_;

    std::vector<std::vector<int>> origion_state_;
    std::vector<std::vector<int>> goal_state_;

    void cleanNode(QGridLayout *g_layout);
    void initTables();
    void updateTable();
    void updateMap();
    void setProcessButton(bool enable);
    void cleanr();
    QLabel* newLable(int num);

// test
private:
    int test_ = 0;
    NDigital *n_digital_;
    std::vector<std::vector<int>> origion_;
    std::vector<std::vector<int>> goal_;

    bool first_create_;
};

#endif // MAINWINDOW_H
