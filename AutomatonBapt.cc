#include "Automaton.h"

using namespace std;

namespace fa {

////////////////////////////////////
//        FONCTIONS DU TP1        //
////////////////////////////////////

  /**
   * Crée un automate
  */
  Automaton::Automaton() {
  }

  /**
   * Vérifie si un automate est valide
   * Càd s'il a bien au moins un état et une lettre dans l'alphabet
  */
  bool Automaton::isValid() const {
	if (listof_State.empty()){
		return false;
	}
	if (alphabet.empty()){
		return false;
	}
    return true;
  }

  /**
   * Ajoute un symbole à l'automate
   * _ retourne faux s'il s'agit d'un symbole non valide (epsilon, !, " ", * etc..) ou si le symbole est déja présent
   * _ vrai sinon
  */
  bool Automaton::addSymbol(char symbol) {
	if (symbol==fa::Epsilon){
		return false;
	}
	if (!isgraph(symbol)){
		return false;
	}
	if (hasSymbol(symbol)){
		return false;
	}
	
	alphabet.push_back(symbol);

    return true;
  }

  /**
   * Retire un symbole à l'automate
   * _ retourne faux s'il n'est pas valide ou n'a pas été trouvé
   * _ sinon le retire et retourne vrai (supprime également les transitions associées)
  */
  bool Automaton::removeSymbol(char symbol){
	if (symbol==fa::Epsilon){
		return false;
	}
	if (!isgraph(symbol)){
		return false;
	}

	for (size_t i=0; i<alphabet.size(); i++){
		if (alphabet.at(i)==symbol){
			alphabet.erase (alphabet.begin()+i);

			for (size_t j=0; j<listof_Transition.size(); j++){
				if (listof_Transition.at(j).letter==symbol){
					removeTransition(listof_Transition.at(j).from,symbol,listof_Transition.at(j).destination);
				}
			}
			
			return true;
		}
  	}

	return false;
  }

  /**
   * Vérifie si un symbole appartient à l'automate
  */
  bool Automaton::hasSymbol(char symbol) const{
	if (symbol==fa::Epsilon)
		return false;

	if (!isgraph(symbol))
		return false;

	for (size_t i=0; i<alphabet.size(); i++){
		if (alphabet.at(i)==symbol){
			return true;
		}
	}
	return false;
  }
  
  /**
   * Retourne le nombre de symboles présents dans l'automate
  */
  size_t Automaton::countSymbols() const{
  	return alphabet.size();
  }

  /**
   * Ajoute un état à l'automate et retourne vrai si l'ajout à fonctionné
  */
  bool Automaton::addState(int state){
	if (state<0){
		return false;
	}
	if (hasState(state)){
		return false;
	}
	State state1;
	(&state1)->state_number=state;
	(&state1)->ini=false;
	(&state1)->final=false;
	listof_State.push_back(state1);
	return true;
  }

