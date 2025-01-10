#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uye_islem.h"
#include "ui_uye_islem.h"
#include "kitap_islem.h"
#include "ui_kitap_islem.h"
#include "odunc_alma.h"
#include "ui_odunc_alma.h"
#include "odunc_teslim.h"
#include "ui_odunc_teslim.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    // butonlara resim ekleme
    ui->pushButton->setIcon(QIcon(":/new/prefix1/user.jpg"));
    ui->pushButton_kitap_islem->setIcon(QIcon(":/new/prefix1/book.jpg"));
    ui->pushButton_odunc_alma->setIcon(QIcon(":/new/prefix1/odunc_1.jpg"));
    ui->pushButton_odunc_teslim->setIcon(QIcon(":/new/prefix1/odunc_2.jpg"));
    ui->pushButton->setIconSize(QSize(150, 150));
    ui->pushButton_kitap_islem->setIconSize(QSize(150, 150));
    ui->pushButton_odunc_alma->setIconSize(QSize(150, 150));
    ui->pushButton_odunc_teslim->setIconSize(QSize(150, 150));

    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");
    if(!database.open()){
        ui->statusbar->showMessage("baglanilamadi.");

    }else{
        ui->statusbar->showMessage("baglanildi.");
    }
    sorgu = new QSqlQuery(database);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    uye_islem * ekran = new uye_islem();
    ekran ->show();
}


void MainWindow::on_pushButton_kitap_islem_clicked()
{
    kitap_islem * ekran = new kitap_islem();
    ekran->show();
}


void MainWindow::on_pushButton_odunc_alma_clicked()
{
    odunc_alma * ekran = new odunc_alma();
    ekran->show();
}


void MainWindow::on_pushButton_odunc_teslim_clicked()
{
    odunc_teslim * ekran = new odunc_teslim();
    ekran->show();
}

