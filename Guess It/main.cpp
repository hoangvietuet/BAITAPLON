#include <iostream>
#include <cstdlib>
using namespace std;
int generateRandomNumber(){
int v1 = rand() % 100;
return v1;
}
int getPlayerGuess(){
int n;
cin >> n;
return n;
}
void printAnswer(int guess, int secretNumber, int score){
if (guess > secretNumber) cout << "your number is big too\n";
else if (guess < secretNumber) cout << "your number is small too\n";
else{
     if (score > 100){
        cout << "Game Over!";
     }
     else{
        cout << "Correct!" << endl;
        cout << "your score is: " << 100 - score << endl;
     }
}
}
int main()
{
    char kitu ;  // xem ng choi muon choi nua khong
    int highscore = 0; // diem so cao nhat
do{
    int score = 1; // diem cua van choi
    cout << "Please enter a number from 1 to 100:\n";
    int secretNumber = generateRandomNumber();
    int guess;
   do {
    guess = getPlayerGuess();
    printAnswer(guess, secretNumber, score);
    score++;
      }while (guess != secretNumber);
     if (100 - score > highscore) highscore = 100 - score;
    cout << "do you want to play again ";
    cout << "If yes please enter 'y':";
     cin >> kitu;
}while(kitu == 'y');
  cout << "Your highscore : " << highscore << endl;;
  cout << "Thanks";
}