  /**
   * Supprime un état de l'automate et ses transitions associées (sortantes et entrantes)
  */
  bool Automaton::removeState(int state){
	if (state<0){
		return false;
	}

	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){

			for(size_t j=0; j<((&listof_State.at(i))->transitions_start).size() ; j++){

				int from=        ((&listof_State.at(i))->transitions_start.at(0)).from;
				char alpha=      ((&listof_State.at(i))->transitions_start.at(0)).letter;
				int to=          ((&listof_State.at(i))->transitions_start.at(0)).destination;
				
				removeTransition(from,alpha,to);
			}

			for(size_t j=0; j<((&listof_State.at(i))->transitions_end).size() ; j++){

				int from=        ((&listof_State.at(i))->transitions_end.at(0)).from;
				char alpha=      ((&listof_State.at(i))->transitions_end.at(0)).letter;
				int to=          ((&listof_State.at(i))->transitions_end.at(0)).destination;
				
				removeTransition(from,alpha,to);
			}

			listof_State.erase (listof_State.begin()+i);
			return true;
		}
	}
	return false;
  }
  
  /**
   * Vérifie si un état appartient à l'automate
  */
  bool Automaton::hasState(int state) const{
	if (state<0){
		return false;
	}

	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){
			return true;
		}
	}
	return false;
  }

  /**
   * Compte le nombre d'états appartenant à l'automate
  */
  size_t Automaton::countStates() const{
	  return listof_State.size();
  }

  /**
   * Met l'état à initial
  */
  void Automaton::setStateInitial(int state){
	if (state<0){
		return;
	}
	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){
			(&listof_State.at(i))->ini=true;
			return;
		}
	}
  }

  /**
   * Vérifie si l'état est initial
  */
  bool Automaton::isStateInitial(int state) const{
	if (state<0){
		return false;
	}
	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){
			return ((&listof_State.at(i))->ini);
		}
	}
	return false;
  }

  /**
   * Met l'état à final
  */
  void Automaton::setStateFinal(int state){
	if (state<0){
		return;
	}
	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){
			(&listof_State.at(i))->final=true;
			return;
		}
	}
  }

  /**
   * Vérifie si l'état est final
  */
  bool Automaton::isStateFinal(int state) const{
	if (state<0){
		return false;
	}
	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==state){
			return ((&listof_State.at(i))->final);
		}
	}
	return false;
  }

  /**
   * Ajoute une transition
  */
  bool Automaton::addTransition(int from ,char alpha ,int to){

	if (!hasSymbol(alpha)){
		return false;
	}
	
	if (!hasState(from)){
		return false;
	}

	if (!hasState(to)){
		return false;
	}
	
	if (hasTransition(from,alpha,to)){
		return false;
	}
	
	Transition transition1;
	(&transition1)->letter=alpha;
	(&transition1)->from=from;
	(&transition1)->destination=to;

	bool from_valid=false;
	bool to_valid=false;

	for (size_t i=0; i<listof_State.size(); i++){
		if ((&listof_State.at(i))->state_number==from){
			(&listof_State.at(i))->transitions_start.push_back(transition1);
			from_valid=true;
		}
		if ((&listof_State.at(i))->state_number==to){
			(&listof_State.at(i))->transitions_end.push_back(transition1);
			to_valid=true;
		}
		if (from_valid==true && to_valid==true){
			break;
		}
	}

	listof_Transition.push_back(transition1);
	return true;
  }
  
  /**
   * Retire une transition
  */
  bool Automaton::removeTransition(int from ,char alpha ,int to){
	if (!hasSymbol(alpha)){
		return false;
	}
	
	for (size_t i=0; i<listof_Transition.size(); i++){
		if ((&listof_Transition.at(i))->from==from && (&listof_Transition.at(i))->letter==alpha && (&listof_Transition.at(i))->destination==to){
			listof_Transition.erase (listof_Transition.begin()+i);
			return true;
		}
	}
	return false;
  }

  /**
   * Vérifie si la transition est dans l'automate
  */
  bool Automaton::hasTransition(int from ,char alpha ,int to) const{
	if (!hasSymbol(alpha)){
		return false;
	}
	for (size_t i=0; i<listof_Transition.size(); i++){
		if ((&listof_Transition.at(i))->from==from && (&listof_Transition.at(i))->letter==alpha && (&listof_Transition.at(i))->destination==to){
			return true;
		}
	}
	return false;
  }

  /**
   * Compte le nombre de transitions dans l'automate
  */
  std::size_t Automaton::countTransitions () const{
	  return listof_Transition.size();
  }

  /**
   * Affiche les caractéristiques de l'automate
  */
  void Automaton::prettyPrint(std:: ostream& os) const{
	  os<<"Initial states:\n";
	  os<<"         ";
	  for (size_t i=0; i<listof_State.size(); i++){
	    if((&listof_State.at(i))->ini){
	  		os<<""<<(&listof_State.at(i))->state_number;
		}
	  }
	  os<<"\nFinal states:\n";
	  os<<"         ";
	  for (size_t i=0; i<listof_State.size(); i++){
	  	if((&listof_State.at(i))->final){
			os<<""<<(&listof_State.at(i))->state_number;
		}
	  }
  	  os<<"\nTransitions:";
	  for (size_t i=0; i<listof_State.size(); i++){
		  os<<"\n        For state "<<listof_State.at(i).state_number<<":";
		  for (size_t j=0; j<alphabet.size(); j++){
			  os<<"\n                For letter "<<alphabet.at(j)<<":";
			  for(size_t k=0; k<listof_State.at(i).transitions_start.size(); k++){
				  if (listof_State.at(i).transitions_start.at(k).letter==alphabet.at(j)){
					  os<<""<<listof_State.at(i).transitions_start.at(k).destination;
				  }
			  }
		  }
	  }
	  os<<"\n";
  }

  /**
   * Affiche les caractéristiques de l'automate au format .DOT
  */
  void Automaton::dotPrint(std:: ostream& os) const{

	os<<"digraph automate {\n";
	os<<"rankdir=LR;\n";

    // Création des états finaux
    os<<"node [shape = doublecircle];\n";
    for(size_t i=0; i<listof_State.size(); i++){
      if(listof_State.at(i).final){
        os<<listof_State.at(i).state_number<<"\n";
      }
    }
    os<<";\n";


    os<<"node [shape = circle];\n";
	  
	// Création des états initiaux
    for(size_t i=0; i<listof_State.size(); i++){
      if(listof_State.at(i).ini){
        os<<"initArrow_"<<listof_State.at(i).state_number<<" [label= \"\",height=0,width=0]\n";
        os<<"initArrow_"<<listof_State.at(i).state_number<< " -> "<<listof_State.at(i).state_number<<"\n";
      }
    }
    os<<";\n";


    // Création des transitions
    for(size_t i=0; i<listof_Transition.size(); i++){
        os<<listof_Transition.at(i).from<<" -> "<<listof_Transition.at(i).destination<<" [label = \""<<listof_Transition.at(i).letter<<"\"];\n";
    }
    os<<"}\n";
  }


