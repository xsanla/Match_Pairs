/* Ohjelman kirjoittaja
 * Nimi: Antti Laiho
 * Opiskelijanumero: 050270644
 * Käyttäjätunnus: xsanla
 * E-Mail: antti.laiho@tuni.fi
 * 
 * Luokka vastaa ohjelman graafisesta toteutuksesta
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameengine.hh"
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(GameEngine* engine,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleCardClick();

    void playButtonClicked();

    void playerNameEntered();
    
    // amount changed slotit luovat spinbuttoneiden toiminnallisuudet
    void playerAmountChanged(int i);
    void cardAmountChanged(int j);
    void seedChanged(int k);
    
    // qtimer singleshotissa 
    void hideCards();

    void turnCardsBack();



private:
    Ui::MainWindow *ui;

    void pairFoundUi(std::vector<std::pair<int,int>> openCards);

    void pairNotFoundUi(std::vector<std::pair<int,int>> openCards);

    void announceTurn();

    void announceScore();

    void initCardsUi();

    void printText(QString text);

    void announceWinner();

    void disableAllCards();

    void enableAllCards();

    std::vector<std::string> takePlayerNames();

    void turnCardUi(int cordinateX, int cordinateY, QPushButton*);

    GameEngine* engine_;

    QPushButton* cardToClose1_;

    QPushButton* cardToClose2_;

    QString userInputTxt_;

    std::vector<std::vector<QPushButton*>> cardButtons_;

    std::vector<std::string> split(const std::string& s,
                                   const char delimiter, bool ignore_empty = false);

};
#endif // MAINWINDOW_HH
