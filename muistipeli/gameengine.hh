/* Ohjelman kirjoittaja
 * Nimi: Antti Laiho
 * Opiskelijanumero: 050270644
 * Käyttäjätunnus: xsanla
 * E-Mail: antti.laiho@tuni.fi
 * Luokka vastaa pelin toiminnallisuudesta
 * */
#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>


using GameRowType = std::vector<Card>;
using GameBoardType = std::vector<std::vector<Card>>;
#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH


class GameEngine
{
public:
    GameEngine();
    ~GameEngine();
    
    void startGame();
    
    //palauttaa pelilaudan jossa kortit muodossa vector<vector<card>>
    GameBoardType getGameBoard();
    
    // kääntää valitun kortin
    void turnCard(int cordinateX, int cordinateY);
    
    // set ja get metodeita
    void setPlayerNames(std::vector<std::string> playerNames);
    void setCardAmount(int num);
    void setPlayerAmount(int num);
    int getPlayerAmount();
    
    //siirtää vuoron seuraavalle pelaajalle
    void passTurn();

    void setSeed(int num);
    
    // tarkistaa oliko valitut kortit pari
    bool checkIfPair();

    bool checkIfGameOver();

    std::vector<Player*> getWinner();

    std::string getPlayerInTurn();

    std::vector<Player*> getPlayers();

    std::vector<std::pair<int, int>> getOpenCards() const;

    void clearOpenCards();


private:
    unsigned int stoiWithCheck(const std::string& str);

    void initWithEmpties(GameBoardType& g_board,
                         unsigned int rows, unsigned int columns);

    unsigned int nextFree(GameBoardType& g_board, unsigned int start);

    void initWithCards(GameBoardType& g_board, int seed);

    void calculateFactors(unsigned int& smaller_factor, unsigned int& bigger_factor);

    void createPlayer(std::string playerName);

    static bool comparePlayerScore(Player* p1, Player* p2);

    int numberOfCards_ = 4;

    int playerAmount_ = 2;

    int pairsLeft_ = numberOfCards_/2;

    int seed_ = 0;

    std::vector<std::string> playerNames_;

    std::vector<Player*> playerObjects_;

    Player* inTurn_;

    GameBoardType gameBoard_;
    
    std::vector<std::pair<int, int>> openCards_;
};


#endif // GAMEENGINE_HH
