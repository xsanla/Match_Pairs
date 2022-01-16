/* Luokka: Card
 * ------------
 * Kuvaa yhtä korttia muistipelissä.
 *
 * Ohjelman kirjoittaja
 * Nimi: Antti Laiho
 * Opiskelijanumero: 050270644
 * Käyttäjätunnus: xsanla
 * E-Mail: antti.laiho@tuni.fi
 */


#ifndef CARD_HH
#define CARD_HH

enum Visibility_type {OPEN, HIDDEN, EMPTY};
const char HIDDEN_CHAR = '#';
const char EMPTY_CHAR = '.';

class Card
{
public:
    // Oletusrakentaja: luo tyhjän kortin.
    //
    // Default constructor: creates an empty card.
    Card();

    // Rakentaja: luo piilotetun kortin, jonka kirjaimeksi tulee annettu merkki.
    //
    // Constructor: creates a hidden card with the given character.
    Card(const char c);

    // Ilmeiset setter- ja getter-metodit.
    //
    // Obvious setter and getter methods.
    void setLetter(const char c);
    void setVisibility(const Visibility_type visibility);
    char getLetter() const;
    Visibility_type getVisibility() const;

    // Kääntää kortin: vaihtaa näkyvyyden avoimesta piilotettuun ja päin vastoin.
    //
    // Turns a card: changes the visibility from open to hidden and vice versa.
    void turn();


    // Poistaa kortin pelilaudalta: muuttaa näkyvyyden tyhjäksi.
    //
    // Removes a card from the game board: changes the visibility to empty.
    void removeFromGameBoard();

private:
    char letter_;
    Visibility_type visibility_;
};

#endif // CARD_HH
