#include "Automaton2.h"
#include <iostream>
#include <fstream>
#include <string>

namespace fa {
  
  Automaton::Automaton() {
  }
  /**
     * Tell if an automaton is valid.
     *
     * A valid automaton has a non-empty set of states and a non-empty set of symbols
     */
  bool Automaton::isValid() const {

    return !(alphabet.empty() || states.empty());
  }
  /**
     * Add a symbol to the automaton
     *
     * Epsilon is not a valid symbol.
     * Returns true if the symbol was effectively added
     */
  bool Automaton::addSymbol(char symbol){
    if((symbol==fa::Epsilon) || (isgraph(symbol)== 0)){
      return false;
    }
    if(hasSymbol(symbol)){
      return false;
    }
    alphabet.push_back(symbol);
    std::sort(alphabet.begin(),alphabet.end(),[](const char& a,const char& b){return a<b;});
    return (hasSymbol(symbol));
  }
  /**
     * Remove a symbol from the automaton
     *
     * Returns true if the symbol was effectively removed
     */
  bool Automaton::removeSymbol(char symbol){
    if((symbol==fa::Epsilon) || (isgraph(symbol)== 0)){
      return false;
    }
    if(!hasSymbol(symbol)){
      return false;
    }
    size_t size=alphabet.size();
    for(size_t i=0;i<size;++i){
      if(alphabet[i]==symbol){
        //remove transitions
           for(auto &s : states){
               auto tr=s.transitions.begin();
               while(tr!=s.transitions.end()){
                if(tr->symbol==symbol){
                  tr=s.transitions.erase(tr);
                }else{
                  tr++;
                }
              }
            }
        alphabet.erase(alphabet.begin()+i);
      }
    }
    return (!hasSymbol(symbol));
  }
  /**
     * Tell if the symbol is present in the automaton
     */
  bool Automaton::hasSymbol(char symbol) const{
    
    if(isgraph(symbol)==0){
      return false;
    }

    for(auto l : alphabet){
      if(l==symbol){
        return true;
      }
    }
    return false;
  }
  /**
     * Count the number of symbols
     */
  std:: size_t Automaton::countSymbols () const{
    return alphabet.size();
  }
  /**
     * Add a state to the automaton.
     *
     * By default, a newly added state is not initial and not final.
     * Returns true if the state was effectively added and false otherwise.
     */
  bool Automaton::addState(int state){
    if(hasState(state) || state<0){
      return false;
    }
    states.insert(state);
    return (hasState(state));
  }
  /**
   * Remove a state from the automaton.
   *
   * The transitions involving the state are also removed.
   * Returns true if the state was effectively removed and false otherwise.
   */
  bool Automaton::removeState(int state){

    if(!hasState(state)){
      return false;
    }
    
    for(auto s=states.begin();s!=states.end();){
      if(s->nb==state){
        s=states.erase(s);
      }else{
        auto tr=s->transitions.begin();
        while(tr!=s->transitions.end()){
              if(tr->to==state){
                tr=s->transitions.erase(tr);
              }else{
                tr++;
              }
              
        }
        s++;
      }
    }
    return (!hasState(state));
  }

  /*** 
   * Tell if the  state  is  present  in the  automaton.
   **/
  bool Automaton::hasState(int state)const{

    return (states.find(state)!=states.end());
  }
  /**
     * Compute the number of states.
     */
    std::size_t Automaton::countStates() const{
      return states.size();
    }

    /**
     * Set the state initial.
     */
    void Automaton::setStateInitial(int state){
      for(auto &s : states){
        if(s.nb==state){
          s.isInit=true;
            break;
        }
      }
    }
    /**
     * Tell if the state is initial.
     */
    bool Automaton::isStateInitial(int state) const{
      for(auto const s : states){
        if(s.nb==state){
          return s.isInit;
        }
      }
      return false;
    }

