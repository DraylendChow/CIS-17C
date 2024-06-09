/* 
 * File:   GameMaster.h
 * Author: Draylend Chow
 *
 * Created on December 9, 2023, 1:52 PM
 * Last Edited: June 8th, 2024, 5:49 PM
 */

#include "GameMaster.h"

int GameMaster::filesCreated = 0;
array<int, 512> GameMaster::pNames;

//Function to print card
void printHand(Card *a) {
    cout << *a << ", ";
}

//Function to sort player's hand
bool handCmp(Card *a, Card *b) {
    return a->getID() > b->getID();
}

GameMaster::GameMaster() {
    theCards = NULL;
    playerSize = 0;
}

GameMaster::GameMaster(int playerSize) {
    this->playerSize = playerSize;
    int counter = 0;
    enterInfo(counter);
    
    shuffleCards();
    try {
        startGame();
    } catch(const char *msg) {
        cout << msg << endl;
    }
    
}

GameMaster::~GameMaster() {
    delete[] theCards;  //Delete dynamically created shuffled cards
}

//Recursive function
void GameMaster::enterInfo(int counter) {
    if(counter == playerSize) {
        return;
    } else {
        cout << "What is the name of Player " << (counter+1) << "? (Please enter a name 3-10 with no spaces): ";
        string name;
        cin >> name;
        players[counter+1].setName(name);
        players[counter+1].setNum(counter+1);
        players[counter+1].setCardAmount(5);
        players[counter+1].getRecord()->setUsername(name);
        
        enterInfo(++counter);
    } // End of for loop
}

void GameMaster::shuffleCards() {
    theCards = new Card*[52];  //The deck of shuffled cards
    int count = 0;
    
    //Generate random number to choose card and assign to theCards
    //Use random_shuffle algorithm
    while(count < 52) {
        theCards[count] = &stdDeck[count];
        count++;
    } //End of while loop
    random_shuffle(theCards, theCards + 52);
}

//Mutually Recursive Function (createHand and dealCards)
void GameMaster::createHand(int pCount, int cCount, int iCount) {
    //Create array of cards for each player
    if(pCount == playerSize) {
        return;
    } else {
        Card **c = new Card*[players[pCount+1].getCardAmount()];
        players[pCount+1].setHand(c);
        dealCards(pCount, cCount, iCount);
    }
}

void GameMaster::dealCards(int pCount, int cCount, int iCount) {
    //Assign cards to players
    if(cCount < (pCount+1)*5) {
        players[pCount+1].getHand()[iCount] = theCards[cCount]; //Assign card to players hand
        dealCards(pCount, ++cCount, ++iCount);
    } else {
        createHand(++pCount, cCount, 0);
    }
}

void GameMaster::shiftUp(int cardCount, int currentCard) {
    int randNum = rand() % (3 + 1) + 1;                 //Generate random number
    int index = (cardCount/2) - randNum + currentCard;  //Calculate number in middle of deck

    Card *temp = theCards[index];               //Holding old value of middle card
    theCards[index] = theCards[currentCard];    //Swapping top card to middle card
    //Shift all values up
    for(int i = index; i > currentCard; i--) {
        Card *temp2 = theCards[i-1];
        theCards[i-1] = temp;
        temp = temp2;
    } //End of for loop
}

bool GameMaster::checkDeck(Card **hand, int size, Card *topCard) {
    bool valid = false; //If player has a valid card to play
  
    //Check to see if player has any cards they can use
    for(int i = 0; i < size; i++) {
        Card *currentCard = hand[i];
        if(currentCard->getValue() == '8' || currentCard->getSuit() == topCard->getSuit() || currentCard->getValue() == topCard->getValue()) {
            valid = true;
            break;
        } //End of independent if
    } //End of for loop

    return valid;
}

bool GameMaster::checkForPass(Card *topCard) {
    bool check = false;
    for(int i = 0; i < playerSize; i++) {
        check = checkDeck(players[i+1].getHand(), players[i+1].getCardAmount(), topCard);
        if(check == true) {
            break;
        }
    }
    
    return check;
}

