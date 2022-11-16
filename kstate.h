#ifndef KSTATE_H
#define KSTATE_H
#include "define.h"
#include "kedge.h"
#include <map>
#include <list>


class Kripke;
class Kedge;
typedef vector<int> EdgeList; // vector of edges' id
typedef vector<Kedge> Edges ;


class Kstate{
  protected:
    Literals lits;
    int id;
	EdgeList edge_out;
	EdgeList edge_in;
	bool exists;
	// int fromState;
	// int fromEvent;
    
    map<int, set<int> > mapEdgelabelNextState; //is used to get the next states get to via the edges of label agent.
    
  public:
	Kstate();
	Kstate* copy();
	virtual ~Kstate();

	/* operators */
	//bool checkEntail1(Literal);
	//bool checkEntail2(Literals);
	//bool checkEntail3(FluentFormula);

	EdgeList get_in();
	EdgeList get_out();
	Literals get_lits();
	int get_id();
	const int get_id_const() const;
	bool get_exist();
    map<int, set<int> > get_preparedMap();
    //void set_preparedMap();
    
	void set_in(EdgeList);
	void set_out(EdgeList);
	void set_lits(Literals);
	void set_id(int);
	void set_exist(bool);
	void add_out(int);
	void add_in(int);
	void print_info();
    void add_to_map(int, int); //is used to add to mapEdgelabelNextState
    void reset_map(); // is used to resetset mapEdgelabelNextState = map<int, set<int> >();
    
    bool entail_fluform(FluentFormula) const;
    bool entail_lit(Literal) const;
    bool entail_lits(Literals) const;
	bool entail_BF(Kripke, BFNode, Agent, bool) const;

	bool entail(BFNode const & , Kripke) const;

	int loopIn();

  };

#endif