    /**
     * Set the state final.
     */
    void Automaton::setStateFinal(int state){
      for(auto &s : states){
        if(s.nb==state){
          s.isFinal=true;
          break;
        }
      }
    }

    /**
     * Tell if the state is final.
     */
    bool Automaton::isStateFinal(int state) const{
      for(auto const s : states){
        if(s.nb==state){
          return s.isFinal;
        }
      }
      return false;
    }
    

    /**
     * Add a transition
     *
     * Returns true if the transition was effectively added and false otherwise.
     * If one of the state or the symbol does not exists, the transition is not added.
     */
    bool Automaton::addTransition(int from, char alpha, int to){
      if(alpha!=fa::Epsilon){
        if(!hasSymbol(alpha) || !hasState(from) || !hasState(to)){
          return false;
        }
      }
      
      if(hasTransition(from,alpha,to)){
        return false;
      }
      for(auto &s : states){
        if(s.nb==from){
          s.transitions.insert(Transition(from,alpha,to));
          break;         
        }
      }
      
      return hasTransition(from,alpha,to);

    }
    /**
     * Remove a transition
     *
     * Returns true if the transition was effectively removed and false otherwise.
     */
    bool Automaton::removeTransition(int from, char alpha, int to){
      if(alpha!=fa::Epsilon){
        if(!hasSymbol(alpha) || !hasState(from) || !hasState(to)){
          return false;
        }
      }
      
      for(auto &s : states){
        if(s.nb==from){
          for(auto tr : s.transitions){
            if(tr.from==from && tr.symbol==alpha && tr.to==to){ 
              s.transitions.erase(tr);
              
              return (!hasTransition(from,alpha,to));
            }
              
          }
          
        }
      }
      //If Transition don't exist
      return false;
    }

    /**
     * Tell if a transition is present.
     */
    bool Automaton::hasTransition(int from, char alpha, int to) const{
 
      for(auto s : states){
        
        if(s.nb==from){
          Transition tr=Transition(from,alpha,to);
          return (s.transitions.find(tr)!=s.transitions.end());
          
        }
      }
      return false;
    }

    /**
     * Compute the number of transitions.
     */
    std::size_t Automaton::countTransitions() const{
      int nb=0;
      for (auto s : states){
        nb+=s.transitions.size();
      }
      return nb;
    }


    /**
     * Print the automaton in a friendly way
     **/
    void Automaton::prettyPrint(std::ostream& os) const{      
      char str_Init[16]="";
      char str_Final[16]="";
      for(auto s : states){
        if(s.isInit){
          char str[8];
          sprintf(str,"%d ",s.nb);
          strcat(str_Init,str);
        }
        if(s.isFinal){
          char str[8];
          sprintf(str,"%d ",s.nb);
          strcat(str_Final,str);
        }
      }
      os << "\nInitial states:\n\t" << str_Init << "\nFinal states:\n\t" << str_Final << "\nTransitions:\n";
 
      for(auto s : states){
          os << "\t\tFor state " << s.nb << "\n";
        for(size_t j=0;j<alphabet.size();j++){
          os << "\t\t\tFor the letter " << alphabet[j] << ": ";
          for(auto tr : s.transitions){
            int t=-1;
            if(tr.from==s.nb && tr.symbol==alphabet[j] && tr.to!=t){
              os << " " << tr.to;
              t=tr.to;
            }
          }

          os << "\n";
        }
        
      }
    }

    /**
     * Print the automaton with respect to the DOT specification
     */
    void Automaton::dotPrint(std::ostream& os) const{
    os<<"digraph automate {\nrankdir=LR;\n";
    //final states
    std::map<int,int>::const_iterator it;
    os<<"node [shape = doublecircle];";
    for(auto s :states){
      if(s.isFinal){
        os<<s.nb<<" ";
      }
    }
    os<<";\n";
    os<<"node [shape = circle];\n";

    //initial states
    for(auto s :states){
      if(s.isInit){
        os<<"initArrow"<<s.nb<<" [label= \"\",height=0,width=0]\n";
        os<<"initArrow"<<s.nb<< " -> "<<s.nb<<"\n";
      }
    }
    os<<";\n";
    //Transitions
    for(auto s : states){
      for (auto tr : s.transitions){
        os<<tr.from<<" -> "<<tr.to<<" [label = \""<<tr.symbol<<"\"];\n";
      }
    }
    os<<"}\n";
      return;
    }