void GameMaster::startGame() {
    bool gameOver = false;                          //Decides when games ends
    int currentCount = playerSize * 5;              //Current index in the shuffled cards
    int cardCount = 52 - currentCount;              //How many cards are left
    int *round = new int;                           //Current round
    Card *topCard = theCards[currentCount];         //The top card on the deck
    array<string, 8> playerNames;                   //Names of all Players
    int winnerPos = 0;                              //Index of winning player
    int pointsWon = 0;                              //Total points won in the game
    vector<Card> usedCards;                         //Vector of used cards (to be shuffled)
    queue<Card> shuffledCards;                      //Queue of shuffled cards
    stack<Card> trashPile;                          //Stack of used cards
    
    //Assign names to array
    for(int i = 0; i < playerSize; i++) {
        playerNames.at(i) = players[i+1].getRecord()->getUsername();
    } //End of for loop

    *round = 1;
    int pCount = 0, cCount = 0, iCount = 0;
    createHand(pCount, cCount, iCount); //Deal cards out to players
    
    //Check if the top card is 8
    if(topCard->getValue() == '8') {
        while(topCard->getValue() == '8') {
            shiftUp(cardCount, currentCount);   //Shuffle card back into deck
            topCard = theCards[currentCount];   //New top card
        } //End of while
        cardCount--;                            //Update cards left
        currentCount++;                         //Update index on shuffled cards
    } else {
        cardCount--;    //Update cards left
        currentCount++; //Update index on shuffled cards
    } //End of dependent else
    
    //Add card values to deck
    for(int i = currentCount; i < cardCount; i++) {
        shuffledCards.push(*theCards[i]);
    } //End of for loop
  
    while(!gameOver) {
        cout << "\nRound " << *round << endl;
        //Loop through all players turn
        for(int i = 0; i < playerSize; i++) {
            bool playerDone = false; //Decide when players turn is over
            while(!playerDone) {
                playerDone = true;
                cout << "\nThis is the top card: " << *(topCard) << endl << endl;
                char input;
                //Sort Cards using sort algorithm
                sort(players[i+1].getHand(), players[i+1].getHand() + players[i+1].getCardAmount(), handCmp);
                //Check if player can make a move
                bool check = checkDeck(players[i+1].getHand(), players[i+1].getCardAmount(), topCard);
                cout << "It is " << players[i+1].getName() << "'s turn!\nHere is your current hand: [";
                //Print out players hand using for_each algorithm
                for_each(players[i+1].getHand(), players[i+1].getHand() + players[i+1].getCardAmount(), printHand);
                cout << "\b\b]" << endl;
                cout << "\nPlease choose from the following menu:" << endl;
                cout << "1. Place a Card\n2. Draw a Card\n\nChoose an option: ";
                cin >> input;
                cin.ignore(256, '\n');
                if(input == '1') {
                    //When player chooses to place a card
                    cout << "You chose to place a card." << endl;
                    if(check == true) {
                        cout << "Please choose a card from your deck or input an invalid number to go back:" << endl;
                        //Display all cards in deck as a list
                        for(int j = 0; j < players[i+1].getCardAmount(); j++) {
                            cout << (j+1) << ". " << *players[i+1].getHand()[j] << endl;
                        } //End of nested for loop
                        int choice;
                        cout << "\nChoose an option: ";
                        cin >> choice;
                        cin.ignore(256, '\n');
                        //If choice is valid
                        if(choice >= 1 && choice <= players[i+1].getCardAmount()) {
                            Card *cardChosen = players[i+1].getHand()[choice - 1];
                            if(cardChosen->getValue() == '8') {
                                //If player uses a crazy eight
                                cout << "You have placed a Crazy Eight! Please choose a suit from the following menu (1-4):\n\n1. Heart\n2. Club\n3. Diamond\n4. Spade" << endl;
                                char suit;
                                cout << "\nChoose an option: ";
                                cin >> suit;
                                cin.ignore(256, '\n');
                                //Player chooses suit
                                if(suit == '1') {
                                    topCard->setValue('0');             //Change top card to chosen suit
                                    topCard->setSuit('H');              //Change top card to chosen suit
                                    players[i+1].removeCard(choice-1);  //Remove card from hand
                                    trashPile.push(*cardChosen);        //Add card to trash pile
                                    usedCards.push_back(*cardChosen);   //Add card to used cards
                                } else if(suit == '2') {
                                    topCard->setValue('0');             //Change top card to chosen suit
                                    topCard->setSuit('C');              //Change top card to chosen suit
                                    players[i+1].removeCard(choice-1);  //Remove card from hand
                                    trashPile.push(*cardChosen);        //Add card to trash pile
                                    usedCards.push_back(*cardChosen);   //Add card to used cards
                                } else if(suit == '3') {
                                    topCard->setValue('0');             //Change top card to chosen suit
                                    topCard->setSuit('D');              //Change top card to chosen suit
                                    players[i+1].removeCard(choice-1);  //Remove card from hand
                                    trashPile.push(*cardChosen);        //Add card to trash pile
                                    usedCards.push_back(*cardChosen);   //Add card to used cards
                                } else if(suit == '4') {
                                    topCard->setValue('0');             //Change top card to chosen suit
                                    topCard->setSuit('S');              //Change top card to chosen suit
                                    players[i+1].removeCard(choice-1);  //Remove card from hand
                                    trashPile.push(*cardChosen);        //Add card to trash pile
                                    usedCards.push_back(*cardChosen);   //Add card to used cards
                                } else {
                                    //Invalid choice in suit
                                    cout << "Invalid choice. Returning back to menu." << endl;
                                    playerDone = false; //Change to repeat user turn
                                } //End of dependent else
                            } else if(cardChosen->getValue() == topCard->getValue() || cardChosen->getSuit() == topCard->getSuit()) {
                                //Regular card chosen
                                topCard = cardChosen;               //Update top card
                                players[i+1].removeCard(choice-1);  //Remove card from hand
                                trashPile.push(*cardChosen);        //Add card to trash pile
                                usedCards.push_back(*cardChosen);   //Add card to used cards
                            } else {
                                //Invalid choice
                                cout << "You cannot play that card. The suit or number value must match." << endl;
                                playerDone = false; //Change to repeat user turn
                            } //End of dependent else
                            
                            //If user has no more cards
                            if(players[i+1].getCardAmount() == 0) {
                                cout << players[i+1].getName() << " has used all of their cards. They have won the game!" << endl;
                                gameOver = true; //End game
                                
                                players[i+1].getRecord()->setWins(1); //Increment winners games win stat

                                fstream outputFile;
                                //Update all players stats
                                for(int j = 0; j < playerSize; j++) {
                                    if(i != j) {
                                        //For losing players
                                        players[j+1].getRecord()->setLosses(1);  //Update losses
                                        players[j+1].getRecord()->setWins(0);    //Update wins
                                        players[j+1].getRecord()->setRatio(0);   //Set Ratio
                                        players[j+1].getRecord()->setFstRnd(0);  //Record Round
                                    } else {
                                        //For winner
                                        winnerPos = j;
                                        pointsWon = calcPts(playerSize);
                                        players[j+1].getRecord()->setTotalPts(pointsWon); //Tally points
                                        players[j+1].getRecord()->setFstRnd(*round); //Record Round
                                        players[j+1].getRecord()->setLngRnd(*round); //Record Round
                                        players[j+1].getRecord()->setRatio(100);     //Set Ratio
                                    } //End of dependent else
                                    players[j+1].getRecord()->setTotalGms(1); //Increment games played

                                    bool found = false;
                                    string fileName;
                                    //Check through all files
                                    int ind = nameHash(players[j+1].getRecord()->getUsername()) % 512;
                                    if(pNames[ind] == 1) {
                                        for(int k = 0; k < filesCreated; k++) { 
                                            found = checkForFile(players[j+1].getRecord()->getUsername(), (k+1)); //Check if player has a file
                                            if(found == true) {
                                                //Update player file
                                                fileName = "stats" + to_string(k+1) + ".dat";
                                                try {
                                                    rewriteToFile(players[j+1].getRecord(), outputFile, fileName);
                                                } catch(const char *msg) {
                                                    cout << msg << endl;
                                                }
                                                break;
                                            } //End of dependent else
                                        } //End of nested for loop
                                    }
                                    if(found == false) {
                                        int ind = nameHash(players[j+1].getRecord()->getUsername()) % 512;
                                        pNames.at(ind) = 1; //Add name to hashTable
                                        //Create new player file
                                        filesCreated++;
                                        fileName = "stats" + to_string(filesCreated) + ".dat";
                                        writeToFile(players[j+1].getRecord(), outputFile, fileName);
                                    } //End of independent if
                                } //End of nested for loop

                                i = playerSize; //End game immediately
                            } //End of independent if
                        } else {
                            //Invalid choice from card chosen
                            cout << "Invalid choice. Returning back to menu." << endl;
                            playerDone = false; //Change to repeat user turn
                        } //End of dependent else
                    } else {
                        //When player has no valid cards
                        cout << "You have no cards that can be placed. Please draw a card instead." << endl;
                        playerDone = false; //Change to repeat user turn
                    } //End of dependent else
                } else if(input == '2') {
                    //When player chooses to draw a card
                    if(cardCount > 0) {
                        //If there are still cards to draw
                        if(check == false) {
                            //If player is forced to draw a card
                            int counter = 0;
                            bool keepDraw = false;
                            while(!keepDraw) {
                                cout << "You have drawn a card: " << shuffledCards.front() << endl;
                                players[i+1].drawCard(&shuffledCards.front()); //Draw card
                                counter++;
                                keepDraw = checkDeck(players[i+1].getHand(), players[i+1].getCardAmount(), topCard); //Check if player still needs to draw cards
                                cardCount--;    //Amount of cards left
                                currentCount++; //Current index in shuffled cards
                                shuffledCards.pop(); //Remove card by popping it off queue
                                if(cardCount == 0) {
                                    //If there are no more cards left to draw
                                    break;
                                } //End of independent if
                            } //End of nested while loop
                            cout << "You drew a total of " << counter << " cards." << endl;
                            playerDone = false; //Change to repeat user turn
                        } else {
                            //If user chose to draw a card
                            cout << "You have drawn a card: " << shuffledCards.front() << endl;
                            players[i+1].drawCard(&shuffledCards.front());
                            cardCount--;    //Amount of cards left
                            currentCount++; //Current index in shuffled cards
                            shuffledCards.pop(); //Remove card by popping it off queue
                            playerDone = false; //Change to repeat user turn
                        } //End of dependent else
                    } else {
                        //If there are no more cards left to draw
                        cout << "There are no more cards to draw." << endl;
                        if(check == false) {
                            //If user is unable to do anything
                            cout << "You are unable to make anymore moves. Your turn has been passed." << endl;
                            if(checkForPass(topCard) == false) {
                                throw "No more moves can be played. Game has ended with no Winner.";
                            }
                        } else {
                            //If user can still place a card down
                            playerDone = false; //Change to repeat user turn
                        }
                    } //End of dependent else
                } else {
                    //When player makes an invalid choice
                    cout << "\nInvalid input! Try again." << endl;
                    playerDone = false; //Change to repeat user turn
                } //End of dependent else
            } //End of nested while loop
        }
        *round += 1; //Increase round after all turns have passed
    }
    *round -= 1; //Decrease round cause it ran extra time
    
    //Display results of game
    cout << "\nHere are the results of the game:" << endl;
    cout << "The game lasted " << *round << " rounds!" << endl;
    cout << playerNames.at(winnerPos) << " accumulated a total of " << pointsWon << " points this round!" << endl << endl;
    cout << "Thank you ";
    //Print names of players
    for(auto it = playerNames.begin(); it != playerNames.end(); it++) {
        if((it+1)->empty() == true || it+1 == playerNames.end()) {
            cout << "and " << *it << " for playing!" << endl;
            break;
        } else {
            cout << *it << ", ";
        } //End of dependent else
    } //End of for loop

    cout << "These were all the cards used in this game (order in which they were played): " << endl;
    while(!trashPile.empty()) {
        cout << trashPile.top() << ", ";
        trashPile.pop(); //Empty out stack
    }
    cout << "\b\b" << endl;
    
    quickSort(usedCards, 0, usedCards.size()-1);
    cout << "\nThese were all the cards used in this game (sorted): " << endl;
    for(int i = 0; i < usedCards.size(); i++) {
        cout << usedCards[i] << ", ";
    } //End of for loop
    cout << "\b\b" << endl;

    delete round; //Delete dynamically created round 
}

