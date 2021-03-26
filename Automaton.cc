#include "Automaton.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h> 
namespace fa{
  Automaton::Automaton() { 
    
  }

  bool Automaton::isValid() const {
    return (!this->alphabet.empty() && !this->etats.empty());
  }

  bool Automaton::addSymbol(char symbol){
    if (hasSymbol(symbol)||symbol=='\0' || !isgraph(symbol)){
      return false; 
    }
    this->alphabet.push_back(symbol);
    return true;
  }

  bool Automaton::removeSymbol(char symbol){
    if (symbol=='\0' || !isgraph(symbol)){
      return false; 
    }
    const size_t orig_size=this->alphabet.size();
    this->alphabet.erase(std::remove(this->alphabet.begin(), this->alphabet.end(), symbol), this->alphabet.end());
    return (orig_size==this->alphabet.size()+1);
  }

  bool Automaton::hasSymbol(char symbol) const{
    return (std::find(this->alphabet.begin(), this->alphabet.end(), symbol) != this->alphabet.end());
  }

  std::size_t Automaton::countSymbols() const{
    return this->alphabet.size();
  }

  bool Automaton::addState(int state){
    std::pair<std::map<int,int>::iterator,bool> ret;
    ret = etats.insert(std::pair<int,int>(state,0));
    return(ret.second);
  }

  bool Automaton::removeState(int state){
    if(etats.erase(state)!=0){
      //erase the transitions from state
      transis.erase(state);
      //erase the transitions to state
      std::map<int,std::multimap<char,int>>::iterator itOnStates;
      for(itOnStates=transis.begin();itOnStates!=transis.end();++itOnStates){
        for(std::multimap<char,int>::iterator itOnChars=itOnStates->second.begin();itOnChars!=itOnStates->second.begin();++itOnChars){
          if(itOnChars->second==state){
            transis.at(itOnStates->first).erase(itOnChars);
          }
        }
      }
      return true;
    }
    return false;
  }

  bool Automaton::hasState(int state) const{
    return etats.find(state) != etats.end();
  }

  std::size_t Automaton::countStates() const{
    return etats.size();
  }

  void Automaton::setStateInitial(int state){
    if(hasState(state)){
      auto it=etats.begin();
      while(it->first!=state){
        ++it;
      }
      //state isn't initial
      if(!isStateInitial(state)){
        ++it->second;
      }
    }
  }

  bool Automaton::isStateInitial(int state)const{
    if(hasState(state)){
      auto it=etats.begin();
      while(it->first!=state){
        ++it;
      }
      return (it->second%2==1);
    }
    return false;
  }

  void Automaton::setStateFinal(int state){
    if(hasState(state)){
      auto it=etats.begin();
      while(it->first!=state){
        ++it;
      }
      //state isn't initial
      if(!isStateFinal(state)){
        it->second+=2;
      }
    }
  }

  bool Automaton::isStateFinal(int state)const{
    if(hasState(state)){
      auto it=etats.begin();
      while(it->first!=state){
        ++it;
      }
      return (it->second>=2);
    }
    return false;
  }

  bool Automaton::addTransition(int from, char alpha, int to){
    if(!hasState(from)||!hasState(to)||!hasSymbol(alpha)){
      return false;
    }
    if(hasTransition(from,alpha,to)){
      return false;
    }
    std::size_t before=transis.size();
    //add from in transis if this is the first transition from "from"
    if(transis.find(from) == transis.end()){
      transis.insert(std::pair<int,std::multimap<char,int>>(from,std::multimap<char,int>()));
      if(transis.size()!=before+1){
        return false;
      }
    }
    before=transis[from].size();
    transis[from].insert(std::pair<char,int>(alpha,to));
    return(transis[from].size()==before+1);
  }

  bool Automaton::removeTransition(int from, char alpha, int to){
    if(!hasTransition(from,alpha,to)){
      return false;
    }
    std::pair<std::multimap<char,int>::const_iterator,std::multimap<char,int>::const_iterator>result=transis.at(from).equal_range(alpha);
    for(std::multimap<char,int>::const_iterator it=result.first;it!=result.second;++it){
      if(it->second==to){
        transis.at(from).erase(it);
        break;
      }
    }
    return(!hasTransition(from,alpha,to));
  }

