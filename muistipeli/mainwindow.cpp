#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(GameEngine* engine, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    engine_ = engine;
    ui->textEdit->setText("Welcome! Enter desired game parameters and "
                          "press play.");
    connect(ui->playButton, &QPushButton::clicked, this,
            &MainWindow::playButtonClicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this,
            &MainWindow::playerNameEntered);
    ui->spinBoxPlayer->setRange(2, 5);
    ui->spinBoxCard->setRange(4, 20);
    ui->spinBoxCard->setSingleStep(2);
    connect(ui->spinBoxPlayer, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int i){playerAmountChanged(i);});
    connect(ui->spinBoxCard, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int j){cardAmountChanged(j);});
    connect(ui->spinBoxSeed, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int k){seedChanged(k);});


}

MainWindow::~MainWindow()
{
    delete ui;
    delete engine_;
}

void MainWindow::pairFoundUi(std::vector<std::pair<int,int>> openCards)
{
    cardToClose1_ = cardButtons_.at(openCards.at(0).second)
            .at(openCards.at(0).first);
    cardToClose2_= cardButtons_.at(openCards.at(1).second)
            .at(openCards.at(1).first);
    printText("Pair found!");
    disableAllCards();
    QTimer::singleShot(1500, this, SLOT(hideCards()));
}

void MainWindow::pairNotFoundUi(std::vector<std::pair<int,int>> openCards)
{
    cardToClose1_ = cardButtons_.at(openCards.at(0).second)
            .at(openCards.at(0).first);
    cardToClose2_ = cardButtons_.at(openCards.at(1).second)
            .at(openCards.at(1).first);
    disableAllCards();
    QTimer::singleShot(1500, this, SLOT(turnCardsBack()));
}

void MainWindow::announceTurn()
{
    QString playerName = QString::fromUtf8(engine_->getPlayerInTurn().c_str());
    QString announcement = "It's " + playerName + "'s turn! Pick 2 cards.";
    printText(announcement);
}

void MainWindow::announceScore()
{
    std::vector<Player*> playerObjects = engine_->getPlayers();
    printText("SCOREBOARD");
    for(Player* playerObject : playerObjects)
    {
        QString playerName = QString::fromUtf8(playerObject->
                                               get_name().c_str());
        QString playerScore = QString::number(playerObject->number_of_pairs());
        printText(playerName + "   " + playerScore);
    }
}

void MainWindow::handleCardClick()
{
    QPoint globalClickPosition = QCursor::pos();

    unsigned long gridCordinateX = 0;
    unsigned long gridCordinateY = 0;
    int localX = globalClickPosition.x() - geometry().x();
    int localY = globalClickPosition.y() - geometry().y();
    QPoint localClickPosition = QPoint(localX, localY);

    for(std::vector<QPushButton*> row : cardButtons_)
    {
        for(QPushButton* cardButton : row)
        {
            if(cardButton->geometry().contains(localClickPosition))
            {
                turnCardUi(gridCordinateX, gridCordinateY, cardButton);
                return;
            }
            ++gridCordinateX;
            if(gridCordinateX >= row.size())
            {
                if(gridCordinateY < cardButtons_.size())
                {
                    gridCordinateX = 0;
                }
                else
                {
                    return;
                }
            }
        }
        ++gridCordinateY;
        if(gridCordinateY >= cardButtons_.size())
        {
            return;
        }
    }
}

void MainWindow::playButtonClicked()
{
    printText("Please enter player names separated by"
              " commas eg(player1, player2...)");
    ui->playButton->hide();
    ui->spinBoxCard->setDisabled(true);
    ui->spinBoxSeed->setDisabled(true);
    ui->spinBoxPlayer->setDisabled(true);

}

void MainWindow::playerNameEntered()
{
    userInputTxt_ = ui->lineEdit->text();
    std::vector<std::string> playerNames = takePlayerNames();

    unsigned long playerAmount = engine_->getPlayerAmount();

    if(playerNames.size() == playerAmount)
    {
        engine_->setPlayerNames(playerNames);
        engine_->startGame();
        initCardsUi();
        ui->lineEdit->clear();
        ui->lineEdit->setDisabled(true);
        announceTurn();
        return;
    }
    printText("Make sure you entered the correct amount of players"
              "and seperated the names by a comma");
}

void MainWindow::playerAmountChanged(int amount)
{
    engine_->setPlayerAmount(amount);
}

void MainWindow::cardAmountChanged(int amount)
{
    engine_->setCardAmount(amount);
}

void MainWindow::seedChanged(int number)
{
    engine_->setSeed(number);
}

void MainWindow::hideCards()
{
    announceScore();
    announceTurn();

    cardToClose1_->hide();
    cardToClose2_->hide();
    engine_->clearOpenCards();
    enableAllCards();
    if(engine_->checkIfGameOver())
    {
        announceWinner();
    }
}

