#include "Automaton.h"
#include <iostream>
#include <fstream>
#include <string>

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
      if(!automaton.isStateFinal(it->first)){
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
  return tableOfCorrespondances1.at(index);
}

int satValueWord(std::map<std::string,int> tableOfCorrespondances1,char letter,int pos){
  std::string index="";index.push_back(letter);
  index+=" "+std::to_string(pos);
  return tableOfCorrespondances1.at(index);
}

void printTable(std::map<std::string,int> table){
  for(std::map<std::string,int>::const_iterator it=table.begin();it!=table.end();++it){
    std::cout << it->first << " -> "<< it->second << "\n";
  }
}

using namespace std;
int main(int argc, char **argv){

  fa::Automaton A1;

  A1.addSymbol('a');
  A1.addSymbol('b');

  A1.addState(0);
  A1.addState(1);
  A1.addState(2);
  A1.addState(3);
  A1.addState(4);

  A1.setStateInitial(0);
  A1.setStateFinal(4);

  A1.addTransition(0,'a',1);
  A1.addTransition(1,'a',2);
  A1.addTransition(1,'b',2);
  A1.addTransition(2,'a',3);
  A1.addTransition(2,'b',3);
  A1.addTransition(3,'a',4);
  A1.addTransition(3,'b',4);
  A1.dotPrint(std::cout);

  fa::Automaton A2;

  A2.addSymbol('a');
  A2.addSymbol('b');

  A2.addState(0);
  A2.addState(1);
  A2.addState(2);
  A2.addState(3);
  A2.addState(4);
  A2.addState(5);
  A2.addState(6);
  A2.addState(7);

  A2.setStateInitial(0);
  A2.setStateFinal(4);

  A2.addTransition(0,'a',1);
  A2.addTransition(1,'a',2);
  A2.addTransition(2,'a',3);
  A2.addTransition(2,'b',3);
  A2.addTransition(3,'a',4);
  A2.addTransition(3,'b',4);
  A2.addTransition(0,'b',5);
  A2.addTransition(5,'a',6);
  A2.addTransition(5,'b',6);
  A2.addTransition(6,'a',7);
  A2.addTransition(6,'b',7);
  A2.addTransition(7,'a',4);
  //A2.dotPrint(std::cout);
  
  int length=A1.countStates()-1;

  std::map<std::string,int> tableOfCorrespondances;
  //index is used to insert elements
  int tableIndex=1;
  
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

  //Ajout des variables pour representer un etat(A1) a une etape : 2 7 = Le chemin passe par l'etat 2 de A1 a l'etape 7
  for(std::map<int,int>::const_iterator it=A1.etats.begin();it!=A1.etats.end();it++){
    for(int i=0;i<=length;++i){
      std::string value="A1 "+std::to_string(it->first)+" "+std::to_string(i);
      tableOfCorrespondances.insert(std::pair<std::string,int>(value,tableIndex));
      tableIndex++;
    }
  }

  //Ajout des variables pour representer un etat(A2) a une etape : 2 7 = Le chemin passe par l'etat 2 de A1 a l'etape 7
  for(std::map<int,int>::const_iterator it=A2.etats.begin();it!=A2.etats.end();it++){
    for(int i=0;i<=length;++i){
      std::string value="A2 "+std::to_string(it->first)+" "+std::to_string(i);
      tableOfCorrespondances.insert(std::pair<std::string,int>(value,tableIndex));
      tableIndex++;
    }
  }

  printTable(tableOfCorrespondances);

  ofstream cnfFile;
  cnfFile.open("test.cnf");
  cnfFile << "p cnf " << tableIndex << " 0 \n";
  /**************************Word is in A1*************************************/
  
  //at each place in the word, only a or only or b
  for(int i=1;i<=length;++i){
    cnfFile << satValueWord(tableOfCorrespondances,'a',i) << " " << satValueWord(tableOfCorrespondances,'b',i) << " 0\n";
    cnfFile << "-" << satValueWord(tableOfCorrespondances,'a',i) << " -" <<satValueWord(tableOfCorrespondances,'b',i) << " 0\n";
  }

  //Starts with an initial state
  for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
    if(A1.isStateInitial(state->first)){
      cnfFile << satValueAutomatons(tableOfCorrespondances,1,state->first,0) << " ";
    }
  }
  cnfFile << "0\n";

  //Ends with a final state
  for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
    if(A1.isStateFinal(state->first)){
      cnfFile << satValueAutomatons(tableOfCorrespondances,1,state->first,length) << " ";
    }
  }
  cnfFile << "0\n";

  //Max one state per step
  for(int step=0;step<=length;++step){
    for(size_t state=0;state<A1.countStates()-1;++state){
      for(size_t state2=state+1;state2<A1.countStates();++state2){
        cnfFile <<"-"<< satValueAutomatons(tableOfCorrespondances,1,state,step) << " -"<<satValueAutomatons(tableOfCorrespondances,1,state2,step) << " 0\n";
      }
    }
  }

  //At least one state per step
  for(int step=0;step<=length;++step){
    for(size_t state=0;state<A1.countStates();++state){
      cnfFile << satValueAutomatons(tableOfCorrespondances,1,state,step) << " ";
    }
    cnfFile << "0\n";
  }

  //word follows A1's transitions
  for(std::map<int,int>::const_iterator state=A1.etats.begin();state!=A1.etats.end();++state){
    std::set<int> destinationsA=statesFromStateLetter(&A1,state->first,'a');
    std::set<int> destinationsB=statesFromStateLetter(&A1,state->first,'b');
    for(int step=0;step<length;++step){
      cnfFile << "-" <<satValueAutomatons(tableOfCorrespondances,1,state->first,step)<<" -"<<satValueWord(tableOfCorrespondances,'a',step+1);
      for(std::set<int>::const_iterator to=destinationsA.begin();to!=destinationsA.end();to++){
        cnfFile << " "<<satValueAutomatons(tableOfCorrespondances,1,*to,step+1);
      }
      cnfFile<<" 0\n";

      cnfFile << "-" <<satValueAutomatons(tableOfCorrespondances,1,state->first,step)<<" -"<<satValueWord(tableOfCorrespondances,'b',step+1);
      for(std::set<int>::const_iterator to=destinationsB.begin();to!=destinationsB.end();to++){
        cnfFile << " "<<satValueAutomatons(tableOfCorrespondances,1,*to,step+1);
      }
      cnfFile<<" 0\n";
    }
  }

}