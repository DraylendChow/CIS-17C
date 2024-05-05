/* 
 * File:   Card.cpp
 * Author: Draylend Chow
 *
 * Created on December 9, 2023, 10:44 AM
 */

#include "Card.h"

Card::Card() {
    value = 0;
    suit = 0;
    ptVal = 0;
    id = 0;
}

Card::Card(char value, char suit, int ptVal, int id) {
    this->value = value;
    this->suit = suit;
    this->ptVal = ptVal;
    this->id = id;
}

const Card Card::operator=(Card card) {
    value = card.getValue();
    suit = card.getSuit();
    ptVal = card.getPtVal();
    id = card.getID();
}

ostream &operator << (ostream &strm, Card &card) {
    strm << card.getValue() << card.getSuit(); //Print cards with value and suit
    return strm;
}