  bool Automaton::hasTransition(int from, char alpha, int to)const{
    if(!hasState(from)||!hasState(to)||!hasSymbol(alpha)){
      return false;
    }
    if(transis.find(from)==transis.end()){
      return false;
    }
    if(transis.at(from).find(alpha)==transis.at(from).end()){
      return false;
    }
    std::pair<std::multimap<char,int>::const_iterator,std::multimap<char,int>::const_iterator>result=transis.at(from).equal_range(alpha);
    for(std::multimap<char,int>::const_iterator it=result.first;it!=result.second;++it){
      if(it->second==to){
        return true;
      }
    }
    return false;
  }

  std::size_t Automaton::countTransitions()const{
    std::size_t result=0;
    std::map<int,std::multimap<char,int>>::const_iterator transiIterator;
    for(transiIterator=transis.begin();transiIterator!=transis.end();++transiIterator){
      result+=transiIterator->second.size();
    }
    return result;
  }

  void Automaton::prettyPrint(std::ostream& os) const{
    //initial states
    printf("Initial States :\n\t");
    std::map<int,int>::const_iterator it;
    for(it=etats.begin();it!=etats.end();++it){
      if(it->second%2==1){
        printf("%d ",it->first);
      }
    }
    //final states
    printf("\nInitial States :\n\t");
    for(it=etats.begin();it!=etats.end();++it){
      if(it->second>1){
        printf("%d ",it->first);
      }
    }
    //Transitions
    std::map<int,int>::const_iterator itStatesFrom;
    printf("\nTransitions : \n");
    for(itStatesFrom=etats.begin();itStatesFrom!=etats.end();++itStatesFrom){
      printf("\tFor State %d :\n",itStatesFrom->first);
      for(auto i : alphabet){
        printf("\t\tFor Letter %c:",i);
        std::map<int,int>::const_iterator itStatesTo;
        for(itStatesTo=etats.begin();itStatesTo!=etats.end();++itStatesTo){
          if(hasTransition(itStatesFrom->first,i,itStatesTo->first)){
            printf(" %d",itStatesTo->first);
          }
        }
        printf("\n");
      }
    }
  }

  void Automaton::dotPrint(std::ostream& os) const{
    printf("digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\"\n");
   
    //final states
    std::map<int,int>::const_iterator it;
    printf("node [shape = doublecircle];");
    for(it=etats.begin();it!=etats.end();++it){
      if(it->second>1){
        printf("%d ",it->first);
      }
    }
    printf(";\n");
    printf("node [shape = circle];\n");

    //initial states
    for(it=etats.begin();it!=etats.end();++it){
      if(it->second%2==1){
        printf("initArrow%d [label= \"\", shape=none,height=.0,width=.0]\n",it->first);
        printf("initArrow%d -> %d\n",it->first,it->first);
      }
    }
    printf("\n");
    //Transitions
    for(std::map<int,std::multimap<char,int>>::const_iterator itTransis=transis.begin();itTransis!=transis.end();++itTransis){
      for(std::multimap<char,int>::const_iterator itOnChars=transis.at(itTransis->first).begin();itOnChars!=transis.at(itTransis->first).end();++itOnChars){
        printf("%d -> %d [label = \"%c\"];\n",itTransis->first,itOnChars->second,itOnChars->first);
      }
    }
    printf("}\n");
  }

  bool Automaton::hasEpsilonTransition() const{
    if(!hasSymbol('\0')){
      return false;
    }
    std::map<int,std::multimap<char,int>>::const_iterator transiIterator;
    for(transiIterator=transis.begin();transiIterator!=transis.end();++transiIterator){
      if(transiIterator->second.find('\0')!=transiIterator->second.end()){
        return true;
      }
    }
    return false;
  }

  bool Automaton::isDeterministic() const{
    if(!isValid()){
      return false;
    }
    //No eps transition
    if(hasEpsilonTransition()){
      return true;
    }
    //One and only one initial state
    int numOfInit=0;
    for(std::map<int,int>::const_iterator it=etats.begin();it!=etats.end();++it){
      numOfInit+=(it->second%2==1);
    }
    if(numOfInit!=1){
      return false;
    }

    //Maximum one transition per letter from each state
    for(std::map<int,std::multimap<char,int>>::const_iterator itTransis=transis.begin();itTransis!=transis.end();++itTransis){
      for(auto alpha : alphabet){
        if(itTransis->second.count(alpha)>1){
          return false;
        }
      }
    }
    return true;
  }