int GameMaster::calcPts(int) {
    //Using list container with bidirectional iterator
    list<char> values = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K'};  //Array for point values
    list<char>::iterator it;    //Iterator
    int total = 0;              //Total points
    
    set<int, greater<int>> playerPts; //Set of player points
    
    //Cycle through all players cards
    for(int i = 0; i < playerSize; i++) {
        int tempPts = 0;
        for(int j = 0; j < players[i+1].getCardAmount(); j++) {
            for(it = values.begin(); it != values.end(); it++) {
                Card *card = players[i+1].getHand()[j];
                if(card->getValue() == *it) {
                    tempPts += card->getPtVal();
                    total += card->getPtVal();
                    break;
                } //End of independent if
            } //End of nested for loop
        } //End of nested for loop
        playerPts.insert(tempPts);
    } //End of for loop
    
    int count = playerSize;
    set<int, greater<int>>::iterator itr; 
    for (itr = playerPts.begin(); itr != playerPts.end(); itr++) {
        cout << "Player " << count << " had " << *itr << " points." << endl;
        count--;
    }
    
    return total; //Return total points
}

int GameMaster::partition(vector<Card> &cards, int beg, int end) {
    int pivot = cards[end].getID();
    int i = (beg-1);
    
    for(int j= beg; j <= end-1; j++)
    {
        if(cards[j].getID() < pivot)
        {
            i++;
            swap(cards[i], cards[j]);
        }
    }
    
    swap(cards[i+1], cards[end]);
    return (i+1);
}