      /**
     * Tell if the automaton has one or more epsilon-transition
     **/
    bool Automaton::hasEpsilonTransition() const{
      if(!isValid()){
        return false;
      }
      for(auto s : states){
        for(auto tr : s.transitions){
          if(tr.symbol==fa::Epsilon){
            return true;
          }
        }
      }
      return false;
    }

    /**
     * Tell if the automaton is deterministic
    **/
    bool Automaton::isDeterministic() const{
      if(!isValid()){
        return false;
      }
      if(hasEpsilonTransition()){
        return false;
      }
      //int nbFinal=0;
      int nbInit=0;
      for(auto s : states){
        if(s.isInit){
          s.isInit=true;
          nbInit+=1;
          if(nbInit>1){
            return false;
          }
        }
        // if(s.isFinal){
        //   nbFinal+=1;
        // }
        for(auto l : alphabet){
          int nbletter=0;
        
          for(auto tr : s.transitions){
            if(tr.symbol==l){
              nbletter++;
              if(nbletter>1){
                return false;
              }
            }
          }
        }

      }
      // if(nbFinal<1){
      //   return false;
      // }
      if(nbInit!=1){
        return false;
      }
      return true;
    }

    /**
     * Tell if the automaton is complete
     */
    bool Automaton::isComplete() const{
      if(!isValid()){
        return false;
      }
      for(auto s : states){
        for(auto l : alphabet){
          int found=0;
          for(auto tr : s.transitions){
            if(tr.symbol==l){
              //Only if we haven't found yet the letter
              if(found==0){
                found++;
              }
            }
          }
          if(found==0){
            return false;
          }
        }
      }

      return true;
    }

    /**
     * Create a complete automaton, if not already complete
     */
    Automaton Automaton::createComplete(const Automaton& automaton){
      assert(automaton.isValid());
      if(automaton.isComplete()){
        return automaton;
      }
      Automaton copy=automaton;
      int n=(copy.states.end()->nb)+1; // Adding a trash state  : states is sorted
      copy.addState(n);
      for(auto s : copy.states){
        for(auto l : copy.alphabet){
          int found=0;
          for(auto tr : s.transitions){
            if(tr.symbol==l){
              //Only if we haven't found yet the letter
              if(found==0){
                found++;
              }
            }
          }
          if(found==0){
            copy.addTransition(s.nb,l,n);
          }
        }
      }
      
      return copy;
    }

    /**
     * Create a complement automaton
     */
    Automaton Automaton::createComplement(const Automaton& automaton){
      assert(automaton.isValid());
      Automaton copy=automaton;
      if(!copy.isDeterministic()){
        copy=createDeterministic(copy);
      }
      //test if is already complete in createComplete
      copy=createComplete(copy);
      for(auto &s : copy.states){
        if(s.isFinal){
          s.isFinal=false;
        }else{
          s.isFinal=true;
        }
      }
      return copy;

    }

    /**
     * Create a mirror automaton
     */
    Automaton Automaton::createMirror(const Automaton& automaton){
      assert(automaton.isValid());
      Automaton copy=automaton;

      for(auto s : automaton.states){
        for(auto tr : s.transitions){
          copy.removeTransition(tr.from,tr.symbol,tr.to);
          copy.addTransition(tr.to,tr.symbol,tr.from);
        }
      }
      for(auto &s : copy.states){
        if(s.isInit){
          s.isFinal=true;
          s.isInit=false;
        }else{
          if(s.isFinal){
            s.isInit=true;
            s.isFinal=false;
          }
        }
        
      }
      return copy;
    }