  bool Automaton::isComplete()const{
    if(!isValid()){
      return false;
    }

    //Minimum one transition per letter from each state
    for(std::map<int,std::multimap<char,int>>::const_iterator itTransis=transis.begin();itTransis!=transis.end();++itTransis){
      for(auto alpha : alphabet){
        if(itTransis->second.count(alpha)<1){
          return false;
        }
      }
    }
    return true;
  }

  void Automaton::removeNonAccessibleStates(){
    //vector containing the accessible states
    std::set<int> accessibleStates;
    //depth first search from each initial state
    for(std::map<int,int>::const_iterator itStates=etats.begin();itStates!=etats.end();++itStates){
      //if Initial and not already visited
      if(itStates->second%2==1 && accessibleStates.find(itStates->second)==accessibleStates.end()){
        depthSearchAccessibles(this,accessibleStates,itStates->first);
      }
    }

    std::map<int,int>::const_iterator it=etats.begin();
    while(it!=etats.end()){
      if(std::find(accessibleStates.begin(),accessibleStates.end(),it->first)==accessibleStates.end()){
        int toRemove=it->first;
        ++it;
        removeState(toRemove);
      }else{
        ++it;
      }
    }
  }

  void Automaton::depthSearchAccessibles(const Automaton *automaton,std::set<int> &accessibleStates,int state){
    accessibleStates.insert(state);
    std::set<int> to =statesFromState(automaton,state);
    for(std::set<int>::const_iterator it=to.begin();it!=to.end();++it){
      if(accessibleStates.insert(*it).second){
        depthSearchAccessibles(automaton,accessibleStates,*it);
      }
    }
  }

  /*
  For a given state, returns a set containing all states that are accessible within a single move
  */
  std::set<int> Automaton::statesFromState(const Automaton *automaton,int from){
    std::set<int>accessibleStates;
    for(std::multimap<char,int>::const_iterator it=automaton->transis.at(from).begin();it!=automaton->transis.at(from).end();++it){
      if (std::find(accessibleStates.begin(),accessibleStates.end(), it->second) == accessibleStates.end()){
        accessibleStates.insert(it->second);
      }
    }
    return accessibleStates;
  }

  /*
  For a given state, returns a set containing all states that are accessible within a single move using a given letter
  */
  std::set<int> statesFromStateLetter(const Automaton *automaton,int from,char c){
    std::set<int>accessibleStates;
    for(std::map<int,int>::const_iterator to=automaton->etats.begin();to!=automaton->etats.end();++to){
      if(automaton->hasTransition(from,c,to->first)){
        accessibleStates.insert(to->first);
      }
    }
    return accessibleStates;
  }

  void Automaton::removeNonCoAccessibleStates(){
    //vector containing the accessible states
    std::set<int> coAccessibleStates;
    //depth first search from each initial state
    for(std::map<int,int>::const_iterator itStates=etats.begin();itStates!=etats.end();++itStates){
      //if Initial and not already visited
      if(itStates->second>1 && coAccessibleStates.find(itStates->second)==coAccessibleStates.end()){
        depthSearchCoAccessibles(this,coAccessibleStates,itStates->first);
      }
    }

    std::map<int,int>::const_iterator it=etats.begin();
    while(it!=etats.end()){
      if(std::find(coAccessibleStates.begin(),coAccessibleStates.end(),it->first)==coAccessibleStates.end()){
        int toRemove=it->first;
        ++it;
        removeState(toRemove);
      }else{
        ++it;
      }
    }
  }

  void Automaton::depthSearchCoAccessibles(const Automaton *automaton,std::set<int> &coAccessibleStates,int state){
    //printf("\n%d",state);
    coAccessibleStates.insert(state);
    std::set<int> to =statesToState(automaton,state);
    for(std::set<int>::const_iterator it=to.begin();it!=to.end();++it){
      if(coAccessibleStates.insert(*it).second){
        depthSearchCoAccessibles(automaton,coAccessibleStates,*it);
      }
    }
  }

