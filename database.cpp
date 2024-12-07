#include "database.h"
#include "ui_database.h"
#include <QtSql>

using namespace std;

DataBase::DataBase(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataBase)
{
    ui->setupUi(this);

    disable_del_add_filt_sort_menu();
}

DataBase::~DataBase()
{
    delete ui;
    delete query;
    delete model;
}


void DataBase::on_open_DB_button_clicked()
{
    QString dbName = "";
    dbName = ui->input_db_name->text();
    m_db = QSqlDatabase::addDatabase("QSQLITE"); //соединение объекта базы данных с СУБД
    if (dbName == "") {
        m_db.setDatabaseName("myDB"); //определение имени базы данных
    } else {
        m_db.setDatabaseName(dbName);
    }

    query = new QSqlQuery(m_db); // создание объекта для запроса
    if(!m_db.open()) { // проверка на ошибку при открытии или создании базы данных
        throw "can't open database";
    }
    if(!m_db.tables().contains("univer")) { // если в базе не существует таблица Person, то создание таблицы Рerson и заполнение данными
        query->clear(); // очистка запроса
        query->exec("CREATE TABLE univer(id INTEGER PRIMARY KEY NOT NULL, professor VARCHAR(25), kafedra VARCHAR(25), subject VARCHAR(25), hours INTEGER, exp INTEGER, stavka FLOAT);"); // исполнение запроса на добавление записи
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(0, 'Smirnova S.S.', 'Geography', 'Physical geography', 50, 16, 1.65);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(1, 'Ivanov I.I.', 'Mathematics', 'Algebra', 40, 15, 1.5);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(2, 'Petrov P.P.', 'Physics', 'Dlinnoe', 50, 20, 1.8);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(3, 'Sidorov S.S.', 'Computer Science', 'Programming', 60, 10, 2.0);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(4, 'Alekseev A.A', 'History', 'Russian History', 30, 25, 1.7);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(5, 'Nikolaev N.N.', 'Biology', 'Botany', 35, 8, 1.6);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(6, 'Kuznetsova E.E.', 'Literature', 'Russian Literature', 40, 12, 1.4);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(7, 'Orlov O.O.', 'Chemistry', 'Organic Chemistry', 45, 18, 1.9);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(8, 'Tarasov T.T.', 'Economics', 'Microeconomics', 55, 22, 1.75);");
        query->clear();
        query->exec("INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(9, 'Mironov M.M.', 'Philosophy', 'Ethics', 25, 7, 1.3);");
    }
    enable_del_add_filt_sort_menu();
    refresh_model();
}


void DataBase::on_table_comboBox_activated(int index)
{
    model = new QSqlTableModel(this, m_db); // создание редактируемой модели базы данных
    QString s = ui->table_comboBox->currentText();
    model->setTable(s); // создание модели таблицы Person
    model->select(); // заполнение модели данными
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // выбор стратегии сохранения изменений в базе данных - сохранение происходит при переходе к другому полю
    ui->tableView->setModel(model); // соединение модели и ее табличного представления в форме
    if (s == "univer") {
        enable_del_add_filt_sort_menu();
    } else {
        disable_del_add_filt_sort_menu();
    }

}


void DataBase::on_del_but_clicked()
{
    if (ui->del_id->text() == "") {
        qInfo() << "no ID";
        return;
    }
    query->clear();
    query->exec("DELETE FROM univer WHERE id=" + ui->del_id->text());
    refresh_model();
}


void DataBase::on_add_but_clicked()
{
    if (ui->add_id->text() == "") {
        qInfo() << "no ID";
        return;
    }
    query->clear();
    //QString buf = "INSERT INTO univer(id, professor, kafedra, subject, hours, exp, stavka) VALUES(" + ui->add_id->text() + ", '" + ui->add_professor->text() + "', '" + ui->add_kafedra->text() + "', '" + ui->add_subject->text() + "', " + ui->add_hours->text() + "," + ui->add_exp->text() + "," + ui->add_stavka->text() + ");";
    QString bufL = "INSERT INTO univer(id";
    QString bufR = " VALUES(" + ui->add_id->text();
    if (ui->add_professor->text() != "") {
        bufL += ", professor";
        bufR += ", '" + ui->add_professor->text() + "'";
    }
    if (ui->add_kafedra->text() != "") {
        bufL += ", kafedra";
        bufR += ", '" + ui->add_kafedra->text() + "'";
    }
    if (ui->add_subject->text() != "") {
        bufL += ", subject";
        bufR += ", '" + ui->add_subject->text() + "'";
    }
    if (ui->add_hours->text() != "") {
        bufL += ", hours";
        bufR += ", " + ui->add_hours->text();
    }
    if (ui->add_exp->text() != "") {
        bufL += ", exp";
        bufR += ", " + ui->add_exp->text();
    }
    if (ui->add_stavka->text() != "") {
        bufL += ", stavka";
        bufR += ", " + ui->add_stavka->text();
    }
    bufL += ")";
    bufR += ");";
    qInfo() << "buf = " + bufL + bufR;
    query->exec(bufL + bufR);
    refresh_model();
}


