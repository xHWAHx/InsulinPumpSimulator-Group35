#include "history.h"
#include "ui_history.h"
#include "datalogger.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>

History::History(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::History),
      m_logger(DataLogger::instance(this))
{
    ui->setupUi(this);

    ui->comboBox->setEditable(false);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &History::refreshHistory);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &History::refreshHistory);
    connect(m_logger, &DataLogger::logsUpdated, this, &History::refreshHistory);

    QTimer::singleShot(0, this, SLOT(refreshHistory()));
}

History::~History()
{
    delete ui;
}

void History::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        event->ignore();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void History::onBackButtonClicked()
{
    emit backRequested();
    close();
}

void History::onSearch()
{
    refreshHistory();
}

void History::onFilterChanged(const QString &)
{
    refreshHistory();
}

void History::refreshHistory()
{
    QList<LogEntry> allLogs = m_logger->retrieveHistory();

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

void History::on_logoButton_clicked()
{
    emit backToHome();
    this-> close();
}