    /**
     * Browse the automaton to check if the language is empty
     * Or check if a state can join a final state
     */
    bool Automaton::DepthSearchEmpty(std::_Rb_tree_const_iterator<fa::Automaton::State>& s,std::set<int>& visit)const{
      visit.insert(s->nb);
      auto tr=s->transitions.begin();
      while(tr!=s->transitions.end()){
        if(tr->to==tr->from){ //if the transition is in the current state, we have already tested if the state was final.
          tr++;
          if(tr==s->transitions.end()){
            break;
          }
        }
        auto si = states.find(tr->to);
        if(si->isFinal){
          return false;
        }
        if(visit.find(si->nb)==visit.end()){
          if(DepthSearchEmpty(si,visit)==false){
            return false;
          }
        }
        tr++;
      }
      return true;
    }

    /**
     * Check if the language of the automaton is empty
     */
    bool Automaton::isLanguageEmpty() const{ 
      if(!isValid()){
        if(states.empty()){
          return true;
        }else{
          for(auto s : states){
            if (s.isInit && s.isFinal){
              return false;
            }
          }
        }
        return true;
      }
      std::set<int> visit;
      auto s=states.begin();
      while(s!=states.end()){
        if(s->isInit){
          if(s->isFinal){
            return false;
          }
          if(DepthSearchEmpty(s,visit)==false){
            return false;
          }
        }
        s++;
      }
      return true;
    }


    /**
     * Create the product of two automata
     *
     * The product of two automata accept the intersection of the two languages.
     */
    Automaton Automaton::createProduct(const Automaton& lhs, const Automaton& rhs){
      assert(lhs.isValid() && rhs.isValid());
      Automaton product;
      product.addState(0);//need a state in any case
      for(char symb : lhs.alphabet){
        if(rhs.hasSymbol(symb)){
          product.addSymbol(symb);
        }
      }
       //if lhs and rhs don't have common letter
      if(product.countSymbols()<1){
          product.addSymbol('#');
      }
      //Pour la suite, les états de l'automate 2 auront comme nombre i+size ex : l 'état 0 sera 0+size= 4 si l'automate 1 a 4 états.
      //Cela permettra de différencier les 2 automates avec leurs états avec un if(nb < size)=> automate 1 else automate 2
      std::map<int,std::vector<int>>tab;
      std::vector<int>curr1;
      std::vector<int>curr2;
      
      std::vector<int>init1;
      std::vector<int>init2;
      for(auto s : lhs.states){
        if(s.isInit){
          init1.push_back(s.nb);
        }
      }
      for(auto s : rhs.states){
        if(s.isInit){
          init2.push_back(s.nb);
        }
      }
      //if lhs or rhs don't have initial state
      if(init1.size()<1 || init2.size()<1){
        return product;
      }
      //first state will be initial (build from initial states)
      product.setStateInitial(0);
      //make the different peers with the initial states (if number of state initial = 1 by automaton => complexity=1)
      int num_index=0;
      for(size_t i=0;i<init2.size();++i){
        for(size_t i2=0;i2<init1.size();++i2){
          tab[num_index].push_back(init1[i2]);
          tab[num_index].push_back(init2[i]);
          product.addState(num_index);
          product.setStateInitial(num_index);
          num_index++;
        }
      }
      auto it=tab.begin();
      while(it!=tab.end()){
        for(size_t i=0;i<product.alphabet.size();++i){ 
          int existInLhs=0;
          int existInRhs=0;
          int nb=it->second[0];
            if(lhs.states.find(nb)==lhs.states.end()){//Never go inside, just a precaution
              //printf("\nLa ce n'est pas bon du tout\n");
              return product;
            }
            for(auto tr : lhs.states.find(nb)->transitions){
              if(tr.symbol==product.alphabet[i]){
                curr1.push_back(tr.to);
                existInLhs++;
              }
            }
            nb=it->second[1];
            if(rhs.states.find(nb)==rhs.states.end()){//Never go inside, just a precaution
              //printf("\nLa ce n'est pas bon du tout\n");
              return product;
            }
            for(auto tr : rhs.states.find(nb)->transitions){
              if(tr.symbol==product.alphabet[i]){
                curr2.push_back(tr.to);
                existInRhs++;
              }
            }
        
        
        if(existInLhs>0 && existInRhs>0){
          if(existInLhs>1 || existInRhs>1){
            std::map<int,std::vector<int>>temp;
            int ind=0;
            int ind2=0;
            for(auto c : curr1){
                for(int k=0;k<existInRhs;++k){
                  temp[ind+k].push_back(c);
                }
                ind+=existInRhs;
            }
            for(auto c :curr2){
                int tmp=ind2;
                for(int k=0;k<existInLhs;++k){
                  temp[tmp].push_back(c);
                  tmp+=existInRhs;
                }
                ind2++;
            }
            for(auto peer : temp){
              int in=0;
              int n=tab.size();
              for(auto t : tab){
                if(t.second==peer.second){
                  in=1;
                  n=t.first;
                  break;
                }
              }
              
              if(in==1){
                product.addTransition(it->first,product.alphabet[i],n);
              }else{
                tab[n]=peer.second;
                product.addState(n);
                product.addTransition(it->first,product.alphabet[i],n);
              }
            }
            temp.clear();
          }else{
            int in=0;
            int n=tab.size();
            for(auto t : tab){
              if(t.second[0]==curr1[0] && t.second[1]==curr2[0]){
                in=1;
                n=t.first;
                break;
              }
            }
            
            if(in==1){
              product.addTransition(it->first,product.alphabet[i],n);
            }else{
              tab[n].push_back(curr1[0]);
              tab[n].push_back(curr2[0]);
              product.addState(n);
              product.addTransition(it->first,product.alphabet[i],n);
            }
          }
        }          
        curr1.clear();
        curr2.clear();
      }
      it++;
    }

      //Search the final state
      for(auto e : tab){
        if(lhs.isStateFinal(e.second[0])){
          if(rhs.isStateFinal(e.second[1])){
            product.setStateFinal(e.first);
          }
        }
      }

      return product;
    }