  /*
  For a given state, returns a set containing all states that can access the state within a single move
  */
  std::set<int> Automaton::statesToState(const Automaton *automaton,int to){
    std::set<int>coAccessibleStates;
    for(std::map<int,std::multimap<char,int>>::const_iterator itTransis=automaton->transis.begin();itTransis!=automaton->transis.end();++itTransis){
      for(std::multimap<char,int>::const_iterator itOnChars=itTransis->second.begin();itOnChars!=itTransis->second.end();++itOnChars){
        if(itOnChars->second==to){
          coAccessibleStates.insert(itTransis->first);
        }
      }
    }
    return coAccessibleStates;
  }

  bool Automaton::isLanguageEmpty() const{
    //Get all states accessible from initial states, then check if there is at least one that is final

    //vector containing the accessible states
    std::set<int> accessibleStates;
    //depth first search from each initial state
    for(std::map<int,int>::const_iterator itStates=etats.begin();itStates!=etats.end();++itStates){
      //if Initial and not already visited
      if(itStates->second%2==1 && accessibleStates.find(itStates->second)==accessibleStates.end()){
        depthSearchAccessibles(this,accessibleStates,itStates->first);
      }
    }

    std::set<int>::const_iterator it;
    for(it=accessibleStates.begin();it!=accessibleStates.end();++it){
      if(isStateFinal(*it)){
        return false;
      }
    }
    return true;
  }

  Automaton Automaton::createComplete(const Automaton& automaton){
    Automaton result=Automaton();
    //copy the alphabet
    for(std::vector<char>::const_iterator it=automaton.alphabet.begin();it!=automaton.alphabet.end();++it){
      result.addSymbol(*it);
    }

    //copy the states
    for(std::map<int,int>::const_iterator it=automaton.etats.begin();it!=automaton.etats.end();++it){
      result.addState(it->first);
      if(automaton.isStateInitial(it->first)){
        result.setStateInitial(it->first);
      }
      if(automaton.isStateFinal(it->first)){
        result.setStateFinal(it->first);
      }
    }

    //copy the transitions
    for(std::map<int,std::multimap<char,int>>::const_iterator itStates=automaton.transis.begin();itStates!=automaton.transis.end();++itStates){
      for(std::multimap<char,int>::const_iterator itOnChars=automaton.transis.at(itStates->first).begin();itOnChars!=automaton.transis.at(itStates->first).end();++itOnChars){
        result.addTransition(itStates->first,itOnChars->first,itOnChars->second);
      }
    }

    if(!automaton.isComplete()){
      //Etats poubelle with number = lowest unused
      int i=0;
      while(automaton.hasState(i)){
        ++i;
      }
      result.addState(i);
      
        
    for(std::vector<char>::const_iterator alpha=automaton.alphabet.begin();alpha!=automaton.alphabet.end();++alpha){
      result.addTransition(i,*alpha,i);
      for(std::map<int,std::multimap<char,int>>::const_iterator itStates=automaton.transis.begin();itStates!=automaton.transis.end();++itStates){
        if(itStates->second.find(*alpha)==itStates->second.end()){
          result.addTransition(itStates->first,*alpha,i);
        }
      }
    }
      
    }
    return result;
  }

  Automaton Automaton::createMirror(const Automaton& automaton){
    Automaton result=Automaton();
    //copy the alphabet
    for(std::vector<char>::const_iterator it=automaton.alphabet.begin();it!=automaton.alphabet.end();++it){
      result.addSymbol(*it);
    }

    //copy the states
    for(std::map<int,int>::const_iterator it=automaton.etats.begin();it!=automaton.etats.end();++it){
      result.addState(it->first);
      if(automaton.isStateInitial(it->first)){
        result.setStateInitial(it->first);
      }
      if(automaton.isStateFinal(it->first)){
        result.setStateFinal(it->first);
      }
    }

    //copy the transitions
    for(std::map<int,std::multimap<char,int>>::const_iterator itStates=automaton.transis.begin();itStates!=automaton.transis.end();++itStates){
      for(std::multimap<char,int>::const_iterator itOnChars=automaton.transis.at(itStates->first).begin();itOnChars!=automaton.transis.at(itStates->first).end();++itOnChars){
        result.addTransition(itOnChars->second,itOnChars->first,itStates->first);
      }
    }
    
    return result;
  }