////////////////////////////////////
//        FONCTIONS DU TP2        //
////////////////////////////////////

  /**
   * Vérifie s'il y a une epsilon-transition dans l'automate
  */
  bool Automaton::hasEpsilonTransition () const{
	for (size_t i=0; i<listof_Transition.size(); i++){
		if ((&listof_Transition.at(i))->letter=='\0'){
			return true;
		}
	}
	return false;
  }

  /**
   * Vérifie si l'automate est déterministe
   * Càd s'il a un état initial et si il y a au plus une transition d'une lettre par état
  */
  bool Automaton::isDeterministic () const{
	int k=0;
	for (size_t i=0; i<listof_State.size(); i++){
		if (isStateInitial(listof_State.at(i).state_number)){
			k++;
			if (k==2){ return false;}
		}
	}
	if (k!=1){
		return false;
	}

	vector<char> vect; 

	for (size_t i=0; i<listof_State.size(); i++){
		vector<char> vect;

		// On met dans vector toutes les transitions qui démarrent
		for (size_t j=0; j<((&listof_State.at(i))->transitions_start).size() ; j++){
			vect.push_back(   (((&listof_State.at(i))->transitions_start).at(j)).letter);
		}

		// S'il y a deux fois la même lettre -> pas déterministe
		for (size_t k=0; k<vect.size();k++){
			for (size_t l=0; l<vect.size();l++){
				if (vect.at(k)==vect.at(l) && k!=l){
					return false;
				}
			}
		}
	}
	return true;
  }

  /**
   * Vérifie si l'automate est complet
  */
  bool Automaton::isComplete () const{
	  if (!isValid()){
		  return false;
	  }

	  for (size_t i=0; i<listof_State.size(); i++){

		vector<char> vect; 
		// On met dans vector toutes les transitions qui démarrent (sans doublons)
		for (size_t j=0; j<((&listof_State.at(i))->transitions_start).size() ; j++){

			if (find(vect.begin(), vect.end(),(((&listof_State.at(i))->transitions_start).at(j)).letter)==vect.end()){
				vect.push_back(   (((&listof_State.at(i))->transitions_start).at(j)).letter);
			}

		}

		size_t count=0;
		// Trouver une lettre de l'alphabet qui n'est pas dans vect
		for (size_t k=0; k<alphabet.size(); k++){
			for (size_t l=0; l<vect.size();l++){
				if (alphabet.at(k)==vect.at(l)){
					count++;
					break;
				}
			}
		}
		if (count!=alphabet.size()){
			return false;
		}

	  }
	  return true;
  }

  
  /**
   * Crée et renvoie un automate complet
  */
  Automaton Automaton::createComplete (const Automaton &automaton){
    Automaton res=Automaton();
	if (!automaton.isValid()){
		res.addState(0);
		res.addSymbol('a');
		return res;
	}

	// On ajoute son alphabet
	for (size_t i=0 ; i<automaton.alphabet.size(); i++){
		res.addSymbol(automaton.alphabet.at(i));
	}

	// On ajoute ses états
	for (size_t i=0 ; i<automaton.listof_State.size(); i++){
		res.addState(automaton.listof_State.at(i).state_number);
		if (automaton.listof_State.at(i).ini){
			res.setStateInitial(automaton.listof_State.at(i).state_number);
		}
		if (automaton.listof_State.at(i).final){
			res.setStateFinal(automaton.listof_State.at(i).state_number);
		}
	}

	// On ajoute ses transitions
	for (size_t i=0 ; i<automaton.listof_Transition.size(); i++){
		res.addTransition(automaton.listof_Transition.at(i).from,automaton.listof_Transition.at(i).letter,automaton.listof_Transition.at(i).destination);
	}

	if (res.isComplete()){
		return res;
	}

	// Création de l'état puits
	int new_state=0;
    while(res.hasState(new_state)){
      ++new_state;
    }
	res.addState(new_state);

	// Ajout de toutes les transitions sur lui-même
	for (size_t i=0 ; i<automaton.alphabet.size(); i++){
		res.addTransition(new_state,automaton.alphabet.at(i),new_state);
	}


	for (size_t i=0; i<res.listof_State.size(); i++){

		vector<char> vect; 
		// On met dans vector toutes les transitions qui démarrent
		for (size_t j=0; j<((&res.listof_State.at(i))->transitions_start).size() ; j++){
			vect.push_back(   (((&res.listof_State.at(i))->transitions_start).at(j)).letter);
		}

		// Trouver une lettre de l'alphabet qui n'est pas dans vect
		for (size_t k=0; k<res.alphabet.size(); k++){
			bool ok =false;
			for (size_t l=0; l<vect.size();l++){
				if (res.alphabet.at(k)==vect.at(l)){
					ok=true;
					break;
				}
			}
			if (ok==false){
				res.addTransition((&res.listof_State.at(i))->state_number,res.alphabet.at(k),new_state);
			}
		}

	}


	return res;
  }
  
  /**
   * Crée et renvoie un automate complémentaire
  */
  Automaton Automaton::createComplement(const Automaton &automaton){
	  //Déterminisation
	  Automaton determinise = createDeterministic(automaton);

	  //Completion
	  Automaton res = createComplete(determinise);

	  //Inversement des états finaux et non finaux
	  for (size_t i=0; i<res.listof_State.size(); i++){
		  if (res.listof_State.at(i).final==true){
			  res.listof_State.at(i).final=false;
		  }else{
			  res.listof_State.at(i).final=true;
		  }
	  }

	  return res;
	  
  }


  /**
   * Crée et renvoie un automate miroir
  */
  Automaton Automaton::createMirror(const Automaton &automaton){
	Automaton res=Automaton();

    for (size_t i=0 ; i<automaton.alphabet.size(); i++){
		res.addSymbol(automaton.alphabet.at(i));
	}

	for (size_t i=0 ; i<automaton.listof_State.size(); i++){
		res.addState(automaton.listof_State.at(i).state_number);
		if (automaton.listof_State.at(i).ini){
			res.setStateFinal(automaton.listof_State.at(i).state_number);
		}
		if (automaton.listof_State.at(i).final){
			res.setStateInitial(automaton.listof_State.at(i).state_number);
		}
	}

	for (size_t i=0 ; i<automaton.listof_Transition.size(); i++){
		res.addTransition(automaton.listof_Transition.at(i).destination,automaton.listof_Transition.at(i).letter,automaton.listof_Transition.at(i).from);
	}

	return res;
  }



  ////////////////////////////////////
  //        FONCTIONS DU TP3        //
  ////////////////////////////////////

  /**
   * Renvoie les états accessibles dans un tableau à partir d'un tableau d'états en paramètre
  */
  std::vector<State> Automaton::getAccessibleStates(std::vector<State> states) const{
	  vector<State> vect;

	  // Pour chaque état en entrée :
	  for (size_t i=0; i<states.size();i++){

		  // Pour chaque transition de l'automate
		  for (size_t j=0; j<listof_Transition.size();j++){
			  if (listof_Transition.at(j).from==states.at(i).state_number){
				  for (size_t k=0; k<listof_State.size(); k++){
					  if (listof_State.at(k).state_number==listof_Transition.at(j).destination){
						  vect.push_back(listof_State.at(k));
					  }
				  }
			  }
		  }


	  }
	  return vect;
  }

  /**
   * Renvoie les états inversement accessibles dans un tableau à partir d'un tableau d'états en paramètre
  */
  std::vector<State> Automaton::getAccessibleStatesReverse(std::vector<State> states) const{
	  vector<State> vect;

	  // Pour chaque état en entrée :
	  for (size_t i=0; i<states.size();i++){

		  // Pour chaque transition de l'automate
		  for (size_t j=0; j<listof_Transition.size();j++){
			  if (listof_Transition.at(j).destination==states.at(i).state_number){
				  for (size_t k=0; k<listof_State.size(); k++){
					  if (listof_State.at(k).state_number==listof_Transition.at(j).from){
						  vect.push_back(listof_State.at(k));
					  }
				  }
			  }
		  }


	  }
	  return vect;
  }



  /**
   * Vérifie si l'automate correspond au langage vide
  */
  bool Automaton::isLanguageEmpty() const{

	//Aucun état initial ou aucun état final
	int countini=0;
	int countfinal=0;
	for (size_t i=0 ; i<listof_State.size(); i++){
		if (listof_State.at(i).ini){
			countini++;
		}
		if (listof_State.at(i).final){
			countfinal++;
		}
	}
	if (countini==0 || countfinal==0){
		return true;
	}

	for (size_t i=0 ; i<listof_State.size(); i++){
		vector<State> vect;

		// Pour chaque état initial :
		if (listof_State.at(i).ini){

			if (listof_State.at(i).final){
				return false;
			}

			vect.push_back(listof_State.at(i));

			// Pour chaque état de l'automate
			// Cela donne une limite d'itérations, la profondeur ne pouvant dépasser la taille
			for (size_t k=0 ; k<listof_State.size(); k++){
				vect=getAccessibleStates(vect);

				if (vect.size()==0){
					break;
				}

				for(size_t j=0 ; j<vect.size(); j++){
					if (vect.at(j).final){
						return false;
					}
				}
			}
		}



	}

	return true;



  }

  /**
   * Supprime les états non-accessibles -> peut rendre l'automate invalide
  */
  void Automaton::removeNonAccessibleStates(){
	// Tableau des états à ne pas supprimer
	vector<State> aconserver;

	for (size_t i=0 ; i<listof_State.size(); i++){
		vector<State> vect; 


		// Pour chaque état initial :
		if (listof_State.at(i).ini){
			vect.push_back(listof_State.at(i));
			aconserver.push_back(listof_State.at(i));

			// Pour chaque état de l'automate
			// Cela donne une limite d'itérations, la profondeur ne pouvant dépasser la taille
			for (size_t k=0 ; k<listof_State.size(); k++){

				vect=getAccessibleStates(vect);

				for (size_t j=0; j<vect.size(); j++){
					aconserver.push_back(vect.at(j));
				}

			}
		}

	}

	vector<State> asuppr; 
	//Suppression des états à ne pas conserver
	for (size_t i=0 ; i<listof_State.size(); i++){
		bool ok =false;
		for (size_t j=0 ; j<aconserver.size(); j++){
			if ((&listof_State.at(i))->state_number == (&aconserver.at(j))->state_number){
				ok=true;
			}
		}
		if (ok==false){
			asuppr.push_back(listof_State.at(i));
			//removeState((&listof_State.at(i))->state_number);
		}
	}

	for (size_t i=0 ; i<asuppr.size(); i++){
		removeState((&asuppr.at(i))->state_number);
	}

  }


  /**
   * Supprime les états non-Coaccessibles -> peut rendre l'automate invalide
  */
  void Automaton::removeNonCoAccessibleStates(){
	// Tableau des états à ne pas supprimer
	vector<State> aconserver;

	for (size_t i=0 ; i<listof_State.size(); i++){
		vector<State> vect; 


		// Pour chaque état final :
		if (listof_State.at(i).final){
			vect.push_back(listof_State.at(i));
			aconserver.push_back(listof_State.at(i));

			// Pour chaque état de l'automate
			// Cela donne une limite d'itérations, la profondeur ne pouvant dépasser la taille
			for (size_t k=0 ; k<listof_State.size(); k++){

				//vect=getAccessibleStates(vect);
				vect=getAccessibleStatesReverse(vect);


				for (size_t j=0; j<vect.size(); j++){
					aconserver.push_back(vect.at(j));
				}

			}
		}

	}

	vector<State> asuppr; 
	//Suppression des états à ne pas conserver
	for (size_t i=0 ; i<listof_State.size(); i++){
		bool ok =false;
		for (size_t j=0 ; j<aconserver.size(); j++){
			if ((&listof_State.at(i))->state_number == (&aconserver.at(j))->state_number){
				ok=true;
			}
		}
		if (ok==false){
			asuppr.push_back(listof_State.at(i));
			//removeState((&listof_State.at(i))->state_number);
		}
	}


	for (size_t i=0 ; i<asuppr.size(); i++){
		removeState((&asuppr.at(i))->state_number);
	}

  }