void DataBase::on_filt_hours30_clicked()
{
    model->setFilter("hours<=30");
    model->select();
    ui->tableView->setModel(model);
}


void DataBase::on_filt_hours50_clicked()
{
    model->setFilter("hours>50");
    model->select();
    ui->tableView->setModel(model);
}


void DataBase::on_filt_stop_clicked()
{
    refresh_model();
}


void DataBase::on_sort_ASC_clicked()
{
    QString s = ui->sort_comboBox->currentText();
    int n = 0;
    if (s == "профессор") {//
        n = 1;
    } else if (s == "кафедра") {
        n = 2;
    } else if (s == "предмет") {
        n = 3;
    } else if (s == "часы") {
        n = 4;
    } else if (s == "опыт") {
        n = 5;
    } else if (s == "ставка") {
        n = 6;
    }
    model->setSort(n, Qt::SortOrder::AscendingOrder);
    model->select();
    ui->tableView->setModel(model);
}


void DataBase::on_sort_DESC_clicked()
{
    QString s = ui->sort_comboBox->currentText();
    int n = 0;
    if (s == "профессор") {//
        n = 1;
    } else if (s == "кафедра") {
        n = 2;
    } else if (s == "предмет") {
        n = 3;
    } else if (s == "часы") {
        n = 4;
    } else if (s == "опыт") {
        n = 5;
    } else if (s == "ставка") {
        n = 6;
    }
    model->setSort(n, Qt::SortOrder::DescendingOrder);
    model->select();
    ui->tableView->setModel(model);
}


void DataBase::on_sort_stop_clicked()
{
    refresh_model();
}


void DataBase::refresh_model()
{
    model = new QSqlTableModel(this, m_db); // создание редактируемой модели базы данных
    model->setTable("univer"); // создание модели таблицы Person
    model->select(); // заполнение модели данными
    model->setEditStrategy(QSqlTableModel::OnFieldChange); // выбор стратегии сохранения изменений в базе данных - сохранение происходит при переходе к другому полю
    ui->tableView->setModel(model); // соединение модели и ее табличного представления в форме
    for (int i = ui->table_comboBox->count(); i > 0; i--) {
        ui->table_comboBox->removeItem(i-1);
    }
    ui->table_comboBox->insertItems(0, m_db.tables());
    ui->table_comboBox->setCurrentIndex(0);
}


void DataBase::disable_del_add_filt_sort_menu()
{
    ui->del->setDisabled(true);
    ui->del_label->setDisabled(true);
    ui->del_id->setDisabled(true);
    ui->del_but->setDisabled(true);

    ui->add->setDisabled(true);
    ui->add_label0->setDisabled(true);
    ui->add_label1->setDisabled(true);
    ui->add_label2->setDisabled(true);
    ui->add_label3->setDisabled(true);
    ui->add_label4->setDisabled(true);
    ui->add_label5->setDisabled(true);
    ui->add_label6->setDisabled(true);
    ui->add_id->setDisabled(true);
    ui->add_exp->setDisabled(true);
    ui->add_hours->setDisabled(true);
    ui->add_stavka->setDisabled(true);
    ui->add_kafedra->setDisabled(true);
    ui->add_professor->setDisabled(true);
    ui->add_subject->setDisabled(true);
    ui->add_but->setDisabled(true);

    ui->filt->setDisabled(true);
    ui->filt_hours30->setDisabled(true);
    ui->filt_hours50->setDisabled(true);
    ui->filt_stop->setDisabled(true);

    ui->sort->setDisabled(true);
    ui->sort_comboBox->setDisabled(true);
    ui->sort_ASC->setDisabled(true);
    ui->sort_DESC->setDisabled(true);
    ui->sort_stop->setDisabled(true);
}


void DataBase::enable_del_add_filt_sort_menu()
{
    ui->del->setDisabled(false);
    ui->del_label->setDisabled(false);
    ui->del_id->setDisabled(false);
    ui->del_but->setDisabled(false);

    ui->add->setDisabled(false);
    ui->add_label0->setDisabled(false);
    ui->add_label1->setDisabled(false);
    ui->add_label2->setDisabled(false);
    ui->add_label3->setDisabled(false);
    ui->add_label4->setDisabled(false);
    ui->add_label5->setDisabled(false);
    ui->add_label6->setDisabled(false);
    ui->add_id->setDisabled(false);
    ui->add_exp->setDisabled(false);
    ui->add_hours->setDisabled(false);
    ui->add_stavka->setDisabled(false);
    ui->add_kafedra->setDisabled(false);
    ui->add_professor->setDisabled(false);
    ui->add_subject->setDisabled(false);
    ui->add_but->setDisabled(false);

    ui->filt->setDisabled(false);
    ui->filt_hours30->setDisabled(false);
    ui->filt_hours50->setDisabled(false);
    ui->filt_stop->setDisabled(false);

    ui->sort->setDisabled(false);
    ui->sort_comboBox->setDisabled(false);
    ui->sort_ASC->setDisabled(false);
    ui->sort_DESC->setDisabled(false);
    ui->sort_stop->setDisabled(false);
}