void GameMaster::quickSort(vector<Card> &cards, int beg, int end) {
    if(beg < end)
    {
        int ind = partition(cards, beg, end);
        quickSort(cards, beg, ind-1);
        quickSort(cards, ind+1, end);
    }
}

int GameMaster::nameHash(string name) {
    int seed = 21;
    int hash = 0;
    
    for(int i = 0; i < name.length(); i++) {
        hash = (hash * seed) + name[i];
    }
    
    return hash;
}

bool GameMaster::checkForFile(string name, int index) {
    bool found = false;                                     //If file for person exists
    fstream file;                                           //File object
    string fileName = "stats" + to_string(index) + ".dat";  //The name of file being accessed
    string nameFile;                                        //Name from file
    
    //Check if name exists within all files
    file.open(fileName, ios::in);   //Open file for input
    if(file.is_open()) {
        //If file exists
        for(int i = 8; i < 30; i++) {
            char ch;
            file.seekg(i, ios::beg); //Read in name from file
            file.get(ch);
            if(isalpha(ch) != 0) {
                //If name is still being read
                nameFile += ch;
            } else {
                //If name ends
                break;
            } //End of dependent else
        } //End of for loop
    } //End of independent if
    
    if(name == nameFile) {
        found = true;
    } //End of independent if
    file.close();
    
    return found; //Return outcome
}