////////////////////////////////////
//        FONCTIONS DU TP4        //
////////////////////////////////////

// Cette méthode est propre au createProduct
  std::vector<vector<int>> Automaton::getDoubleAccessibleStatesForALetter(Automaton lhs, Automaton rhs, std::vector<vector<int>> states, char letter){
	vector<vector<int>> res;

	bool l=false;
	bool r=false;

	set<int> dejamis1;
	set<int> dejamis2;

	for (size_t i=0; i< states.size(); i++){
		  // D'abord pour L
		  if (states.at(i).at(0)==1){
			  for (size_t j=0; j<lhs.listof_Transition.size(); j++){
				  if (lhs.listof_Transition.at(j).from==states.at(i).at(1) && lhs.listof_Transition.at(j).letter==letter){
					  vector<int> newres;
					  newres.push_back(1);
					  newres.push_back(lhs.listof_Transition.at(j).destination);
					  size_t size = dejamis1.size();
					  dejamis1.insert(lhs.listof_Transition.at(j).destination);
					  if (size!=dejamis1.size()){
						res.push_back(newres);
						l=true;
					  }
				  }
			  }
		  }

	      // Ensuite pour R
		  if (states.at(i).at(0)==2){
			  	for (size_t j=0; j<rhs.listof_Transition.size(); j++){
				  if (rhs.listof_Transition.at(j).from==states.at(i).at(1) && rhs.listof_Transition.at(j).letter==letter){
					  vector<int> newres;
					  newres.push_back(2);
					  newres.push_back(rhs.listof_Transition.at(j).destination);
					  size_t size = dejamis2.size();
					  dejamis2.insert(rhs.listof_Transition.at(j).destination);
					  if (size!=dejamis2.size()){
						res.push_back(newres);
						r=true;
					  }
				  }
			  }
		  }

	}

	if (l==false || r==false){
		vector<vector<int>> emptyres;
		return emptyres;
	}

	return res;
  }
  
  /**
  * Crée un automate produit à partir de deux automates
  */
  Automaton  Automaton::createProduct(const Automaton& lhs, const Automaton& rhs){
	  Automaton res=Automaton();

	  // On attribue l'alphabet à res
	  for(size_t i=0; i< lhs.alphabet.size(); i++){
		  res.alphabet.push_back(lhs.alphabet.at(i));
	  }

	  // On crée le tableau s0 composé de l'ensemble des états initiaux
	  vector<vector<int>> s0;

	  bool isFinal=true;
	  bool isEmpty=true;

	  for (size_t i=0; i<lhs.listof_State.size(); i++){
		if ((&lhs.listof_State.at(i))->ini){
			vector<int> newres;
			newres.push_back(1);
			newres.push_back((&lhs.listof_State.at(i))->state_number);
			s0.push_back(newres);

			if (!(&lhs.listof_State.at(i))->final){
				isFinal=false;
			}
			isEmpty=false;
		}
	  }
	  for (size_t i=0; i<rhs.listof_State.size(); i++){
		if ((&rhs.listof_State.at(i))->ini){
			vector<int> newres;
			newres.push_back(2);
			newres.push_back((&rhs.listof_State.at(i))->state_number);
			s0.push_back(newres);

			if (!(&lhs.listof_State.at(i))->final){
				isFinal=false;
			}
			isEmpty=false;
		}
	  }

	  // On crée un nouvel état s0
	  res.addState(0);
	  if (!isEmpty){
		res.setStateInitial(0);
	  }
	  if (isFinal){
		  res.setStateFinal(0);
	  }

	  // On crée la première colonne du tableau, la première ligne étant s0
	  vector<vector<vector<int>>> list_groups;
	  list_groups.push_back(s0);

	  //pour chaque element du groupe : (celui ci s'agrandit, au départ il n'y a que s0)
	  for (size_t i=0; i< list_groups.size(); i++){
		
		// pour chaque lettre de l'alphabet (càd une nouvelle colonne : a, b, c ....)
		for(size_t j=0; j<res.alphabet.size(); j++){
			
			// on crée un tableau des états accessibles sXY depuis l'élément du groupe (s0 ou s1 ou s2 etc) pour une lettre donnée où
			// X = 0,1,2,3 etc..
			// Y = a, b, c etc..
			vector<vector<int>> sXY = getDoubleAccessibleStatesForALetter(lhs,rhs,list_groups.at(i),res.alphabet.at(j));

			if (sXY.size()!=0){

				bool fin=true;
				for (size_t x=0; x<sXY.size(); x++){
					if (!lhs.isStateFinal(sXY.at(x).at(1)) && sXY.at(x).at(0)==1){
						fin=false;
					}

					if (!rhs.isStateFinal(sXY.at(x).at(1)) && sXY.at(x).at(0)==2){
						fin=false;
					}
				}
				// On vérifie si SXY appartient à un élément de la liste

				//Pour chaque élément du groupe on définit un booléen : s'il est à vrai (on a trouvé le groupe), on renvoie l'index
				int index=-1;
				for (size_t k=0; k< list_groups.size(); k++){

					bool trouve=false;

					// S'ils n'ont pas la même taille on passe directement à l'élément suivant
					if (list_groups.at(k).size()==sXY.size()){

						// Pour chaque élément du sous-groupe
						for(size_t l=0; l< list_groups.at(k).size(); l++){

							bool okok=false;

							// S'il y en a un qu'on reconnait on retourne true
							for(size_t m=0; m< sXY.size(); m++){
								if (sXY.at(m).at(0)==list_groups.at(k).at(l).at(0) && sXY.at(m).at(1)==list_groups.at(k).at(l).at(1)){
									okok=true;
									break;
								}									
							}	
							//
							if (okok==false){
								trouve=false;
								break;
							}
							trouve=true;

						}
					}
					if (trouve){
						index=k;
						break;
					}
				}

				if (index!=-1){
					res.addTransition(i,res.alphabet.at(j),index);
				}else{
					// On crée un nouvel état s(X+1)
					res.addState(list_groups.size());
					if (fin){
						res.setStateFinal(list_groups.size());
					}
					res.addTransition(i,res.alphabet.at(j),list_groups.size());
					list_groups.push_back(sXY);
				}

			}

		}

	  }

	  return res;

  }

  /**
  * Vérifie si le produit de l'automate courant avec celui du paramètre engendre un langage vide
  */
  bool Automaton::hasEmptyIntersectionWith(const Automaton& other) const{
	Automaton res = createProduct(*this,other);
	return res.isLanguageEmpty();
  }


  ////////////////////////////////////
  //        FONCTIONS DU TP5        //
  ////////////////////////////////////

  std::vector<int> Automaton::getAccessibleStatesForALetter(std::vector<int> states, char letter) const{
	set<int> res;
	for (size_t i=0; i< states.size(); i++){
		for (size_t j=0; j<listof_Transition.size(); j++){
			if (listof_Transition.at(j).from==states.at(i) && listof_Transition.at(j).letter==letter){
				res.insert((&listof_Transition.at(j))->destination);
			}
		}
	}
	std::vector<int> v(res.begin(), res.end());
	return v;
  }
  

  /**
   * Renvoie les états sur lesquels ont dérivé le mot, ou l'ensemble vide si le mot n'est pas fini
  */
  std::set<int>  Automaton::readString(const std:: string& word) const{
	vector<int> tab;

	for (size_t i=0; i<listof_State.size(); i++){
	  if ((&listof_State.at(i))->ini){
		tab.push_back((&listof_State.at(i))->state_number);
	  }
	}

	for(size_t i=0; i<word.length();i++){
	  tab = getAccessibleStatesForALetter(tab,word.at(i));
	}

	vector<int> newtab;

	for (size_t i=0; i<tab.size(); i++){
		if (isStateFinal(tab.at(i))){
			newtab.push_back(tab.at(i));
		}
	}


	std::set<int> set;
    std::copy(newtab.begin(),
            newtab.end(),
            std::inserter(set, set.end()));

	return set;
  }


  /**
   * Vérifie si l'automate reconnaît le mot entré en paramètre
  */
  bool  Automaton::match(const std:: string& word) const{

	// On obtient la liste d'états initiaux
	vector<int> tab;
	for (size_t i=0; i<listof_State.size(); i++){
	  if ((&listof_State.at(i))->ini){
		tab.push_back((&listof_State.at(i))->state_number);
	  }
	}

	// On itere un getAccessibleStatesForALetter
	for(size_t i=0; i<word.length();i++){
	  tab = getAccessibleStatesForALetter(tab,word.at(i));
	}

	// On vérifie a la dernière itération qu'on se trouve bien dans un état final
	for (size_t i=0; i<tab.size(); i++){
		if (isStateFinal(tab.at(i))){
			return true;
		}
	}

	return false;
  }

  
  /**
   * Créer un automate déterministe
  */
  Automaton  Automaton::createDeterministic(const Automaton&automaton){
	Automaton res = Automaton();

	if (!automaton.isValid()){
		res.addState(0);
		res.addSymbol('a');
		return res;
	}

	// On attribue l'alphabet à res
    for(size_t i=0; i< automaton.alphabet.size(); i++){
	  res.alphabet.push_back(automaton.alphabet.at(i));
	}

	// On obtient la liste d'états initiaux
	vector<int> s0;
	bool final=false;

	for (size_t i=0; i<automaton.listof_State.size(); i++){
	  if ((&automaton.listof_State.at(i))->ini){
		s0.push_back((&automaton.listof_State.at(i))->state_number);
		if ((&automaton.listof_State.at(i))->final){
			final=true;
		}
	  }
	}

	// On crée un nouvel état s0
	res.addState(0);
	res.setStateInitial(0);
	if (final){
		res.setStateFinal(0);
	}

	// On le met dans la liste des groupes
	vector<vector<int>> list_groups;
	list_groups.push_back(s0);

	//pour chaque element du groupe : (celui ci s'agrandit, au départ il n'y a que s0)
	for (size_t i=0; i< list_groups.size(); i++){

      // pour chaque lettre de l'alphabet (càd une nouvelle colonne : a, b, c ....)
	  for(size_t j=0; j<res.alphabet.size(); j++){
			
		// on crée un tableau des états accessibles sXY depuis l'élément du groupe (s0 ou s1 ou s2 etc) pour une lettre donnée où
		// X = 0,1,2,3 etc..
		// Y = a, b, c etc..
		vector<int> sXY = automaton.getAccessibleStatesForALetter(list_groups.at(i),res.alphabet.at(j));

		if (sXY.size()!=0){

			bool fin=false;
			for (size_t x=0; x<sXY.size(); x++){
				if (automaton.isStateFinal(sXY.at(x))){
					fin=true;
				}
			}
			// On vérifie si SXY appartient à un élément de la liste

			//Pour chaque élément du groupe on définit un booléen : s'il est à vrai (on a trouvé le groupe), on renvoie l'index
			int index=-1;
			for (size_t k=0; k< list_groups.size(); k++){

				bool trouve=false;

				// S'ils n'ont pas la même taille on passe directement à l'élément suivant
				if (list_groups.at(k).size()==sXY.size()){

					// Pour chaque élément du sous-groupe
					for(size_t l=0; l< list_groups.at(k).size(); l++){

						bool okok=false;

						// S'il y en a un qu'on reconnait on retourne true
						for(size_t m=0; m< sXY.size(); m++){
							if (sXY.at(m)==list_groups.at(k).at(l)){
								okok=true;
								break;
							}									
						}	
						//
						if (okok==false){
							trouve=false;
							break;
						}
						trouve=true;

					}
				}
				if (trouve){
					index=k;
					break;
				}
			}

			
			if (index!=-1){
				res.addTransition(i,res.alphabet.at(j),index);
			}else{
				// On crée un nouvel état s(X+1)
				res.addState(list_groups.size());
				if (fin){
					res.setStateFinal(list_groups.size());
				}
				res.addTransition(i,res.alphabet.at(j),list_groups.size());
				list_groups.push_back(sXY);
				}

			}

		}

	  }

	return res;
  }
  
  /**
   * Vérifie si l'automate courant est inclus dans l'autre automate passé en paramètre
  */
  bool Automaton::isIncludedIn(const Automaton& other) const{
	Automaton complet = createComplement(other);
	
	Automaton produit = createProduct(*this, complet);
	if (produit.isLanguageEmpty()){
		return true;
	}
	
	return false;
  }

  ////////////////////////////////////
  //        FONCTIONS DU TP6        //
  ////////////////////////////////////

  /**
   * Crée un automate minimisé selon l'algorithme de Moore
  */
  Automaton  Automaton::createMinimalMoore (const Automaton&automaton){

	// On déterminise 
	Automaton determinise = createDeterministic(automaton);

	// On le rend complet
	Automaton res = createComplete(determinise);

	// On fait un tableau de tableau (nombre d'états X (nombre de lettres + 2) )
	vector<vector<int>> tab;
	vector<int> lasttab;

	// On complète les sous éléments du tableau
	for (size_t i=0; i<res.listof_State.size(); i++){

		vector<int> newtab;
		newtab.push_back((&res.listof_State.at(i))->state_number);

		if (res.listof_State.at(0).final==false){
			if (res.listof_State.at(i).final==true){
				newtab.push_back(2);
				lasttab.push_back(2);
			}else{
				newtab.push_back(1);
				lasttab.push_back(1);
			}
		}else{
			if (res.listof_State.at(i).final==true){
				newtab.push_back(1);
				lasttab.push_back(1);
			}else{
				newtab.push_back(2);
				lasttab.push_back(2);
			}
		}

		tab.push_back(newtab);
	}

	bool firsttour=true;
	// Prochaine étape
	while (1){
		// Pour chaque état
		for (size_t j=0; j<res.listof_State.size(); j++){

			// Pour chaque symbole
			for (size_t k=0; k<res.alphabet.size(); k++){
				vector<int> unique;
				unique.push_back(res.listof_State.at(j).state_number);
				vector<int> result = res.getAccessibleStatesForALetter(unique,res.alphabet.at(k));

				for (size_t l=0; l<tab.size(); l++){
					if (result.size()!=0){
						if (tab.at(l).at(0)==result.at(0)){

							if (!firsttour){
								tab.at(j).at(k+2)=tab.at(l).at(1);
							}else{
								tab.at(j).push_back(tab.at(l).at(1));
							}

						}
					}
				}
			}

		}

		// On compare les éléments du tableau
		vector<bool> tabflag;
		for (size_t j=0; j<tab.size(); j++){
			tabflag.push_back(false);
		}

		int count=1;

		for (size_t j=0; j<tab.size(); j++){
			vector<int> aremplacer;
			if (tabflag.at(j)){
				continue;
			}
			for (size_t k=0; k<tab.size(); k++){
				bool valid=true;
				for (size_t l=1; l<=res.alphabet.size()+1; l++){
					if (tab.at(j).at(l)!=tab.at(k).at(l)){
						valid=false;
						break;
					}
				}
				if (valid && tabflag.at(k)==false){
					aremplacer.push_back(k);
					tabflag.at(k)=true;
				}
			}
			for (size_t k=0; k<aremplacer.size(); k++){
				tab.at(aremplacer.at(k)).at(1)=count;
			}
			count++;
		}

		bool same=true;
		for (size_t j=0; j<tab.size(); j++){
			if (tab.at(j).at(1)!=lasttab.at(j)){
				same=false;
				break;
			}
		}

		if (same==true){
			break;
		}

		for (size_t j=0; j<tab.size(); j++){
			lasttab.at(j)=tab.at(j).at(1);
		}

	    firsttour=false;

    }
	// On recommence

	// On construit l'automate à partir du tableau actuel
	Automaton finalres = Automaton();

	// On attribue l'alphabet à finalres
    for(size_t i=0; i< automaton.alphabet.size(); i++){
	  finalres.alphabet.push_back(automaton.alphabet.at(i));
	}

	for (size_t i=0; i<lasttab.size(); i++){
		if (finalres.addState(lasttab.at(i))){
			for (size_t j=0; j<res.listof_State.size(); j++){
				if (tab.at(i).at(0)==res.listof_State.at(j).state_number && res.listof_State.at(j).ini){
					finalres.setStateInitial(lasttab.at(i));
				}
				if (tab.at(i).at(0)==res.listof_State.at(j).state_number && res.listof_State.at(j).final){
					finalres.setStateFinal(lasttab.at(i));
				}
			}
		}
	}

    for(size_t i=0; i<tab.size(); i++){
	  for (size_t j=0; j<finalres.alphabet.size(); j++){
		  finalres.addTransition(tab.at(i).at(1),finalres.alphabet.at(j),tab.at(i).at(j+2));
	  }
	}

	return finalres;
  }

  /**
   * Crée un automate minimisé selon l'algorithme de Brzozowski
  */
  Automaton  Automaton::createMinimalBrzozowski (const Automaton& automaton){
	Automaton res = Automaton();

	// On ajoute son alphabet
	for (size_t i=0 ; i<automaton.alphabet.size(); i++){
		res.addSymbol(automaton.alphabet.at(i));
	}

	// On ajoute ses états
	for (size_t i=0 ; i<automaton.listof_State.size(); i++){
		res.addState(automaton.listof_State.at(i).state_number);
		if (automaton.listof_State.at(i).ini){
			res.setStateInitial(automaton.listof_State.at(i).state_number);
		}
		if (automaton.listof_State.at(i).final){
			res.setStateFinal(automaton.listof_State.at(i).state_number);
		}
	}

	// On ajoute ses transitions
	for (size_t i=0 ; i<automaton.listof_Transition.size(); i++){
		res.addTransition(automaton.listof_Transition.at(i).from,automaton.listof_Transition.at(i).letter,automaton.listof_Transition.at(i).destination);
	}

	// Emondation
	res.removeNonAccessibleStates();
	res.removeNonCoAccessibleStates();

	// Déterminisation de son miroir
	Automaton miroir = createMirror(res);
	Automaton determiniseMiroir = createDeterministic(miroir);

	// Retournement
	Automaton miroir2 = createMirror(determiniseMiroir);

	// Déterminisation
	res = createDeterministic(miroir2);

	return res;
  }


  /**
   * Crée un automate minimisé selon l'algorithme de Hopcroft (non réalisé)
  */
  Automaton Automaton::createMinimalHopcroft(const Automaton& automaton){
	  Automaton res = Automaton();
	  if (!automaton.isValid()){
		  return res;
	  }
	  return res;
  }

  /**
   * Crée un automate équivalent dépourvu de ses epsilon-transition (non réalisé)
  */
  Automaton Automaton::createWithoutEpsilon(const Automaton&automaton){
	  Automaton res = Automaton();
	  if (!automaton.isValid()){
		  return res;
	  }
	  return res;
  }


}

