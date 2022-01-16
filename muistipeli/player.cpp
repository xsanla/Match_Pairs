#include "player.hh"
#include <iostream>
Player::Player(const std::string& name):
    name_(name)
{

}

std::string Player::get_name() const
{
    return name_;
}

unsigned int Player::number_of_pairs() const
{
    if(collectedCards_.empty())
    {
        return 0;
    }
    int score = collectedCards_.size();
    return score;
}

void Player::add_card(Card& card)
{
    collectedCards_.push_back(card);
    card.removeFromGameBoard();
}
void Player::print() const
{
    std::cout << "*** " << name_ << " has " << number_of_pairs() << " pair(s)."
    << std::endl;
}
