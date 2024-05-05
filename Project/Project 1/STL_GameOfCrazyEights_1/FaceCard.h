/* 
 * File:   FaceCard.h
 * Author: Draylend Chow
 *
 * Created on December 12, 2023, 3:45 PM
 */

#ifndef FACECARD_H
#define FACECARD_H

#include "Card.h"

//Inheritance of Card
class FaceCard : public Card{
    public:
        FaceCard() : Card(){ //Constructor
            ptVal = 0;
        };
        FaceCard(char value, char suit, int num) : Card(value, suit, num) { //Overloaded Constructor
            ptVal = 10;
        };
};

#endif /* FACECARD_H */