void GameMaster::writeToFile(Stats<int> *info, fstream &outputFile, string fileName) {
    outputFile.open(fileName, ios::out | ios::binary);  //Open file in binary
    outputFile.write((char*)info, sizeof(Stats<int>));  //Write to file in binary
    
    outputFile.close(); //Close file
}

void GameMaster::rewriteToFile(Stats<int> *info, fstream &outputFile, string fileName) {
    Stats<int> *temp = new Stats<int>();
    outputFile.open(fileName, ios::in | ios::out | ios::binary);    //Open file in binary
    //Retrieve data from file
    if(outputFile.is_open()) {
        //If file exists
        outputFile.read((char*)temp, sizeof(Stats<int>)); //Read from file in binary
        
        info->setWins(info->getWins() + temp->getWins());               //Update wins
        info->setLosses(info->getLosses() + temp->getLosses());         //Update losses
        info->setTotalGms(info->getTotalGms() + temp->getTotalGms());   //Update total games played
        info->setTotalPts(info->getTotalPts() + temp->getTotalPts());   //Update total points

        //Update data in file
        if(info->getFstRnd() > temp->getFstRnd() && temp->getFstRnd() != 0 || info->getFstRnd() == 0) {
            //If round is faster
            info->setFstRnd(temp->getFstRnd());
        } //End of independent if
        if(info->getLngRnd() < temp->getLngRnd()) {
            //If round is longer
            info->setLngRnd(temp->getLngRnd());
        } //End of independent if
        float num = info->getWins();
        info->setRatio((num / info->getTotalGms()) * 100); //Calculate new win percent

        outputFile.seekp(0, ios::beg);
        outputFile.write((char*)info, sizeof(Stats<int>)); //Write to file in binary
        
        delete temp;
    } else {
        //If file doesn't exist
        throw "The file does not exist.";
    } //End of dependent else
    
    outputFile.close();
}

