#include "qt/include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dimesion_(3),
    first_create_(true)
{
    n_digital_ = new NDigital();

    open_table_model_ = new QStandardItemModel();
    close_table_model_ = new QStandardItemModel();
    result_table_model_ = new QStandardItemModel();

    // init model
    open_table_model_->setColumnCount(3);
    open_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    open_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("层数"));
    open_table_model_->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("估价值"));

    close_table_model_->setColumnCount(3);
    close_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    close_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("层数"));
    close_table_model_->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("估价值"));

    result_table_model_->setColumnCount(3);
    result_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    result_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("层数"));
    result_table_model_->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("估价值"));

    ui->setupUi(this);

    ui->OrigionNode->setSpacing(0);
    ui->GoalNode->setSpacing(0);
    ui->OpenResult->setSpacing(0);
    ui->MinOpenResult->setSpacing(0);
    ui->CloseResult->setSpacing(0);
    ui->ResultLayout->setSpacing(0);

    ui->SelectDimesion->setSuffix("数码难题");
    ui->SelectDimesion->setSingleStep(7);
    ui->SelectDimesion->setRange(8, 15);

    ui->SelectHFunction->setSuffix("F");
    ui->SelectHFunction->setSingleStep(1);
    ui->SelectHFunction->setRange(0, 1);

    // setTable
    ui->openTable->setModel(open_table_model_);
    ui->openTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->openTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->openTable->setColumnWidth(0,40);
    ui->openTable->setColumnWidth(1,40);
    ui->openTable->setColumnWidth(2,55);
    ui->openTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->openTable->setAlternatingRowColors(true);
    ui->openTable->verticalHeader()->hide();
    ui->openTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->openTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->openTable->selectionModel(),&QItemSelectionModel::currentChanged,this,&MainWindow::onOpenTableCurrenChange);

    ui->closeTable->setModel(close_table_model_);
    ui->closeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->closeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->closeTable->setColumnWidth(0,40);
    ui->closeTable->setColumnWidth(1,40);
    ui->closeTable->setColumnWidth(2,55);
    ui->closeTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->closeTable->setAlternatingRowColors(true);
    ui->closeTable->verticalHeader()->hide();
    ui->closeTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->closeTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->closeTable->selectionModel(),&QItemSelectionModel::currentChanged,this,&MainWindow::onCloseTableCurrenChange);

    ui->resultTable->setModel(result_table_model_);
    ui->resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->resultTable->setColumnWidth(0,40);
    ui->resultTable->setColumnWidth(1,40);
    ui->resultTable->setColumnWidth(2,55);
    ui->resultTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->resultTable->setAlternatingRowColors(true);
    ui->resultTable->verticalHeader()->hide();
    ui->resultTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->resultTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->resultTable->selectionModel(),&QItemSelectionModel::currentChanged,this,&MainWindow::onResultTableCurrenChange);

    // set button
    setProcessButton(false);

    // set Map
    ui->ResultLayout->setSpacing(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SelectDimesion_valueChanged(int arg1)
{
    dimesion_ = sqrt(arg1 + 1);
//    ui->TestLabel->setText(QString::number(arg1)+"__"+QString::number(dimesion_));
}

void MainWindow::on_SelectHFunction_valueChanged(int arg1)
{
    n_digital_->setFFunction(arg1);
}

void MainWindow::cleanNode(QGridLayout *g_layout)
{
    QLayoutItem *child;
    while(child=g_layout->layout()->takeAt(0))
    {
        // 防止删除后不消失
        if(child->widget())
        {
            child->widget()->setParent(NULL);
        }

        delete child;
    }
}

void MainWindow::on_CreateTable_clicked()
{
    if (first_create_)
    {
        first_create_ = false;
    }
    else
    {
        cleanr();
    }

    if (dimesion_ == 3)
    {
        n_digital_->getDefault3Map(origion_, goal_);
        n_digital_->setMap3(origion_, goal_);
    }
    else if (dimesion_ == 4)
    {
        n_digital_->getDefault4Map(origion_, goal_);
        n_digital_->setMap4(origion_, goal_);
    }
    else
    {
        QMessageBox::information(this,"提示","维度过高, 请重设置维度");
        return;
    }

    for (int i = 0; i < dimesion_; i++)
    {
        for (int j = 0; j < dimesion_; j++)
        {
            ui->GoalNode->addWidget(newLable(goal_[i][j]), i, j, 1, 1);
            ui->OrigionNode->addWidget(newLable(origion_[i][j]), i, j, 1, 1);
        }
    }

    setProcessButton(true);
}

QLabel* MainWindow::newLable(int num)
{
    QLabel *new_label;
    if (num == 0)
    {
        new_label = new QLabel(" ");
    }
    else
    {
        new_label = new QLabel(QString::number(num));
    }

    new_label->setFrameShape (QFrame::Box);
    return new_label;
}

void MainWindow::on_Process_clicked()
{
    int result = 1;
    while (result != 0)
    {
        result = n_digital_->findNext();
        if (result == -2)
        {
            setProcessButton(false);
            QMessageBox::information(this,"提示","无解");
            return;
        }
    }
    updateTable();
    updateMap();
    endProcess();
}

void MainWindow::on_ProcessOnce_clicked()
{
    int result = n_digital_->findNext();
    if (result == -2)
    {
        setProcessButton(false);
        QMessageBox::information(this,"提示","无解");
        return;
    }

    updateTable();
    updateMap();
    if (result == 0)
    {
        endProcess();
    }
}

void MainWindow::onOpenTableCurrenChange(const QModelIndex current, const QModelIndex &previous)
{
    QModelIndex index = current.sibling(current.row(),0);
    QStandardItem* item = open_table_model_->itemFromIndex(index);
    if (item)
    {
//        QMessageBox::information(this,"提示", item->text());
        int current_id = item->text().toInt();
        cleanNode(ui->OpenResult);
        std::vector<std::vector<int>> open_map = n_digital_->getOpeneMap(current_id);

        for (int i = 0; i < dimesion_; i++)
        {
            for (int j = 0; j < dimesion_; j++)
            {
                ui->OpenResult->addWidget(newLable(open_map[i][j]), i, j, 1, 1);
            }
        }
    }
}

void MainWindow::onCloseTableCurrenChange(const QModelIndex current, const QModelIndex &previous)
{
    QModelIndex index = current.sibling(current.row(),0);
    QStandardItem* item = close_table_model_->itemFromIndex(index);
    if (item)
    {
        cleanNode(ui->CloseResult);
        std::vector<std::vector<int>> close_map = n_digital_->getCloseMap(current.row());
        for (int i = 0; i < dimesion_; i++)
        {
            for (int j = 0; j < dimesion_; j++)
            {
                ui->CloseResult->addWidget(newLable(close_map[i][j]), i, j, 1, 1);
            }
        }
    }
}

void MainWindow::onResultTableCurrenChange(const QModelIndex current, const QModelIndex &previous)
{
    QModelIndex index = current.sibling(current.row(),0);
    QStandardItem* item = result_table_model_->itemFromIndex(index);
    if (item)
    {
        cleanNode(ui->ResultLayout);
        std::vector<std::vector<int>> result_map = n_digital_->getResultMap(current.row());
        for (int i = 0; i < dimesion_; i++)
        {
            for (int j = 0; j < dimesion_; j++)
            {
                ui->ResultLayout->addWidget(newLable(result_map[i][j]), i, j, 1, 1);
            }
        }
    }
}

void MainWindow::updateTable()
{
    // clean table
    open_table_model_->removeRows(0, open_table_model_->rowCount());
    close_table_model_->removeRows(0, close_table_model_->rowCount());

    // get tips
    std::vector<TableTips> open_tips = n_digital_->getOpenTableTips();
    std::vector<TableTips> close_tips = n_digital_->getCloseTableTips();

    if (open_tips.size() <= 0)
    {
        setProcessButton(false);
        QMessageBox::information(this,"提示","无解");
    }


    // update open table
    for (int i = 0; i < open_tips.size(); i++)
    {
        open_table_model_->setItem(i, 0, new QStandardItem(QString::number(open_tips[i].id)));
        open_table_model_->setItem(i, 1, new QStandardItem(QString::number(open_tips[i].layer)));
        open_table_model_->setItem(i, 2, new QStandardItem(QString::number(open_tips[i].value)));
    }

    // update close table
    for (int i = 0; i < close_tips.size(); i++)
    {
        close_table_model_->setItem(i, 0, new QStandardItem(QString::number(close_tips[i].id)));
        close_table_model_->setItem(i, 1, new QStandardItem(QString::number(close_tips[i].layer)));
        close_table_model_->setItem(i, 2, new QStandardItem(QString::number(close_tips[i].value)));
    }
}

void MainWindow::updateMap()
{
    // clean map
    cleanNode(ui->CloseResult);
    cleanNode(ui->MinOpenResult);
    cleanNode(ui->OpenResult);

    // get map
    std::vector<std::vector<int>> close_map = n_digital_->getEndCloseMap();
    std::vector<std::vector<int>> min_open_map = n_digital_->getMinOpeneMap();

    // update close_map
    for (int i = 0; i < dimesion_; i++)
    {
        for (int j = 0; j < dimesion_; j++)
        {
            ui->CloseResult->addWidget(newLable(close_map[i][j]), i, j, 1, 1);
            ui->MinOpenResult->addWidget(newLable(min_open_map[i][j]), i, j, 1, 1);
            ui->OpenResult->addWidget(newLable(min_open_map[i][j]), i, j, 1, 1);
        }
    }
}

void MainWindow::updateResultTable()
{
    // clean result map
    result_table_model_->removeRows(0, result_table_model_->rowCount());

    //get map
    std::vector<TableTips> result_tips = n_digital_->getResultTableTips();

    //update result table
    for (int i = 0; i < result_tips.size(); i++)
    {
        result_table_model_->setItem(i, 0, new QStandardItem(QString::number(result_tips[i].id)));
        result_table_model_->setItem(i, 1, new QStandardItem(QString::number(result_tips[i].layer)));
        result_table_model_->setItem(i, 2, new QStandardItem(QString::number(result_tips[i].value)));
    }
}

void MainWindow::setProcessButton(bool enable)
{
    ui->ProcessOnce->setEnabled(enable);
    ui->Process->setEnabled(enable);
}

void MainWindow::on_Clean_clicked()
{
    cleanr();

    setProcessButton(false);
}

void MainWindow::endProcess()
{
    QMessageBox::information(this,"提示","查找完成");

    updateResultTable();
    setProcessButton(false);
}

void MainWindow::cleanr()
{
    n_digital_->cleanr();

    // clean map
    cleanNode(ui->CloseResult);
    cleanNode(ui->MinOpenResult);
    cleanNode(ui->OpenResult);
    cleanNode(ui->OrigionNode);
    cleanNode(ui->GoalNode);
    cleanNode(ui->ResultLayout);

    // clean table
    open_table_model_->removeRows(0, open_table_model_->rowCount());
    close_table_model_->removeRows(0, close_table_model_->rowCount());
    result_table_model_->removeRows(0, close_table_model_->rowCount());
}
