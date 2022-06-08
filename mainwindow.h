#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ExitBtn_clicked();

    void on_AcceptAnswer_clicked();

    void on_StartAgainBtn_clicked();

    void on_OpenFileBtn_clicked();

    void on_NextRoundBtn_clicked();

private:

    void LoadNextQuestion();

    void AdjustProgressBarAll();

    void AdjustProgressBarGood();

    void AdjustProgressBarBad();

    bool ValidateAnswer(QString provided);

    void RoundEnd();

    void keyPressEvent(QKeyEvent*) override;

    Ui::MainWindow *ui;

    std::vector<std::pair<QString,QString>> loadedData;

    std::vector<std::pair<QString,QString>> toGuess;

    std::vector<std::pair<QString,QString>> badAnswer;

    char Separator = ';';

    int numberOfQuestion = 1;

    QString validAnswer;

    std::pair<QString,QString> currentQuestionData;

    bool badAnswerMarket = false;
};
#endif // MAINWINDOW_H