Stats<int>* GameMaster::readFromFile(fstream &binFile, string fileName) {
    Stats<int> *person = new Stats<int>();          //Player's stats
    binFile.open(fileName, ios::in | ios::binary);  //Open file in binary
    //Retrieve data from file
    if(binFile.is_open()) {
        //If file exists
        binFile.read((char*)person, sizeof(Stats<int>)); //Read from file in binary
        binFile.close(); //Close file
    } else {
        //If file doesn't exist
        throw "The file does not exist.";
    } //End of dependent else
    
    binFile.open(fileName, ios::in);
    string name = "";
    for(int i = 8; i < 30; i++) {
        char ch;
        binFile.seekg(i, ios::beg); //Read in name from file
        binFile.get(ch);
        if(isalpha(ch) != 0) {
            //If name is still being read
            name += ch;
        } else {
            //If name ends
            break;
        } //End of dependent else
    } //End of for loop
    person->setUsername(name);
    binFile.close();
    
    return person; //Return player's stats
}

void GameMaster::generateTextFile() {
    fstream txtFile;    //File object
    string fileName;    //File name
    Stats<int> *person; //Player's stats
    
    txtFile.open("allStats.txt", ios::out); //Open file
    //Create text file with all player stats
    if(filesCreated == 0) {
        //If there are no existing files
        txtFile << "There is nothing to display yet.";
        txtFile.close(); //Close file
        throw "There is no data to display yet.";
    } else {
        //If there are files
        for(int i = 0; i < filesCreated; i++) {
            //Display stats
            fileName = "stats" + to_string(i+1) + ".dat";
            fstream tempFile;
            try {
                person = readFromFile(tempFile, fileName); //Get data from file
            } catch(const char *msg) {
                cout << msg << endl;
            }
            txtFile << person->getUsername() << "\'s Stats" << endl << endl;
            txtFile << "Wins:\t\t\t" << person->getWins() << endl;
            txtFile << "Losses:\t\t\t" << person->getLosses() << endl;
            txtFile << fixed << setprecision(2); //Format win percent
            txtFile << "Win Percentage:\t\t" << person->getRatio() << "%" << endl;
            txtFile << setprecision(0);
            txtFile << "Total Games Played:\t" << person->getTotalGms() << endl;
            txtFile << "Total Points Earned:\t" << person->getTotalPts() << endl;
            txtFile << "Fastest Round to Win:\t" << person->getFstRnd() << endl;
            txtFile << "Longest Round to Win:\t" << person->getLngRnd() << endl;
            txtFile << endl << "-----------------------------------------" << endl << endl;
            
            delete person; //Delete dynamically created class
        } //End of for loop
        txtFile.close(); //Close file
    } //End of dependent else
}