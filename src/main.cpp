//   ____  _            _       _            _     ___  ____   //
//  | __ )| | __ _  ___| | __  | | __ _  ___| | __/ _ \|  _ \  //
//  |  _ \| |/ _` |/ __| |/ /  | |/ _` |/ __| |/ / | | | |_) | //
//  | |_) | | (_| | (__|   < |_| | (_| | (__|   <| |_| |  __/  //
//  |____/|_|\__,_|\___|_|\_\___/ \__,_|\___|_|\_\\___/|_|     //
//                      Made by NULLCT                         //

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std;

#define DEBUG

//generate random num
std::mt19937 mt{std::random_device{}()};
std::uniform_int_distribution<int> dist(1, 13);

void putArrayNums(const vector<int> &_list, const string _str) { // put array
  cout << _str << ": ";
  for (int i : _list)
    cout << i << " ";
  cout << "\n";
}

void arrayFillRandom(vector<int> &_list) { // fill array random num (1 ~ 13)
  vector<int> cnt(13, 0);
  for (int i = 0; i < _list.size();) {
    int num = dist(mt);
    if (cnt[num - 1] < 4) {
      cnt[num - 1]++;
      _list[i] = num;
      i++;
    }
  }
}

void fixArray(vector<int> &_list) {
  for (int &i : _list)
    if (10 <= i)
      i = 10;
}

namespace gamestatus {
enum GAMESTATUS { // game results
  WIN = 0,
  LOSE = 1,
  DRAW = 2,
  DOUBLEDOWNTOWIN = 3,
  DOUBLEDOWNTOLOSE = 4,
  DOUBLEDOWNTODRAW = 5,
  SALENDER = 6,
  BLACKJACK = 7,
  ERROR = -1
};
}
namespace cardstatus {
enum CARDSTATUS { // card tyoes
  HIT = 0,
  STAND = 1,
  DOUBLEDOWN = 2,
  SALENDER = 3
};
}
constexpr int chart_first[10][10] = { //first chart
  {0,0,0,0,0,0,0,0,0,0},
  {0,2,2,2,2,0,0,0,0,0},
  {2,2,2,2,2,2,2,2,0,0},
  {2,2,2,2,2,2,2,2,2,0},
  {0,0,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,3,0},
  {1,1,1,1,1,0,0,3,3,3},
  {1,1,1,1,1,1,1,1,1,1}
};
constexpr int chart_plusa[8][10] = { //if include ace
  {0,0,0,2,2,0,0,0,0,0},
  {0,0,0,2,2,0,0,0,0,0},
  {0,0,0,2,2,0,0,0,0,0},
  {0,0,2,2,2,0,0,0,0,0},
  {0,0,2,2,2,0,0,0,0,0},
  {0,2,2,2,2,0,0,0,0,0},
  {1,2,2,2,2,1,1,0,0,0},
  {1,1,1,1,1,1,1,1,1,1}
};
constexpr int chart_second[7][10] = { // in second time
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1}
};
constexpr int chart_seconda[3][10] = { // in secon if include ace
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,0,0,0},
  {1,1,1,1,1,1,1,1,1,1}
};

int getChooseCardFromFirst(int _me_sum, int _dealer_opened_card) { // Choose card from chart_first
  // EDUCATION to me_sum
  if (_me_sum <= 8)
    _me_sum = 8;
  if (17 <= _me_sum)
    _me_sum = 17;

  // Too
  if (10 <= _dealer_opened_card)
    _dealer_opened_card = 10;
  if (_dealer_opened_card == 1)
    _dealer_opened_card = 11;

  return chart_first[_me_sum - 8][_dealer_opened_card - 2];
}
int getChooseCardFromFirstAce(int _me_sum, int _dealer_opened_card) { // Choose card from chart_plusa
  // EDUCATION to me_sum
  if (_me_sum <= 2)
    _me_sum = 2;
  if (9 <= _me_sum)
    _me_sum = 9;

  // Too
  if (10 <= _dealer_opened_card)
    _dealer_opened_card = 10;
  if (_dealer_opened_card == 1)
    _dealer_opened_card = 11;

  return chart_first[_me_sum - 2][_dealer_opened_card - 2];
}
int getChooseCardFromSecond(int _me_sum, int _dealer_opened_card) { // Choose card from chart_second
  // EDUCATION to me_sum
  if (_me_sum <= 11)
    _me_sum = 11;
  if (17 <= _me_sum)
    _me_sum = 17;

  // Too
  if (_dealer_opened_card == 1)
    _dealer_opened_card = 11;
  if (10 <= _dealer_opened_card)
    _dealer_opened_card = 10;

  return chart_first[_me_sum - 11][_dealer_opened_card - 2];
}
int getChooseCardFromSecondAce(int _me_sum, int _dealer_opened_card) { // Choose card from chart_second
  // EDUCATION to me_sum
  if (_me_sum <= 7)
    _me_sum = 7;
  if (9 <= _me_sum)
    _me_sum = 9;

  // Too
  if (10 <= _dealer_opened_card)
    _dealer_opened_card = 10;
  if (_dealer_opened_card == 1)
    _dealer_opened_card = 11;

  return chart_first[_me_sum - 7][_dealer_opened_card - 2];
}

