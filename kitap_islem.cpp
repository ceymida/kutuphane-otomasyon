#include "kitap_islem.h"
#include "ui_kitap_islem.h"

kitap_islem::kitap_islem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::kitap_islem)
{
    ui->setupUi(this);
    // veri tabanı baglantisi
    //database.setDatabaseName("/Users/ceydaunal/Desktop/veritabanı/kutuphaneOtomasyon.db");
    // Ben ustteki sekilde calistirabildim. ama resource dosyamda kutuphaneOtomasyon.db mevcut bu sekilde calistirabilrisiniz.
    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");


    if (!database.open()) {
        QMessageBox::critical(this, "Hata", "Veritabanı bağlantısı başarısız: " + database.lastError().text(), "OK");
    }
    sorgu = new QSqlQuery(database);
    //QSqlQuery sorgu;
    listele();
}

kitap_islem::~kitap_islem()
{
    delete ui;
}

void kitap_islem::listele()
{
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model1 = new QSqlQueryModel();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView->setModel(model1);

}

void kitap_islem::on_pushButton_yeni_kayit_clicked()
{
    if(ui->lineEdit_kitap_no->text().isEmpty() || ui->lineEdit_kitap_ad->text().isEmpty() || ui->lineEdit_kitap_stok->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }


    sorgu->prepare("select * from kitap where kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu->exec();
    if(sorgu->next()){
        QMessageBox::critical(this,"hata","bu kitap nosuna ait baska kitap vardir","ok");
        return;
    }
    else{
        sorgu->prepare("insert into kitap(kitap_no,kitap_ad,kitap_sayisi) values(?,?,?)");
        sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
        sorgu->addBindValue(ui->lineEdit_kitap_ad->text());
        sorgu->addBindValue(ui->lineEdit_kitap_stok->text());
        sorgu->exec();
        QMessageBox::information(this, "Başarılı", "Yeni kitap başarıyla eklendi", "OK");
    }
    int no = ui->lineEdit_kitap_no->text().toInt();
    QString ad = ui->lineEdit_kitap_ad->text();
    QString soyad = ui->lineEdit_kitap_stok->text();
    listele();

}


void kitap_islem::on_pushButton_guncelle_clicked()
{

    if(ui->lineEdit_kitap_no->text().isEmpty() || ui->lineEdit_kitap_ad->text().isEmpty() || ui->lineEdit_kitap_stok->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("UPDATE kitap SET kitap_ad = ?, kitap_sayisi = ? WHERE kitap_no = ?");
    sorgu.addBindValue(ui->lineEdit_kitap_ad->text());
    sorgu.addBindValue(ui->lineEdit_kitap_stok->text());
    sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu.exec();

    QMessageBox::information(this, "Başarılı", "Kitap bilgileri başarıyla güncellendi", "OK");

    updateTableView();
}


void kitap_islem::on_pushButton_sil_clicked()
{
    if(ui->lineEdit_kitap_no->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Silinecek üye no'sunu giriniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("SELECT * FROM odunc WHERE kitap_no = ? AND teslim_tarihi IS NULL");
    sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu.exec();

    if (sorgu.next()) {
        QMessageBox::critical(this, "Hata", "  kitap silinemez.", "OK");
    } else {
        sorgu.prepare("DELETE FROM kitap WHERE kitap_no = ?");
        sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
        sorgu.exec();

        QMessageBox::information(this, "Başarılı", "Kitap başarıyla silindi", "OK");

        updateTableView();
    }

}

void kitap_islem::updateTableView()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query("SELECT * FROM kitap");
    model->setQuery(query);
    ui->tableView->setModel(model);
}

