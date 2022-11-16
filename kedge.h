#ifndef KEDGE_H
#define KEDGE_H
#include "define.h"
#include "kstate.h"


class Kstate;


class Kedge{
   protected:
	int from;
	int to;
	Agent label;
	int id;
	bool exists;
	bool inUpdateModel;
	Nodes* edge_condition;

   public:
	Kedge();
	Kedge(Kedge*);
	~Kedge();

	/* operators */
	int get_from();
	int get_to();
	Agent get_label();
	int get_id();
	bool get_exist();
	bool get_inUpdateModel();
	Nodes* get_edge_condition();
	void set_from(int);
	void set_to(int);
	void set_label(Agent);
	void set_id(int);
	void set_exist(bool);
	void set_inUpdateModel(bool);
	void set_edge_condition(Nodes*);
	
	void print_info();


};


#endif