constexpr int isWinLoseDraw(const int &me_sum, const int &dealer_sum) {
  if (21 < me_sum and 21 < dealer_sum) {
    return gamestatus::DRAW;
  } else {
    if (21 < me_sum)
      return gamestatus::LOSE;
    if (21 < dealer_sum)
      return gamestatus::WIN;

    if (me_sum == dealer_sum) {
      return gamestatus::DRAW;
    } else {
      if (me_sum < dealer_sum) {
        return gamestatus::LOSE;
      } else {
        return gamestatus::WIN;
      }
    }
  }
  return gamestatus::ERROR;
}

int game(const vector<int> &_list) { // code of kernel (0.0000044s)(0.0044ms)
  int cardpos = 0;                   // point of _list[]
  int me_sum = 0, dealer_sum = 0;    // both player's sum
  bool atr = false;                  // Ace trigger
  bool firsttr = true;               // First trigger
  int meshouldcard = 0;              // should to do
  vector<int> me, dealer;            // Both hand

  // first hit time x2
  me.push_back(_list[cardpos]);cardpos++;
  me.push_back(_list[cardpos]);cardpos++;
  me_sum = me[0] + me[1];
  if(me[0] == 1 or me[1] == 1) atr=true;

  dealer.push_back(_list[cardpos]);cardpos++;
  dealer.push_back(_list[cardpos]);cardpos++;
  dealer_sum = dealer[0] + dealer[1];

  if ((me[0] == 1 or me[1] == 1) and me_sum == 11) // BlackJack!!
    return gamestatus::BLACKJACK;

  if (dealer_sum == 21) // BlackJack!!
    return gamestatus::LOSE;

  while (true) { // Open card is dealer[0]
    if (_list.size() - 1 < cardpos)
      return gamestatus::ERROR;

    if (firsttr) {
      firsttr = false;
      if (atr)
        meshouldcard = getChooseCardFromFirstAce(me_sum, dealer[0]);
      else
        meshouldcard = getChooseCardFromFirst(me_sum, dealer[0]);
    } else {
      if (atr)
        meshouldcard = getChooseCardFromSecondAce(me_sum, dealer[0]);
      else
        meshouldcard = getChooseCardFromSecond(me_sum, dealer[0]);
    }

    switch (meshouldcard) {
    case cardstatus::HIT:
      me.push_back(_list[cardpos]);
      cardpos++;
      me_sum += me[me.size() - 1];
      if (me[me.size() - 1] == 1)
        atr = true;

      if (21 <= me_sum)
        goto END;
      break;

    case cardstatus::STAND:
      goto END;
      break;

    case cardstatus::DOUBLEDOWN: // idk TODO:
      me.push_back(_list[cardpos]);
      cardpos++;
      me_sum += me[me.size() - 1];

      switch (isWinLoseDraw(me_sum, dealer_sum)) {
      case gamestatus::WIN:
        return gamestatus::DOUBLEDOWNTOWIN;
      case gamestatus::LOSE:
        return gamestatus::DOUBLEDOWNTOLOSE;
      case gamestatus::DRAW:
        return gamestatus::DOUBLEDOWNTODRAW;
      default:
        return gamestatus::ERROR;
      }
      return gamestatus::ERROR;

    case cardstatus::SALENDER:; // idk TODO:
      return gamestatus::SALENDER;

    default:
      return gamestatus::ERROR;
    }
  }
END: // end of game

  // dealer take card while ($sum < 17)
  while(dealer_sum < 17){
    dealer.push_back(_list[cardpos]);cardpos++;
    dealer_sum += dealer[dealer.size() - 1];
  }

  if(atr and me_sum+10 <= 21)
    me_sum+=10;

  return isWinLoseDraw(me_sum, dealer_sum);
}