  Automaton Automaton::createComplement(const Automaton& automaton){
    Automaton copy=automaton;
      if(!copy.isDeterministic()){
        copy=createDeterministic(copy);
      }
      if(!copy.isComplete())
      copy=createComplete(copy);
      
    Automaton result=Automaton();
    //copy the alphabet
    for(std::vector<char>::const_iterator it=copy.alphabet.begin();it!=copy.alphabet.end();++it){
      copy.addSymbol(*it);
    }

    //copy the states
    for(std::map<int,int>::const_iterator it=copy.etats.begin();it!=copy.etats.end();++it){
      result.addState(it->first);
      if(copy.isStateInitial(it->first)){
        result.setStateInitial(it->first);
      }
      if(!copy.isStateFinal(it->first)){
        result.setStateFinal(it->first);
      }
    }

    //copy the transitions
    for(std::map<int,std::multimap<char,int>>::const_iterator itStates=automaton.transis.begin();itStates!=automaton.transis.end();++itStates){
      for(std::multimap<char,int>::const_iterator itOnChars=automaton.transis.at(itStates->first).begin();itOnChars!=automaton.transis.at(itStates->first).end();++itOnChars){
        result.addTransition(itStates->first,itOnChars->first,itOnChars->second);
      }
    }
    
    return result;
  }


    /**
     * Create a deterministic automaton, if not already deterministic
     */
    Automaton Automaton::createDeterministic(const Automaton& other){
      Automaton product;
      product.alphabet=other.alphabet;
      std::map<int,std::set<int>>tab;
      std::set<int>dest;
      product.addState(0);
      auto s=other.etats.begin();
      while(s!=other.etats.end()){
        if(s->second==1){//Initial
          tab[0].insert(s->first);
        }
        s++;
      }
      //first state will be initial (build from initial states)
      product.setStateInitial(0);

      if(tab[0].empty()){
        return product;
      }
      
      auto it=tab.begin();
      while(it!=tab.end()){
        for(size_t i=0;i<product.alphabet.size();++i){
          
          int in=0;
          for(auto s : it->second){

              dest=statesFromStateLetter(&product,s,product.alphabet[i]);
             
          }
        
        if(!dest.empty()){
          for(auto it = tab.begin();it!=tab.end();it++){
            if(it->second==dest){
              in=1;
              break;
            }
          }
          int n=-1;
          if(in==0){
            int n=tab.size();
            tab[n]=dest;
            product.addState(n);
            product.addTransition(it->first,product.alphabet[i],n);
          }else{
            for(auto snb : tab){
              if(snb.second==dest){
                n=snb.first;
              }
            }
            product.addTransition(it->first,product.alphabet[i],n);
          }
        }
          
        
        dest.clear();
      }
      
      it++;
    }
    //Search the final state
    for(auto e : tab){
      for(auto s : e.second){
        if(other.isStateFinal(s)){
          product.setStateFinal(e.first);
        }
      }
    }


      return product;
    }

}
struct transi{
  int from;
  char alpha;
  int to;
};

std::string split(std::string str,char delimiter, int posWanted){
  int actualPos=0;
  std::string res="";
  for(std::size_t i=0;i<str.size();++i){
    if(str.at(i)==delimiter){
      if(actualPos==posWanted){
        return res;
      }
      ++actualPos;
      res="";
    }else{
      res+=str.at(i);
    }
  }
  if(actualPos==posWanted){
    return res;
  }
  return "NOT FOUND";
}

//automaton : 1 for the one to be included, 2 for the other
int satValueAutomatons(std::map<std::string,int> tableOfCorrespondances1,int automaton,int state, int step){
  std::string index="";
  if(automaton==1){
    index+="A1 ";
  }else if(automaton==2){
    index+="A2 ";
  }
  index+=std::to_string(state)+" "+std::to_string(step);
  return tableOfCorrespondances1[index];
}

