/* 
 * File:   GameMaster.h
 * Author: Draylend Chow
 *
 * Created on December 9, 2023, 12:58 PM
 */

#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <ctime>
#include <array>
#include <list>
#include <set>
#include <map>
#include <algorithm>

#include "Player.h"
#include "NormalCard.h"
#include "FaceCard.h"
#include "SpecialCard.h"

using namespace std;

class GameMaster {
    private:
        //Standard Playing Deck //Note: I swapped 10's for X's for consistency in size
        Card stdDeck[52] = {NormalCard('2', 'H', 0, 1), NormalCard('2', 'C', 0, 14), NormalCard('2', 'D', 0, 27), NormalCard('2', 'S', 0, 40),
                            NormalCard('3', 'H', 0, 2), NormalCard('3', 'C', 0, 15), NormalCard('3', 'D', 0, 28), NormalCard('3', 'S', 0, 41),
                            NormalCard('4', 'H', 0, 3), NormalCard('4', 'C', 0, 16), NormalCard('4', 'D', 0, 29), NormalCard('4', 'S', 0, 42),
                            NormalCard('5', 'H', 0, 4), NormalCard('5', 'C', 0, 17), NormalCard('5', 'D', 0, 30), NormalCard('5', 'S', 0, 43),
                            NormalCard('6', 'H', 0, 5), NormalCard('6', 'C', 0, 18), NormalCard('6', 'D', 0, 31), NormalCard('6', 'S', 0, 44),
                            NormalCard('7', 'H', 0, 6), NormalCard('7', 'C', 0, 19), NormalCard('7', 'D', 0, 32), NormalCard('7', 'S', 0, 45),
                            SpecialCard('8', 'H', 0, 7), SpecialCard('8', 'C', 0, 20), SpecialCard('8', 'D', 0, 33), SpecialCard('8', 'S', 0, 46),
                            NormalCard('9', 'H', 0, 8), NormalCard('9', 'C', 0, 21), NormalCard('9', 'D', 0, 34), NormalCard('9', 'S', 0, 47),
                            FaceCard('X', 'H', 0, 9), FaceCard('X', 'C', 0, 22), FaceCard('X', 'D', 0, 35), FaceCard('X', 'S', 0, 48),
                            FaceCard('J', 'H', 0, 10), FaceCard('J', 'C', 0, 23), FaceCard('J', 'D', 0, 36), FaceCard('J', 'S', 0, 49),
                            FaceCard('Q', 'H', 0, 11), FaceCard('Q', 'C', 0, 24), FaceCard('Q', 'D', 0, 37), FaceCard('Q', 'S', 0, 50),
                            FaceCard('K', 'H', 0, 12), FaceCard('K', 'C', 0, 25), FaceCard('K', 'D', 0, 38), FaceCard('K', 'S', 0, 51),
                            NormalCard('A', 'H', 0, 13), NormalCard('A', 'C', 0, 26), NormalCard('A', 'D', 0, 39), NormalCard('A', 'S', 0, 52)};
        
        map<int, Player> players;   //Set of all the Players in the game
        Card **theCards;            //The shuffled cards used
        int playerSize;             //The amount of players playing
        static int filesCreated;    //Amount of files created (1 for each new player)
        
        void shuffleCards();                    //Shuffle Cards
        void dealCards();                       //Deal 5 Cards to All Players
        void shiftUp(int, int);                 //Add top card into middle and shifts up
        bool checkDeck(Card **, int, Card *);   //Check to see if Player can make a move
        bool checkForPass(Card *);              //Check to see if no more valid moves can be played
        void startGame();                       //Start the game
        int calcPts(int);                       //Calculate all the points earned
    public:
        GameMaster();       //Constructor
        GameMaster(int);    //Overloaded Constructor
        ~GameMaster();      //Destructor
        
        void setPlayerSize(int amount) {playerSize = amount;}                   //Sets the playerSize
        int getFilesCreated() {return filesCreated;}                            //Returns amount of filesCreated
                
        void writeToFile(Stats<int> *, fstream &, string);      //Creates a new file
        void rewriteToFile(Stats<int> *, fstream &, string);    //Updates an existing file
        Stats<int>* readFromFile(fstream &, string);            //Gathers data from file
        bool checkForFile(string, int);                         //Checks to see if file for person exists
        void generateTextFile();                                //Generate a text file to view all statistics
};

#endif /* GAMEMASTER_H */