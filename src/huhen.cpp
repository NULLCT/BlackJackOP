#include "bjutil.cpp"
#include <string>

using namespace std;

std::mt19937 mtforper{std::random_device{}()};
std::uniform_int_distribution<int> distforper(1, 10000000);
//10^7

namespace per{
  //sum = 1
  constexpr long double WIN    = 0.32531629;
  constexpr long double LOSE   = 0.41007484;
  constexpr long double DRAW   = 0.07719535;
  constexpr long double DDWIN  = 0.05256743;
  constexpr long double DDLOSE = 0.03678966;
  constexpr long double DDDRAW = 0.00662317;
  constexpr long double SLDR   = 0.04680537;
  constexpr long double BJ     = 0.04462789;
}

string getBetMoneyMG(){
  long double per = distforper(mtforper)/static_cast<long double>(10000000);

  using namespace per;

  if      (per <= (WIN)){ //WIN
    return "勝";
  }else if(per <= (WIN)+(LOSE)){ //LOSE
    return "負";
  }else if(per <= (WIN)+(LOSE)+(DRAW)){ //DRAW
    return "引";
  }else if(per <= (WIN)+(LOSE)+(DRAW)+(DDWIN)){//DDWIN
    return "超勝";
  }else if(per <= (WIN)+(LOSE)+(DRAW)+(DDWIN)+(DDLOSE)){//DDLOSE
    return "超負";
  }else if(per <= (WIN)+(LOSE)+(DRAW)+(DDWIN)+(DDLOSE)+(DDDRAW)){//DDDRAW
    return "超引";
  }else if(per <= (WIN)+(LOSE)+(DRAW)+(DDWIN)+(DDLOSE)+(DDDRAW)+(SLDR)){//SLDR
    return "参";
  }else if(per <= (WIN)+(LOSE)+(DRAW)+(DDWIN)+(DDLOSE)+(DDDRAW)+(SLDR)+(BJ)){//BJ
    return "黒";
  }

  return "エラー";
}

int main(){
  cout<<"How many arrays? :";
  int n;cin>>n;
  vector<string> ary(n);
  
  for(auto &i:ary){
    i=getBetMoneyMG();
  }

  for(auto i:ary){
    cout<<i<<" ";
  }
  cout<<"\n";
}