int satValueWord(std::map<std::string,int> tableOfCorrespondances1,char letter,int pos){
  std::string index="";index.push_back(letter);
  index+=" "+std::to_string(pos);
  return tableOfCorrespondances1[index];
}

void printTable(std::map<std::string,int> table){
  for(std::map<std::string,int>::const_iterator it=table.begin();it!=table.end();++it){
    std::cout << it->first << " -> "<< it->second << "\n";
  }
}
/*
Création d'automate random 
*/
using namespace std;
fa::Automaton RandomAutomaton(int nbstates){

  ofstream fich("./CreateAutomaton2.txt");
  fa::Automaton res;
  std::vector<char> letter;
  letter.push_back('a');
  letter.push_back('b');
  for(char c : letter){
    res.addSymbol(c);
    fich << "A2.addSymbol('"<<c<<"');"<<"\n";
  }
  for(int i=0;i<nbstates;++i){
    res.addState(i);
    fich << "A2.addState("<<i<<");"<<"\n";
  }
  for(auto s : res.etats){
    for(auto ss : res.etats){
      for(char c : letter){
        double rand1 = rand() / (double)RAND_MAX;
        
        if(rand1 < 1.6/nbstates){
          //printf("\ndouble : %f\n",rand1);
          res.addTransition(s.first,c,ss.first);
          fich << "A2.addTransition("<<s.first<<",'"<<c<<"',"<<ss.first<<");"<<"\n";
        }
      }
    }
  }

  for(int i=0;i<nbstates;i++){
    double rand1 = rand() / (double)RAND_MAX;
    if(rand1 < 0.3){
      res.setStateFinal(i);
      fich << "A2.setStateFinal("<<i<<");"<<"\n";
    }
  }
  for(int i=1;i<nbstates;i++){
    double rand1 = rand() / (double)RAND_MAX;
    if(rand1 < 0.3){
      res.setStateInitial(i);
      fich << "A2.setStateInitial("<<i<<");"<<"\n";
    }
  }
  res.setStateInitial(0);
  fich<<"A2.setStateInitial(0);"<<"\n";

  return res;
}