void putLogo(){
  const string logo[] = {
    #include "logo.txt"
  };
  for(const auto &i:logo){
    cout<<i<<"\n";
  }
}

vector<int> result(8,0);
mutex mtx_result[9];
mutex mtx_persent;

void threadGaming(int _n,int _threadnum) {
  vector<int> list(15,0); // Initialcards
  int persent=0;
  for(int i=0;i<_n;i++){
    if(persent != int(double(i)/_n*100)){
      persent = int(double(i)/_n*100);
      mtx_persent.lock();
      cout<<_threadnum<<":"<<persent<<"%"<<endl;
      mtx_persent.unlock();
    }
    arrayFillRandom(list);
    fixArray(list);

    switch (game(list)) {
    case gamestatus::WIN:
      mtx_result[0].lock();
      result[0]++;
      mtx_result[0].unlock();
      break;

    case gamestatus::LOSE:
      mtx_result[1].lock();
      result[1]++;
      mtx_result[1].unlock();
      break;

    case gamestatus::DRAW:
      mtx_result[2].lock();
      result[2]++;
      mtx_result[2].unlock();
      break;

    case gamestatus::DOUBLEDOWNTOWIN:
      mtx_result[3].lock();
      result[3]++;
      mtx_result[3].unlock();
      break;

    case gamestatus::DOUBLEDOWNTOLOSE:
      mtx_result[4].lock();
      result[4]++;
      mtx_result[4].unlock();
      break;

    case gamestatus::DOUBLEDOWNTODRAW:
      mtx_result[5].lock();
      result[5]++;
      mtx_result[5].unlock();
      break;

    case gamestatus::SALENDER:
      mtx_result[6].lock();
      result[6]++;
      mtx_result[6].unlock();
      break;

    case gamestatus::BLACKJACK:
      mtx_result[7].lock();
      result[7]++;
      mtx_result[7].unlock();
      break;

    case gamestatus::ERROR:
      mtx_result[8].lock();
      result[8]++;
      mtx_result[8].unlock();
      break;

    default:
      mtx_result[8].lock();
      result[8]++;
      mtx_result[8].unlock();
      break;
    }
  }
}

int main(void) {
  putLogo();

  long n;
  int threadnum=thread::hardware_concurrency();
  vector<int> list(15,0); // Initialcards
  ofstream resultfile("result.csv");
  if (!resultfile.is_open()) {
    cout<<"File open error\n";
    return -1;
  }

  cout<<"How many test?(x"<<threadnum<<") :"<<flush;
  cin>>n;

  vector<thread> threads;

  cout<<"Making threads...\n";
  for (int i=0;i<threadnum;i++){
    threads.emplace_back(thread(threadGaming,n,i));
    cout<<i+1<<"/"<<threadnum<<"\n";
  }
  cout<<"done!\n";

  cout<<"Waiting threads...  \n";
  for (int i=0;i<threadnum;i++){
    threads[i].join();
    cout<<i<<":👍"<<"\n";
  }
  cout<<"Complete excuting!\n";

  resultfile << "SUM," << n*threadnum << "\n";
  resultfile << "WIN," << result[0] << "\n";
  resultfile << "LOSE," << result[1] << "\n";
  resultfile << "DRAW," << result[2] << "\n";
  resultfile << "DOUBLEDOWNTOWIN," << result[3] << "\n";
  resultfile << "DOUBLEDOWNTOLOSE," << result[4] << "\n";
  resultfile << "DOUBLEDOWNTODRAW," << result[5] << "\n";
  resultfile << "SALENDER," << result[6] << "\n";
  resultfile << "BLACKJACK," << result[7] << "\n";
  resultfile << "ERROR," << result[8];
}
