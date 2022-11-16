#include "define.h"
#include "kstate.h"
#include "kedge.h"

/*********************************************************************
 Kedge implementation
**********************************************************************/

Kedge::Kedge()
{
	inUpdateModel = false;
	edge_condition = NULL;
}

Kedge::Kedge(Kedge* e)
{
	from = e->get_from();
	to = e->get_to();
	label = e->get_label();
	id = e->get_id();
	inUpdateModel = e->get_inUpdateModel();
	edge_condition = e->get_edge_condition();
}
Kedge::~Kedge()
{

}

int Kedge::get_from()
{
	return from;
}

int Kedge::get_to()
{
	return to;
}

Agent Kedge::get_label()
{
	return label;
}

int Kedge::get_id()
{
	return id;
}

bool Kedge::get_exist()
{
	return exists;
}

bool Kedge::get_inUpdateModel()
{
	return inUpdateModel;
}

Nodes* Kedge::get_edge_condition()
{
	return edge_condition;
}

void Kedge::set_from(int ks)
{
	from = ks;
}

void Kedge::set_to(int ks)
{
	to = ks;
}

void Kedge::set_label(Agent agent)
{
	label = agent;
}

void Kedge::set_id(int ident)
{
	id = ident;
}

void Kedge::set_exist(bool ex)
{
	exists = ex;
}

void Kedge::set_inUpdateModel(bool um)
{
	inUpdateModel = um;
}

void Kedge::set_edge_condition(Nodes* ec)
{
	edge_condition = ec;
}

void Kedge::print_info()
{
	if (inUpdateModel){
		cout << "This edge in an update model ";
	}
	cout << "edge id " << id << " label: " << label << " from: " << from << " to " << to;// << endl;	
	if (edge_condition != NULL){
		Nodes::iterator it;
		for (it = edge_condition->begin(); it!=edge_condition->end(); it++){
			cout << " with edge_con: ";
			it->print();
		}
	}
	cout << endl;
}