using namespace std;
int main(int argc, char **argv){
  int length=40;
  if(argc>2){
    length=stoi(argv[2]);
  }
  //printf("length : %d\n",length);
  
  if(argc==1){
    std::map<int,int> result;
    string line;
    ifstream outFile ("test.out");
    if (outFile.is_open()){
      while(getline(outFile,line)){
        if(line.compare("UNSAT")==0){
          std::cout << "A1 is included in A2\n";
          return 1;
        }
        std::cout << "A1 is not included in A2\n";

        if(!line.compare("SAT")==0){
          for(int i=0;i<length;++i){
            std::string var=split(line,' ',i);
            if (var.at(0)!='-'){
              std::cout<<"a";
            }else{
              std::cout<<"b";
            }
          }
        }
      }
      std::cout << "\n";
      outFile.close();
    }
  }else{
    int nbStates=10;
    if(argc>3){
      nbStates=stoi(argv[3]);
    }
    //printf("nb : %d\n",nbStates);
    srand(20);
    //fa::Automaton A1=RandomAutomaton(40);
    if(argc==5){
      srand(atoi(argv[4]));
    }else{
      srand(time(NULL));
    }
      
  //Automate reconnaissant tous les mots
       fa::Automaton A1;

      A1.addSymbol('a');
      A1.addSymbol('b');
      A1.addState(0);

      A1.setStateInitial(0);
      A1.setStateFinal(0);

       A1.addTransition(0,'a',0);
       A1.addTransition(0,'b',0);
      A1.addState(0);A1.addState(1);
      A1.addState(2);A1.addState(3);
      
      fa::Automaton A2=RandomAutomaton(nbStates);
        //A1.dotPrint(std::cout);
       //A2.dotPrint(std::cout);
      // return 0;
      std::map<std::string,int> tableOfCorrespondances;
      //index is used to insert elements
      int tableIndex=1;
      
      //clock_t c2 =clock();
      //Ajout des variables pour representer le mot : a4 = la 4eme lettre du mot est a
      for(std::vector<char>::const_iterator it=A1.alphabet.begin();it!=A1.alphabet.end();++it){
        for(int i=1;i<=length;++i){
          std::string value="";
          value.push_back(*it);
          value+=" "+std::to_string(i);
          tableOfCorrespondances.insert(std::pair<std::string,int>(value,tableIndex));
          tableIndex++;
        }
      }
      //printf("C2 : %.2fs \n",(double)(clock()-c2)/CLOCKS_PER_SEC);

      //clock_t c3=clock();
      //Ajout des variables pour representer un etat(A1) a une etape : 2 7 = Le chemin passe par l'etat 2 de A1 a l'etape 7
      for(std::map<int,int>::const_iterator it=A1.etats.begin();it!=A1.etats.end();it++){
        for(int i=0;i<=length;++i){
          std::string value="A1 "+std::to_string(it->first)+" "+std::to_string(i);
          tableOfCorrespondances.insert(std::pair<std::string,int>(value,tableIndex));
          tableIndex++;
        }
      }
      //printf("C3 : %.2fs \n",(double)(clock()-c3)/CLOCKS_PER_SEC);

      //clock_t c4=clock();
      //Ajout des variables pour representer un etat(A2) a une etape : 2 7 = Le chemin passe par l'etat 2 de A1 a l'etape 7
      for(std::map<int,int>::const_iterator it=A2.etats.begin();it!=A2.etats.end();it++){
        for(int i=0;i<=length;++i){
          std::string value="A2 "+std::to_string(it->first)+" "+std::to_string(i);
          tableOfCorrespondances.insert(std::pair<std::string,int>(value,tableIndex));
          tableIndex++;
        }
      }

      //printf("C4 : %.2fs \n",(double)(clock()-c4)/CLOCKS_PER_SEC);

      ofstream cnfFile;
      cnfFile.open("test.cnf");
      cnfFile << "p cnf " << tableIndex << " 0 \n";
      /**************************Word is in A1*************************************/
      //clock_t c5=clock();
      //at each place in the word, only a or only or b
      for(int i=1;i<=length;++i){
        std::string index1 ="a "+std::to_string(i);
        std::string index2 ="b "+std::to_string(i);
        cnfFile << tableOfCorrespondances[index1] << " " << tableOfCorrespondances[index2] << " 0\n";
        cnfFile << "-" << tableOfCorrespondances[index1] << " -" <<tableOfCorrespondances[index2] << " 0\n";
      }

      //Starts with an initial state
      for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
        if(A1.isStateInitial(state->first)){
          std::string index1="A1 "+std::to_string(state->first)+" "+"0";
          cnfFile << tableOfCorrespondances[index1] << " ";
        }
      }
      cnfFile << "0\n";

      //Ends with a final state
      for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
        if(A1.isStateFinal(state->first)){
          std::string index1="A1 "+std::to_string(state->first)+" "+std::to_string(length);
          cnfFile << tableOfCorrespondances[index1] << " ";
        }
      }
      cnfFile << "0\n";
      //printf("C5 : %.2fs \n",(double)(clock()-c5)/CLOCKS_PER_SEC);

      //clock_t c6=clock();
      //Max one state per step
      for(int step=0;step<=length;++step){
        for(size_t state=0;state<A1.countStates()-1;++state){
          for(size_t state2=state+1;state2<A1.countStates();++state2){
            std::string index1="A1 "+std::to_string(state)+" "+std::to_string(step);
            std::string index2="A1 "+std::to_string(state2)+" "+std::to_string(step);
            cnfFile <<"-"<< tableOfCorrespondances[index1] << " -"<<tableOfCorrespondances[index2]<< " 0\n";
          }
        }
      }

      //At least one state per step
      for(int step=0;step<=length;++step){
        for(size_t state=0;state<A1.countStates();++state){
          std::string index1="A1 "+std::to_string(state)+" "+std::to_string(step);
          cnfFile << tableOfCorrespondances[index1] << " ";
        }
        cnfFile << "0\n";
      }
      //printf("C6 : %.2fs \n",(double)(clock()-c6)/CLOCKS_PER_SEC);

      //clock_t c7=clock();
      //word follows A1's transitions
      for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
        std::set<int> destinationsA=statesFromStateLetter(&A1,state->first,'a');
        std::set<int> destinationsB=statesFromStateLetter(&A1,state->first,'b');
        for(int step=0;step<length;++step){
          std::string index1="A1 "+std::to_string(state->first)+" "+std::to_string(step);
          std::string index2 ="a "+std::to_string(step+1);
          cnfFile << "-" <<tableOfCorrespondances[index1]<<" -"<<tableOfCorrespondances[index2];
          for(std::set<int>::const_iterator to=destinationsA.begin();to!=destinationsA.end();to++){
            std::string index3="A1 "+std::to_string(*to)+" "+std::to_string(step+1);
            cnfFile << " "<<tableOfCorrespondances[index3];
          }
          cnfFile<<" 0\n";
          std::string index4 ="b "+std::to_string(step+1);
          cnfFile << "-" <<tableOfCorrespondances[index1]<<" -"<<tableOfCorrespondances[index4];
          for(std::set<int>::const_iterator to=destinationsB.begin();to!=destinationsB.end();to++){
            std::string index3="A1 "+std::to_string(*to)+" "+std::to_string(step+1);
            cnfFile << " "<<tableOfCorrespondances[index3];
          }
          cnfFile<<" 0\n";
        }
      }
      //printf("C7 : %.2fs \n",(double)(clock()-c7)/CLOCKS_PER_SEC);

      /**** Rules of A2  ****/
      //clock_t c8=clock();
      //Starts with an initial state
      for(std::map<int,int>::const_iterator state=A2.etats.begin();state!=A2.etats.end();++state){
        if(A2.isStateInitial(state->first)){
          // std::string index="A2 ";index+=std::to_string(state->first)+" 0";
          // cnfFile << tableOfCorrespondances[index] << " ";
          std::string index="A2 "+std::to_string(state->first)+" 0";
          cnfFile << tableOfCorrespondances[index] << " 0\n";
        }
      }
     // cnfFile << "0\n";
      //printf("C8 : %.2fs \n",(double)(clock()-c8)/CLOCKS_PER_SEC);

      //clock_t c9=clock();
      //Accessible States
      for(std::map<int,int>::const_iterator state=A2.etats.begin();state!=A2.etats.end();++state){
        std::set<int> destinationsA=statesFromStateLetter(&A2,state->first,'a');
        std::set<int> destinationsB=statesFromStateLetter(&A2,state->first,'b');
        for(int step=0;step<length;step++){
          for(auto to : destinationsA){
            std::string index="A2 ";index+=std::to_string(state->first)+" "+std::to_string(step); //satValueAutomatons
            std::string index2="";index2.push_back('a');index2+=" "+std::to_string(step+1); //satValuWord
            std::string index3="A2 ";index3+=std::to_string(to)+" "+std::to_string(step+1); //satValueAutomatons
            cnfFile << "-" <<tableOfCorrespondances[index]<<" -"<<tableOfCorrespondances[index2];
            cnfFile << " " << tableOfCorrespondances[index3];
            cnfFile<<" 0\n";
          }
          
          for(auto to : destinationsB){
            std::string index="A2 ";index+=std::to_string(state->first)+" "+std::to_string(step); //satValueAutomatons
            std::string index2="";index2.push_back('b');index2+=" "+std::to_string(step+1); //satValueWord
            std::string index3="A2 ";index3+=std::to_string(to)+" "+std::to_string(step+1); //satValueAutomatons
            cnfFile << "-" <<tableOfCorrespondances[index]<<" -"<<tableOfCorrespondances[index2];
            cnfFile << " " << tableOfCorrespondances[index3];
            cnfFile<<" 0\n";
          }
        }
      }
      //printf("\nC9 : %.2fs\n",(double)(clock()-c9)/CLOCKS_PER_SEC);

      //clock_t c10=clock();
      //isNotFinalState
      for(auto state : A2.etats){
        if(A2.isStateFinal(state.first)){
          std::string index= "A2 "+std::to_string(state.first)+" "+std::to_string(length);
          cnfFile <<" -"<<tableOfCorrespondances[index] << " 0\n";      
        }
      }
      //printf("C10 : %.2fs \n",(double)(clock()-c10)/CLOCKS_PER_SEC);
  }
}