    /**
     * Tell if the intersection with another automaton is empty
     */
   bool Automaton::hasEmptyIntersectionWith(const Automaton& other) const{
      assert(other.isValid());
      if(!isValid()){
        if(isLanguageEmpty()){
          return true;
        }else{
          for(auto s: other.states){
            if(s.isInit && s.isFinal){
              return false;
            }
          }
          return true;
        }
        return true;
      }
      std::cout<<"Product\n";
      double time=0.0;
      std::clock_t start;
      start=std::clock();

      
       Automaton product=createProduct(*this,other);
       time=(std::clock()-start)/(double) CLOCKS_PER_SEC;
      std::cout<<"temps de l'intersection : "<<time<<std::endl;
      std::cout<<"Product DONE\n";
      if(!product.isValid()){ //Never go inside
        return true;
      }
      return product.isLanguageEmpty();
    }



    /**
     * Create a deterministic automaton, if not already deterministic
     */
    Automaton Automaton::createDeterministic(const Automaton& other){
      assert(other.isValid());
      Automaton product;
      product.alphabet=other.alphabet;
      std::map<int,std::set<int>>tab;
      std::set<int>curr;
      product.addState(0);
      
      for(auto s : other.states){
        if(s.isInit){
          tab[0].insert(s.nb);
        }
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

            for(auto tr : other.states.find(s)->transitions){
                if(tr.symbol==product.alphabet[i]){
                  curr.insert(tr.to);
                }
            }
          }
          if(!curr.empty()){
            for(auto it = tab.begin();it!=tab.end();it++){
              if(it->second==curr){
                in=1;
                break;
              }
            }
            int n=-1;
            if(in==0){
              int n=tab.size();
              tab[n]=curr;
              product.addState(n);
              product.addTransition(it->first,product.alphabet[i],n);
            }else{
              for(auto snb : tab){
                if(snb.second==curr){
                  n=snb.first;
                }
              }
              product.addTransition(it->first,product.alphabet[i],n);
            }
          }
            
          
          curr.clear();
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

    /**
     * Tell if the langage accepted by the automaton is included in the
     * language accepted by the other automaton
     */
    bool Automaton::isIncludedIn(const Automaton& other) const{
      assert(other.isValid());

      if(!isValid()){
        if(isLanguageEmpty()){
          return true;
        }
      }
      Automaton copy=other;
       for(char symb : alphabet){
        if(!other.hasSymbol(symb)){
          copy.addSymbol(symb);
        }
      }
      std::cout<<"Création du complément\n";
      double time=0.0;
      std::clock_t start;
      start=std::clock(); 
      Automaton complement=createComplement(copy);
      time=(std::clock()-start)/(double) CLOCKS_PER_SEC;
      std::cout<<"temps pour la déterminisation: "<<time<<std::endl;
      std::cout<<"Création du complément DONE\n";
      //Automaton deter=createDeterministic(*this);
      return hasEmptyIntersectionWith(complement);
    }

}

using namespace std;
fa::Automaton RandomAutomaton(int nbstates){

//  ofstream fich("./CreateAutomaton.txt");
  fa::Automaton res;
  std::vector<char> letter;
  letter.push_back('a');
  letter.push_back('b');
  for(char c : letter){
    res.addSymbol(c);
  //  fich << "A2.addSymbol('"<<c<<"');"<<"\n";
  }
  for(int i=0;i<nbstates;++i){
    res.addState(i);
   // fich << "A2.addState("<<i<<");"<<"\n";
  }
  for(auto s : res.states){
    for(auto ss : res.states){
      for(char c : letter){
        double rand1 = rand() / (double)RAND_MAX;

        if(rand1 < 1.6/(nbstates)){
          res.addTransition(s.nb,c,ss.nb);
          //fich << "A2.addTransition("<<s.nb<<",'"<<c<<"',"<<ss.nb<<");"<<"\n";
        }
      }
    }
  }

  for(int i=0;i<nbstates;i++){
    double rand1 = rand() / (double)RAND_MAX;
    if(rand1 < 0.3){
      res.setStateFinal(i);
    //  fich << "A2.setStateFinal("<<i<<");"<<"\n";
    }
  }
  for(int i=1;i<nbstates;i++){
    double rand1 = rand() / (double)RAND_MAX;
    if(rand1 < 0.3){
      res.setStateInitial(i);
     // fich << "A2.setStateInitial("<<i<<");"<<"\n";
    }
  }
  res.setStateInitial(0);
  //fich<<"A2.setStateInitial(0);"<<"\n";

  return res;
}



int main(int argc, char **argv){
  srand(25);
    // fa::Automaton A1=RandomAutomaton(10);

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

      A1.setStateInitial(0);
      A1.setStateFinal(3);A1.setStateFinal(2);

      // A1.addTransition(0,'a',1);A1.addTransition(1,'a',2);
      // A1.addTransition(2,'a',1);A1.addTransition(1,'a',3);
      // A1.addTransition(0,'b',0);A1.addTransition(0,'b',3);A1.addTransition(1,'b',3);
      // A1.addTransition(3,'b',1);A1.addTransition(3,'b',2);
      if(argc>2){
      srand(atoi(argv[2]));
    }else{
      srand(time(NULL));
    }
    int nbStates=10;
    if(argc>1){
      nbStates=stoi(argv[1]);
    }
    //printf("nb : %d\n",nbStates);
  fa::Automaton A2=RandomAutomaton(nbStates);
  //A2.dotPrint(std::cout);
    if(A1.isIncludedIn(A2)){
        printf("Automaton A1 is Include in A2 : True \n");
    }else{
        printf("Automaton A1 is not Include in A2 : False \n");
    }

}