void MainWindow::turnCardsBack()
{
    cardToClose1_->setText("");
    cardToClose1_->setEnabled(true);

    cardToClose2_->setText("");
    cardToClose2_->setEnabled(true);

    cardToClose1_->setAutoFillBackground(true);
    cardToClose2_->setAutoFillBackground(true);

    QPalette palette1 = cardToClose1_->palette();
    palette1.setColor(QPalette::Button, QColor(Qt::blue));
    cardToClose1_->setPalette(palette1);
    cardToClose1_->update();

    QPalette palette2 = cardToClose2_->palette();
    palette2.setColor(QPalette::Button, QColor(Qt::blue));
    cardToClose2_->setPalette(palette2);
    cardToClose2_->update();

    engine_->clearOpenCards();
    engine_->passTurn();
    printText("Pair not found!");
    announceScore();
    announceTurn();
    enableAllCards();
}



void MainWindow::initCardsUi()
{
    QGraphicsView* view_ = new QGraphicsView(this);
    view_->setGeometry(20,30,300,300);

    int x = 1;
    int y = 1;

    GameBoardType gBoard = engine_->getGameBoard();
    for(GameRowType row : gBoard)
    {
        int rowLength = row.size();
        std::vector<QPushButton*> cardButtonRow;
        for(int i=0; i < rowLength ;++i)
        {
            QPushButton* pushButton = new QPushButton(this);
            pushButton->setGeometry((75+i*35), 30+ y*(60), 30, 50);
            pushButton->show();
            QPalette palette1 = pushButton->palette();
            palette1.setColor(QPalette::Button, QColor(Qt::blue));
            pushButton->setPalette(palette1);
            pushButton->update();
            cardButtonRow.push_back(pushButton);
            connect(pushButton, &QPushButton::clicked,
                   this, &MainWindow::handleCardClick);
            ++x;
        }
        cardButtons_.push_back(cardButtonRow);
        ++y;
    }
    view_->show();
}


void MainWindow::printText(QString text)
{
    ui->textEdit->append(text);
}

void MainWindow::announceWinner()
{
    ui->textEdit->clear();
    printText("Game Over");
    std::vector<Player*> winners = engine_->getWinner();

    QString winnerScore = QString::number(winners.at(0)->number_of_pairs());
    if(winners.size() == 1)
    {
        QString winnerName = QString::fromUtf8(winners.at(0)->get_name().c_str());
        printText("The winner is " + winnerName +" with a score of " + winnerScore);
        return;
    }
    printText("It's a tie between");
    for(Player* playerObject : winners)
    {
        QString winnerName = QString::fromUtf8(playerObject->get_name().c_str());
        printText(winnerName);
    }
    printText("With a score of " + winnerScore);
}

void MainWindow::disableAllCards()
{
    for(std::vector<QPushButton*> row : cardButtons_)
    {
        for(QPushButton* button : row)
        {
            button->setDisabled(true);
        }
    }
}

void MainWindow::enableAllCards()
{
    for(std::vector<QPushButton*> row : cardButtons_)
    {
        for(QPushButton* button : row)
        {
            if(not button->isHidden())
            {
                button->setEnabled(true);
            }
        }
    }
}


std::vector<std::string> MainWindow::takePlayerNames()
{
    std::vector<std::string> playerNames;
    std::string userInput = userInputTxt_.toUtf8().constData();
    playerNames = split(userInput, ',', true);
    return playerNames;
}

void MainWindow::turnCardUi(int gridCordinateX, int gridCordinateY,
                          QPushButton* button)
{   
    // Käännetään kortti guissa
    Card pickedCard = engine_->getGameBoard().at(gridCordinateY).at(gridCordinateX);
    const char cardLetter = pickedCard.getLetter();

    QString cardPicture = QString(QChar::fromLatin1(cardLetter));
    button->setText(cardPicture);
    QPalette palette1 = button->palette();
    palette1.setColor(QPalette::Button, QColor(Qt::green));
    button->setPalette(palette1);
    button->update();
    button->setDisabled(true);
    
    // käännettään kortti pelimoottorissa. Jos kaksi korttia käännetty
    // kutsuu moottori tarkistusta siitä ovatko kortit pari.
    engine_->turnCard(gridCordinateX, gridCordinateY);
    if(engine_->getOpenCards().size() == 2)
    {
        ui->textEdit->clear();
        if(engine_->checkIfPair())
        {
            pairFoundUi(engine_->getOpenCards());
            return;
        }
        pairNotFoundUi(engine_->getOpenCards());
    }
}

std::vector<std::string> MainWindow::split(const std::string &s, const char delimiter, bool ignore_empty)
{
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}


