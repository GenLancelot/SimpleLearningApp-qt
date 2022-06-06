#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->NextRoundBtn->setVisible(false);
    ui->Question->setVisible(false);
    ui->Answer->setVisible(false);
    ui->AcceptAnswer->setVisible(false);
    ui->StartAgainBtn->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_ExitBtn_clicked()
{
    QCoreApplication::exit();
}

void MainWindow::on_AcceptAnswer_clicked()
{
    QString provided = ui->Answer->text();
    ui->Answer->setText("");
    if(ValidateAnswer(provided))
    {
        AdjustProgressBarGood();
    }
    else
    {
        //#TODO: if answer is bad user will see valid answer and rewrite it
        AdjustProgressBarBad();
        badAnswer.emplace_back(currentQuestionData);
    }

    LoadNextQuestion();
    AdjustProgressBarAll();
}

void MainWindow::LoadNextQuestion()
{
    if(toGuess.empty())
    {
        ui->AcceptAnswer->setVisible(false);
        RoundEnd();
    }
    ++numberOfQuestion;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, toGuess.size() - 1);
    int toGet = distr(gen);
    currentQuestionData = toGuess[toGet];
    toGuess.erase(toGuess.begin() + toGet);
    toGuess.shrink_to_fit();

    ui->Question->setText(currentQuestionData.second);
    validAnswer = currentQuestionData.first;
}

bool MainWindow::ValidateAnswer(QString provided)
{
    return provided.toLower() == validAnswer.toLower();
}

void MainWindow::AdjustProgressBarAll()
{
    ui->All->setValue(numberOfQuestion);
}

void MainWindow::AdjustProgressBarGood()
{
    ui->Good->setValue(ui->Good->value() + 1);
}

void MainWindow::AdjustProgressBarBad()
{
    ui->Bad->setValue(ui->Bad->value() + 1);
}

void MainWindow::RoundEnd()
{
    if(badAnswer.empty())
    {
        //#TODO:
        //popout congrats window
    }
    else
    {
        ui->NextRoundBtn->setVisible(true);
    }
    numberOfQuestion = 0;
    toGuess = badAnswer;
    toGuess.shrink_to_fit();
    badAnswer.clear();
    badAnswer.shrink_to_fit();

}

void MainWindow::on_OpenFileBtn_clicked()
{
    QString filter = "All File (*.*) ;; Text File (*.txt)";
    QString fileName = QFileDialog::getOpenFileName(this,"opan a file", "C://", filter);
    QFile dataFile(fileName);

    if (!dataFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "title", "file not open");
    }
    else
    {
        QTextStream in(&dataFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList splitedLine = line.split(Separator);
            if(splitedLine.length() == 2)
            {
                std::pair<QString, QString> lineInPair = std::make_pair(splitedLine[0], splitedLine[1]);
                loadedData.emplace_back(lineInPair);
                toGuess.emplace_back(lineInPair);
            }
        }
        dataFile.close();
        ui->OpenFileBtn->setVisible(false);
        ui->StartAgainBtn->setVisible(true);
    }
}


void MainWindow::on_StartAgainBtn_clicked()
{
    ui->Question->setVisible(true);
    ui->Answer->setVisible(true);
    ui->AcceptAnswer->setVisible(true);
    ui->StartAgainBtn->setText("Start again");
    numberOfQuestion = 0;
    toGuess = loadedData;
    toGuess.shrink_to_fit();
    badAnswer.clear();
    badAnswer.shrink_to_fit();

    LoadNextQuestion();

    ui->All->setRange(0, toGuess.size());
    ui->Good->setRange(0, toGuess.size());
    ui->Bad->setRange(0, toGuess.size());
    ui->All->setValue(numberOfQuestion);
    ui->Good->setValue(0);
    ui->Bad->setValue(0);
}



void MainWindow::on_NextRoundBtn_clicked()
{
    LoadNextQuestion();

    ui->All->setRange(0, toGuess.size());
    ui->Good->setRange(0, toGuess.size());
    ui->Bad->setRange(0, toGuess.size());
    ui->All->setValue(numberOfQuestion);
    ui->Good->setValue(0);
    ui->Bad->setValue(0);

    ui->AcceptAnswer->setVisible(true);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{

   if(event->key() == Qt::Key_Return)
   {
       if(ui->AcceptAnswer->isVisible())
            on_AcceptAnswer_clicked();
   }
}
