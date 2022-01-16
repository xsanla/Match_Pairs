#include "gameengine.hh"
#include<algorithm>

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{
    //REMEMBER TO DEALLOCATE PLAYERS IN PLAYER OBJECTS LATER
    for(Player* playerObject : playerObjects_)
    {
        delete playerObject;
    }
}

void GameEngine::startGame()
{
    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    calculateFactors(factor1, factor2);
    initWithEmpties(gameBoard_ , factor1, factor2);
    initWithCards(gameBoard_, seed_);
    for(std::string name : playerNames_)
    {
        createPlayer(name);
    }
    inTurn_ = playerObjects_.at(0);
}

GameBoardType GameEngine::getGameBoard()
{
    return gameBoard_;
}

void GameEngine::turnCard(int cordinateX, int cordinateY)
{
    Card& pickedCard = gameBoard_.at(cordinateY).at(cordinateX);
    std::pair<int, int> openedCard (cordinateX, cordinateY);
    pickedCard.turn();
    openCards_.push_back(openedCard);
}

void GameEngine::setPlayerNames(std::vector<std::string> playerNames)
{
    playerNames_ = playerNames;
}

void GameEngine::setCardAmount(int num)
{
    numberOfCards_ =num;
    pairsLeft_ = numberOfCards_ / 2;
}

void GameEngine::setPlayerAmount(int num)
{
    playerAmount_ = num;
}

int GameEngine::getPlayerAmount()
{
    return playerAmount_;
}

void GameEngine::passTurn()
{
    if(inTurn_->get_name() == playerObjects_.back()->get_name())
    {
        inTurn_ = playerObjects_.at(0);
        return;
    }
    int playerposition = 0;
    for(Player* playerObject : playerObjects_)
    {
        if(playerObject->get_name() == inTurn_->get_name())
        {
            break;
        }
        ++playerposition;
    }
    inTurn_ = playerObjects_.at(playerposition+1);
}

void GameEngine::setSeed(int num)
{
    seed_ = num;
}

unsigned int GameEngine::stoiWithCheck(const std::string& str)
{
    bool isNumeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            isNumeric = false;
            break;
        }
    }
    if(isNumeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Täyttää pelilaudan (kooltaan rows * columns) tyhjillä korteilla.
void GameEngine::initWithEmpties(GameBoardType& gBoard, unsigned int rows,
                                 unsigned int columns)
{
    gBoard.clear();
    GameRowType row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        gBoard.push_back(row);
    }
}


// Etsii seuraavan tyhjän kohdan pelilaudalta (g_board) aloittamalla
// annetusta kohdasta start ja jatkamalla tarvittaessa alusta.
// (Kutsutaan vain funktiosta init_with_cards.)
//
// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int GameEngine::nextFree(GameBoardType& gBoard, unsigned int start)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = gBoard.size();
    unsigned int columns = gBoard.at(0).size();

    // Aloitetaan annetusta arvosta
    //
    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(gBoard.at(i / columns).at(i % columns).getVisibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }
    // Jatketaan alusta
    //
    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(gBoard.at(i / columns).at(i % columns).getVisibility() == EMPTY)
        {
            return i;
        }
    }
    // Tänne ei pitäisi koskaan päätyä
    //
    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
//
// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void GameEngine::initWithCards(GameBoardType& gBoard, int seed)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = gBoard.size();
    unsigned int columns = gBoard.at(0).size();

    // Arvotaan täytettävä sijainti
    //
    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);
    // Hylätään ensimmäinen satunnaisluku (joka on aina jakauman alaraja)
    //
    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siitä seuraava tyhjä paikka.
    // (Seuraava tyhjä paikka haetaan kierteisesti funktion next_free avulla.)
    //
    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisätään kaksi samaa korttia (parit) pelilaudalle
        //
        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = nextFree(gBoard, cell);
            gBoard.at(cell / columns).at(cell % columns).setLetter(c);
            gBoard.at(cell / columns).at(cell % columns).setVisibility(HIDDEN);
        }
    }
}

//Luo Player olion ja tallentaa sen vektoriin
//player_objects myöhempää käyttöä varten.
void GameEngine::createPlayer(std::string playerName)
{
    Player* newPlayer = new Player(playerName);
    playerObjects_.push_back(newPlayer);
}

bool GameEngine::comparePlayerScore(Player* p1, Player* p2)
{
    return (p1->number_of_pairs() > p2->number_of_pairs());
}

bool GameEngine::checkIfPair()
{
    if(openCards_.size() != 2)
    {
        return false;
    }
    std::pair<int, int> cardOneCordinates = openCards_.at(0);
    std::pair<int, int> cardTwoCordinates = openCards_.at(1);
    
    Card& cardOne = gameBoard_.at(cardOneCordinates.second)
            .at(cardOneCordinates.first);
    Card& cardTwo = gameBoard_.at(cardTwoCordinates.second)
            .at(cardTwoCordinates.first);
    
    if(cardOne.getLetter() == cardTwo.getLetter())
    {
        inTurn_->add_card(cardOne);
        cardTwo.removeFromGameBoard();
        --pairsLeft_;
        return true;
    }
    cardOne.turn();
    cardTwo.turn();
    return false;
}

bool GameEngine::checkIfGameOver()
{
    if(pairsLeft_ == 0)
    {
        return true;
    }
    return false;
}

std::vector<Player*> GameEngine::getWinner()
{
    std::sort(playerObjects_.begin(), playerObjects_.end(),comparePlayerScore);
    std::vector<Player*> winners;
    unsigned long winningScore = playerObjects_.at(0)->number_of_pairs();
    for(Player* playerObject : playerObjects_)
    {
        if(playerObject->number_of_pairs() == winningScore)
        {
            winners.push_back(playerObject);
        }
    }
    return winners;
}

std::string GameEngine::getPlayerInTurn()
{
    return inTurn_->get_name();
}

std::vector<Player*> GameEngine::getPlayers()
{
    return playerObjects_;
}

std::vector<std::pair<int, int> > GameEngine::getOpenCards() const
{
    return openCards_;
}

void GameEngine::clearOpenCards()
{
    openCards_.clear();
}


// Kysyy käyttäjältä tulon ja sellaiset tulon tekijät, jotka ovat
// mahdollisimman lähellä toisiaan.
//
// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void GameEngine::calculateFactors(unsigned int& smallerFactor, unsigned int& biggerFactor)
{
    unsigned int product = numberOfCards_;


    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smallerFactor = i;
        }
    }
    biggerFactor = product / smallerFactor;
}
