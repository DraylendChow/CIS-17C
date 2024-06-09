/* 
 * File:   AbsPlayer.h
 * Author: Draylend Chow
 *
 * Created on December 12, 2023, 4:29 PM
 * Last Edited: May 4, 2024, 9:45 PM
 */

#ifndef ABSPLAYER_H
#define ABSPLAYER_H

#include <string>
#include "Card.h"
#include "Stats.h"

using namespace std;

class AbsPlayer {
    public:
        virtual string getName() const = 0;             //Returns name
        virtual unsigned int getCardAmount() = 0;       //Returns cardAmount
        virtual Card** getHand() = 0;                   //Returns players hand
        virtual Stats<int>* getRecord() = 0;            //Returns stat class
        
        virtual void setName(string) = 0;               //Sets the name
        virtual void setCardAmount(unsigned int) = 0;   //Sets the card amount
        virtual void setHand(Card **) = 0;              //Sets the hand
        
        virtual void drawCard(Card*) = 0;               //Draws a card from hand
        virtual void removeCard(int) = 0;               //Removes a card from hand
};

#endif /* ABSPLAYER_H */