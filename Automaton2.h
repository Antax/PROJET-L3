#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>
#include <assert.h>
#include <ctype.h>
#include <cctype>
#include <string.h>
#include <bits/stdc++.h> 
#include <set>
#include <iterator>
#include <iostream>
#include <stdbool.h>
namespace fa {
  
  constexpr char Epsilon = '\0';

  class Automaton {

      struct Transition
  {
    int from;
    char symbol;
    int to;

    Transition(int from, char symbol, int to){
      this->from=from;
      this->symbol=symbol;
      this->to=to;
    }
    Transition(){
      
    }

    bool operator==(const Transition& other)const{
        return (from == other.from && symbol==other.symbol && to==other.to); 
    }
    bool operator<(const Transition& other)const{
      if(from==other.from){
        if(to==other.to){
          return symbol<other.symbol;
        }
        return to<other.to;
      }
        return from < other.from; 
    }
    
  };
      struct TransCompare
{
    bool operator () (const Transition& t1, const Transition& t2)const{
      if(t1.from==t2.from){
        if(t1.to==t2.to){
          return t1.symbol<t2.symbol;
        }
        return t1.to<t2.to;
      }
      return t1.from<t2.from;
       
    }
    
};
public:
    struct State
  {
    int nb;
    mutable bool isInit;
    mutable bool isFinal;
    mutable std::set<Transition,TransCompare>transitions;
    State(int state){
      this->nb=state;
      this->isInit=false;
      this->isFinal=false;
      
    }
    
    State(){
      this->nb=0;
      this->isInit=false;
      this->isFinal=false;
    }
    bool operator==(const State& other)const{
        return (nb == other.nb); 
    }
    bool operator<(const State& other)const{
        return nb < other.nb; 
    }
  };
  struct StateCompare
{    
    bool operator () (const State& s1, const State& s2)const{
        return s1.nb < s2.nb; 
    }
    
    
};
  
  
  
  public:
  std::vector<char>alphabet;
  std::set<State,StateCompare>states;
    /**
     * Build an empty automaton (no state, no transition).
     */
    Automaton();

    /**
     * Tell if an automaton is valid.
     *
     * A valid automaton has a non-empty set of states and a non-empty set of symbols
     */
    bool isValid() const;

    /**
     * Add a symbol to the automaton
     *
     * Epsilon is not a valid symbol.
     * Returns true if the symbol was effectively added
     */
    bool addSymbol(char symbol);

    /**
     * Remove a symbol from the automaton
     *
     * Returns true if the symbol was effectively removed
     */
    bool removeSymbol(char symbol);

    /**
     * Tell if the symbol is present in the automaton
     */
    bool hasSymbol(char symbol) const;

    /**
     * Count the number of symbols
     */
    std::size_t countSymbols() const;

    /**
     * Add a state to the automaton.
     *
     * By default, a newly added state is not initial and not final.
     * Returns true if the state was effectively added and false otherwise.
     */
    bool addState(int state);

    /**
     * Remove a state from the automaton.
     *
     * The transitions involving the state are also removed.
     * Returns true if the state was effectively removed and false otherwise.
     */
    bool removeState(int state);

    /**
     * Tell if the state is present in the automaton.
     */
    bool hasState(int state) const;

    /**
     * Compute the number of states.
     */
    std::size_t countStates() const;

    /**
     * Set the state initial.
     */
    void setStateInitial(int state);

    /**
     * Tell if the state is initial.
     */
    bool isStateInitial(int state) const;

    /**
     * Set the state final.
     */
    void setStateFinal(int state);

    /**
     * Tell if the state is final.
     */
    bool isStateFinal(int state) const;

    /**
     * Add a transition
     *
     * Returns true if the transition was effectively added and false otherwise.
     * If one of the state or the symbol does not exists, the transition is not added.
     */
    bool addTransition(int from, char alpha, int to);

    /**
     * Remove a transition
     *
     * Returns true if the transition was effectively removed and false otherwise.
     */
    bool removeTransition(int from, char alpha, int to);

    /**
     * Tell if a transition is present.
     */
    bool hasTransition(int from, char alpha, int to) const;

    /**
     * Compute the number of transitions.
     */
    std::size_t countTransitions() const;

    /**
     * Print the automaton in a friendly way
     */
    void prettyPrint(std::ostream& os) const;

    /**
     * Print the automaton with respect to the DOT specification
     */
    void dotPrint(std::ostream& os) const;

    /**
     * Tell if the automaton has one or more epsilon-transition
     */
    bool hasEpsilonTransition() const;

    /**
     * Tell if the automaton is deterministic
     */
    bool isDeterministic() const;

    /**
     * Tell if the automaton is complete
     */
    bool isComplete() const;

    /**
     * Remove non-accessible states
     */
    void removeNonAccessibleStates();

    /**
     * Remove non-co-accessible states
     */
    void removeNonCoAccessibleStates();

    /**
     * Check if the language of the automaton is empty
     */
    bool isLanguageEmpty() const;

    /**
     * Tell if the intersection with another automaton is empty
     */
    bool hasEmptyIntersectionWith(const Automaton& other) const;

    /**
     * Read the string and compute the state set after traversing the automaton
     */
    std::set<int> readString(const std::string& word) const;

    /**
     * Tell if the word is in the language accepted by the automaton
     */
    bool match(const std::string& word) const;

    /**
     * Tell if the langage accepted by the automaton is included in the
     * language accepted by the other automaton
     */
    bool isIncludedIn(const Automaton& other) const;

    /**
     * Create a mirror automaton
     */
    static Automaton createMirror(const Automaton& automaton);

    /**
     * Create a complete automaton, if not already complete
     */
    static Automaton createComplete(const Automaton& automaton);

    /**
     * Create a complement automaton
     */
    static Automaton createComplement(const Automaton& automaton);

    /**
     * Create the product of two automata
     *
     * The product of two automata accept the intersection of the two languages.
     */
    static Automaton createProduct(const Automaton& lhs, const Automaton& rhs);

    /**
     * Create a deterministic automaton, if not already deterministic
     */
    static Automaton createDeterministic(const Automaton& other);

    /**
     * Create an equivalent minimal automaton with the Moore algorithm
     */
    static Automaton createMinimalMoore(const Automaton& other);

    /**
     * Create an equivalent minimal automaton with the Brzozowski algorithm
     */
    static Automaton createMinimalBrzozowski(const Automaton& other);


  private:
    /**
     * Browse the automaton to check if the language is empty (and remove the Non-Co-accessible states)
     */
    bool DepthSearchEmpty(std::_Rb_tree_const_iterator<fa::Automaton::State>& s,std::set<int>& visit)const;
    /**
     * Browse the automaton to remove the Non-accessbile states
     */
    void DepthSearchRemove(std::_Rb_tree_const_iterator<fa::Automaton::State>& s,std::set<int>& visit);
    /**
     * Browse the automaton to read a word
     */
    void SearchWord(std::_Rb_tree_const_iterator<fa::Automaton::State> s,std::string& mot,const std::string& word,std::set<int>& way,int i)const;
  };

}

#endif // AUTOMATON_H
