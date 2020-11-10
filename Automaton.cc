#include "Automaton.h"
#include <iostream>
#include <fstream>

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

int satValue(std::map<int,std::string> tableOfCorrespondances,int from,char c, int to){
  for(std::map<int,std::string>::const_iterator it=tableOfCorrespondances.begin();it!=tableOfCorrespondances.end();++it){
    if(split(it->second,' ',0)==std::to_string(from) && split(it->second,' ',1)[0]==c && split(it->second,' ',2)==std::to_string(to)){
      return it->first;
    }
  }
  return -1;
}

using namespace std;
int main(int argc, char **argv){
  
  //creation of first Automaton
    /*fa::Automaton fa;
    
    fa.addSymbol('a');
    fa.addSymbol('b');

    fa.addState(1);
    fa.addState(2);
    fa.addState(3);
    fa.addState(4);
    fa.addState(5);
    fa.setStateInitial(1);
    fa.setStateFinal(5);
    
    fa.addTransition(1,'a',4);
    fa.addTransition(1,'a',3);
    fa.addTransition(1,'a',2);
    fa.addTransition(2,'b',4);
    fa.addTransition(3,'a',4);
    fa.addTransition(3,'b',5);
    fa.addTransition(4,'a',4);
    fa.addTransition(4,'b',5);
    fa.addTransition(5,'a',5);*/

    fa::Automaton fa;
    
    fa.addSymbol('a');
    fa.addSymbol('b');
    fa.addSymbol('c');

    fa.addState(1);
    fa.addState(2);
    fa.addState(3);
    fa.addState(4);
    fa.addState(5);
    fa.addState(6);
    fa.addState(7);
    fa.addState(8);
    fa.addState(9);
    fa.addState(10);
    fa.addState(11);
    fa.addState(12);
    fa.addState(13);
    fa.addState(14);

    fa.setStateInitial(1);
    fa.setStateInitial(2);
    fa.setStateFinal(12);
    fa.setStateFinal(14);

    fa.addTransition(1,'a',3);
    fa.addTransition(1,'c',4);
    fa.addTransition(2,'a',5);
    fa.addTransition(3,'b',6);
    fa.addTransition(3,'c',4);
    fa.addTransition(4,'c',5);
    fa.addTransition(5,'b',1);
    fa.addTransition(5,'c',8);
    fa.addTransition(6,'c',7);
    fa.addTransition(7,'a',5);
    fa.addTransition(8,'a',9);
    fa.addTransition(8,'c',10);
    fa.addTransition(8,'b',11);
    fa.addTransition(9,'b',7);
    fa.addTransition(9,'c',10);
    fa.addTransition(10,'a',11);
    fa.addTransition(11,'a',13);
    fa.addTransition(11,'b',12);
    fa.addTransition(12,'a',12);
    fa.addTransition(12,'a',14);
    fa.addTransition(13,'a',13);
    fa.addTransition(13,'c',14);
    fa.addTransition(14,'b',12);

    fa.dotPrint(std::cout);
    std::size_t maxLength=fa.countStates();

    //int is value of transition that we'll use in SAT, string gives a representation : F C T F=state from, C=letter, T=state to
    //Important : here only transitions are represented, we also need to represent the step we re in
    //the variable transition (satValue) 4 at step 3 will be 4*countStates()*+3
    std::map<int,std::string> tableOfCorrespondances;
    int index=0;
    for(std::map<int,std::multimap<char,int>>::const_iterator itm=fa.transis.begin();itm!=fa.transis.end();++itm){
      for(const auto x: fa.transis.at(itm->first)){
        std::string correspondance=std::to_string(itm->first)+" "+x.first+" "+std::to_string(x.second);
        tableOfCorrespondances.insert(std::pair<int,std::string>(index*maxLength+1,correspondance));
        printf("SAT : %ld equals to : ",index*maxLength+1);std::cout << correspondance; printf("\n");
        ++index;
      }
    }
  if(argc == 1){
    //We can start writing the rules
    ofstream cnfFile;
    cnfFile.open("test.cnf");
    cnfFile << "p cnf " << std::to_string(maxLength*fa.countTransitions()) << " 0 \n";

    /********************************************************************************************************
     *********************************In first language rules************************************************
    *******************************************************************************************************/

   //max 1 transition per step : countState²*countTransition rules of two vars
    //for each step
    for(std::size_t step=1;step<maxLength+1;++step){
      //for each transi, if true, then all must be false
      for(std::size_t transi=step;transi<=maxLength*(fa.countTransitions()-1)+step;transi=transi+maxLength){
        for(std::size_t otherTransi=transi+maxLength;otherTransi<=maxLength*(fa.countTransitions()-1)+step;otherTransi=otherTransi+maxLength){
          cnfFile << "-" << std::to_string(transi) <<" -" << std::to_string(otherTransi) <<" 0\n";
        }
      }
    }

    //must start with an initial state : 1 rule with num of init transitions vars
    for(std::map<int,int>::const_iterator itOnStates=fa.etats.begin();itOnStates!=fa.etats.end();++itOnStates){
      if(fa.isStateInitial(itOnStates->first)){
        for(std::multimap<char,int>::const_iterator itOnTransi=fa.transis.at(itOnStates->first).begin();itOnTransi!=fa.transis.at(itOnStates->first).end();++itOnTransi){
          cnfFile << std::to_string(satValue(tableOfCorrespondances,itOnStates->first,itOnTransi->first,itOnTransi->second)) << " ";
        }
      }
    }
    cnfFile << "0\n";

    

    //must end with a final state : (countTransitions - numOfFinalTransitions)*(maxLength-1) rules of numOfFinalTransitions+1 var
    //if, at n+1 step, every variable is false, then one transition leading to a final step must be true at n step
    //last step is particular, it will be done just after
    //here we add the sat values of transitions leading to a final state (at step 0 by default)
    std::set<int> transiToFinalStates;
    for(std::map<int,int>::const_iterator itOnStates=fa.etats.begin();itOnStates!=fa.etats.end();++itOnStates){
      if(fa.isStateFinal(itOnStates->first)){
        for(std::map<int,std::multimap<char,int>>::const_iterator itTransi=fa.transis.begin();itTransi!=fa.transis.end();++itTransi){
          for(std::multimap<char,int>::const_iterator itOnChars=itTransi->second.begin();itOnChars!=itTransi->second.end();++itOnChars){
            if(itOnChars->second==itOnStates->first){
              transiToFinalStates.insert(satValue(tableOfCorrespondances,itTransi->first,itOnChars->first,itOnStates->first));
            }
          }
        }
      }
    }
    //If there is a transition at nth step (>1) there must be a transition at n-1th step, 
    //with n-1th transition's arriving state and nth transition's starting state being the same
    for(size_t step=2;step<maxLength+1;++step){
      for(size_t transi=step;transi<=step+(fa.countTransitions()-1)*maxLength;transi=transi+maxLength){
        cnfFile << "-" <<std::to_string(transi);
        //search for all transitions arriving on transi's starting state
        int startingState = std::stoi(split(tableOfCorrespondances.at(transi-step+1),' ',0));
        for(size_t transiBefore=step-1;transiBefore<=step+(fa.countTransitions()-1)*maxLength;transiBefore=transiBefore+maxLength){
          if(std::stoi(split(tableOfCorrespondances.at(transiBefore-step+2),' ',2))==startingState){
            cnfFile << " " <<std::to_string(transiBefore);
          }
        }
        cnfFile << " 0\n";
      }  
    }
    
    for(size_t step=1;step<maxLength;++step){
      //writing the rule
      for(auto f : transiToFinalStates){
        cnfFile << std::to_string(f+step-1) << " ";
      }
      for(std::size_t transi=step+1;transi<=maxLength*(fa.countTransitions()-1)+step;transi=transi+maxLength){
        cnfFile << std::to_string(transi) << " ";
      }
      cnfFile << "0\n";
    }

    //At the last line, there can't be a non final state
    for(std::size_t transi=fa.countStates();transi<=maxLength*(fa.countTransitions()-1)+fa.countStates();transi=transi+maxLength){
      //if this transition isn't final
      if(transiToFinalStates.find(transi-fa.countStates()+1)==transiToFinalStates.end()){
          cnfFile << "-" <<std::to_string(transi) <<" 0\n";
      }
    }

    

    /********************************************************************************************************
     ****************************Not in second language rules************************************************
    *******************************************************************************************************/

    cnfFile.close();
  }else{
    std::map<int,int> result;
    string line;
    ifstream outFile ("test.out");
    if (outFile.is_open()){
      while(getline(outFile,line)){
        if(!line.compare("SAT")==0 && !line.compare("UNSAT")==0){
          for(std::size_t i=0;i<maxLength*fa.countTransitions();++i){
            std::string var=split(line,' ',i);
            if(var.at(0)!='-'){
              cout << var << " ";
              result.insert(std::pair<int,int>(std::stoi(var),1));
            }else{
              result.insert(std::pair<int,int>(-std::stoi(var),0));
            }
          }
        }
      }
      cout << "\n";
      outFile.close();
    }
    bool wordIsFinished=false;
    std::size_t step=1;
    std::string wordFound="Word found is ";
    std::string transitionUsed="Transitions used are ";
    while(!wordIsFinished){
      wordIsFinished=true;
      if(step<maxLength+1){
        for(size_t i=step;i<=maxLength*(fa.countTransitions()-1)+step;i=i+maxLength){
          if(result.at(i)==1){
            wordIsFinished=false;
            transitionUsed+=tableOfCorrespondances.at(i-step+1)+" THEN ";
            wordFound+=split(tableOfCorrespondances.at(i-step+1),' ',1);
          }
        }
        step++;
      }
    }
    transitionUsed+='\n';
    wordFound+='\n';
    cout << wordFound << transitionUsed;
  } 
}