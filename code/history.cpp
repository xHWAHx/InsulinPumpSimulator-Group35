#include "history.h"
#include "ui_history.h"
#include "datalogger.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QKeyEvent>

history::history(datalogger *logger, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Dialog),
      m_logger(logger)
{
    ui->setupUi(this);
    
    ui->pushButton->setText("Back");

    ui->comboBox->setEditable(false);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &history::refreshHistory);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &history::refreshHistory);
    connect(ui->pushButton, &QPushButton::clicked, this, &history::onBackButtonClicked);
    connect(m_logger, &datalogger::logsUpdated, this, &history::refreshHistory);
    
    refreshHistory();
}

history::~history()
{
    delete ui;
}

void history::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        event->ignore();
    } else {
        QDialog::keyPressEvent(event);
    }
}

void history::onBackButtonClicked()
{
    emit backRequested();
    close();
}

void history::onSearch()
{
    refreshHistory();
}

void history::onFilterChanged(const QString &)
{
    refreshHistory();
}

void history::refreshHistory()
{
    int profileId = 1;
    QList<LogEntry> allLogs = m_logger->retrieveHistory(profileId);
    
    QString query = ui->lineEdit->text().trimmed().toLower();
    QString eventFilter = ui->comboBox->currentText().trimmed().toLower();
    if (eventFilter == "all")
        eventFilter = "";
    
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    
    for (const LogEntry &entry : allLogs) {
        QString ts = entry.timestamp.toString(Qt::ISODate).toLower();
        QString type = entry.eventType.toLower();
        QString desc = entry.description.toLower();
        
        if (!query.isEmpty() &&
            (ts.indexOf(query) == -1 && type.indexOf(query) == -1 && desc.indexOf(query) == -1))
        {
            continue;
        }
        
        if (!eventFilter.isEmpty() && type != eventFilter) {
            continue;
        }
        
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(entry.timestamp.toString(Qt::ISODate)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(entry.eventType));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(entry.description));
    }
}
