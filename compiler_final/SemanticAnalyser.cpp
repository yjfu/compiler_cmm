//
// Created by ������ on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "SemanticAnalyser.h"

void SemanticAnalyser::process_node(node* n) {
	bool success = 1;
	switch (n->symbol.type) {
	case n_program: {
		this->process_program(n);
		break;
	}
	case n_fun_decl_list: {
		this->process_n_fun_decl_list(n);
		break;
	}
	case n_fun_decl: {
		this->process_n_fun_decl(n);
		break;
	}
	case n_para_list: {
		this->process_n_para_list(n);
		break;
	}
	case n_para_dec: {
		this->process_n_para_dec(n);
		break;
	}
	case n_para_list_q: {
		this->process_n_para_list_q(n);
		break;
	}
	case n_stmt_list: {
		this->process_n_stmt_list(n);
		break;
	}
	case n_decl_stmt: {
		this->process_n_decl_stmt(n);
		break;
	}
	case n_call_stmt: {
		this->process_n_call_stmt(n);
		break;
	}
	case n_read_stmt: {
		this->process_n_read_stmt(n);
		break;
	}
	case n_write_stmt: {
		this->process_n_write_stmt(n);
		break;
	}
	case n_ret_stmt: {
		this->process_n_ret_stmt(n);
		break;
	}
	case n_if_stmt: {
		this->process_n_if_stmt(n);
		break;
	}
	case n_while_stmt: {
		this->process_n_while_stmt(n);
		break;
	}
	case n_exp_stmt: {
		this->process_n_exp_stmt(n);
		break;
	}
	}
}
void SemanticAnalyser::process_program(node *n) {
	this->process_node(n->sub_node[0]);
}
void SemanticAnalyser::process_n_fun_decl_list(node *n) {
	vector<node*>& subn = (n->sub_node);
	for (int i = 0; i<subn.size(); i++) {
		//����n_fun_decl
		process_node(subn[i]);
	}
	return;
}
void SemanticAnalyser::process_n_fun_decl(node *n) {
	int type = n->sub_node[0]->sub_node[0]->symbol.type;//ȷ��������������
	string id = n->sub_node[1]->symbol.contain;//ȷ������������
											   //�����������������ű�this->id_table��ȥ
	if (this->id_table[0].table.count(id)) {
		//�Ѿ��������������
		//printf("�ú��� %s �ѱ�����!\n",id.c_str());
		this->wp->process(decleared_function, n->sub_node[1]);
		error = 1;
		return;
	}

	this->id_table[0].table[id].name = n->sub_node[1]->symbol.contain;
	this->id_table[0].table[id].addr = this->code_point;
	if (this->id_table[0].table[id].name == "main")
		this->start_point = this->code_point;//�������
	this->id_table[0].table[id].type = a_function;
	this->id_table[0].table[id].return_type = type;
	this->id_table[0].order.push_back(id);
	//���������ķ��ű�
	this->id_table.push_back(IDTable());
	//��������n_para_list
	process_node(n->sub_node[3]);
	//�������n_stmt_list
	process_node(n->sub_node[6]);
	//��������������м����,�����ܷ�return
	if (!must_can_return(this->id_table[0].table[id].addr)) {
		//��һ�����Է���,����
		this->wp->process(function_may_cannot_return, n->sub_node[1]);
		this->error = 1;
		//��������
	}
	//������˺���,�˺������ű��ջ
	this->id_table.pop_back();

}
void SemanticAnalyser::process_n_para_list(node *n) {
	if (n->sub_node[0]->symbol.type == s_null) {
		//��������б�Ϊ��
		return;
	}
	//����n_para_dec
	process_node(n->sub_node[0]);
	//����n_para_list_q
	process_node(n->sub_node[1]);
}
void SemanticAnalyser::process_n_para_dec(node *n) {
	mID id;
	//�鿴��������
	if (n->sub_node[0]->sub_node[0]->symbol.type == s_int)
		id.type = a_int_var;
	else
		id.type = a_real_var;
	id.name = n->sub_node[1]->symbol.contain;
	id.value = 0;
	//���ں�����
	string func_name = this->id_table[0].order[this->id_table[0].order.size() - 1];
	//�ж��Ƿ����ں������Ƿ���ͬ������
	bool have_same = 0;
	for (int i = 0; i<this->id_table[0].table[func_name].para_list.size(); i++) {
		if (id.name == this->id_table[0].table[func_name].para_list[i].name) {
			have_same = 1;
			break;
		}
	}
	if (have_same) {
		this->wp->process(have_same_para, n->sub_node[1]);
		this->error = 1;
		return;
	}
	//���û��,���뵽�����Ĳ���������ȥ
	this->id_table[0].table[func_name].para_list.push_back(id);
	//���������뵽�����ķ��ű���
	this->id_table[1].table[id.name].addr = id_table[1].number_of_id;
	id_table[1].number_of_id++;
}
void SemanticAnalyser::process_n_para_list_q(node *n) {
	if (n->sub_node[0]->symbol.type == s_null) {
		//��������б�Ϊ��
		return;
	}
	//����n_para_dec,����sub_node[0]�Ƕ���
	process_node(n->sub_node[1]);
	//����n_para_list_q
	process_node(n->sub_node[2]);
}
void SemanticAnalyser::process_n_stmt_list(node *n) {
	if (n->sub_node[0]->symbol.type == s_null) {
		//�������б��ǿ�
		return;
	}
	//����n_stmt��Ψһ���ӽڵ�,Ҳ����n_decl_stmt,n_call_stmt�ȵ�
	process_node(n->sub_node[0]->sub_node[0]);
	//����n_stmt_list
	process_node(n->sub_node[1]);
}
//done
void SemanticAnalyser::process_n_exp_stmt(node*n) {
	//����ʹ�ô���n_obj�ڵ�ĺ�������n_exp_stmt�ڵ�
	//��Ϊn_exp_stmt��ǰ�����ӽڵ����þ���һ��n_obj�ڵ�
	Obj obj = process_n_obj(n);
	if (obj.addr == -1)return;//ǰ����д�,����
	int pos = (int)this->id_table.size() - 1;
	int now_addr = this->id_table[pos].number_of_id;
	//�Ƚ������յĸ�ֵ��Ԫʽ
	quaternion q_assign;
	q_assign.op.action = "=";
	q_assign.op.line = obj.line;

	//q_assign.a.addr=obj.addr;
	q_assign.a.obj = obj;

	Obj ans = process_n_e(n->sub_node[3], obj.line);
	if (ans.addr == -1)return;//������ʽʱ����,����
	q_assign.b.obj = ans;
	//q_assign.b.addr=ans.addr;
	//���q_assign����code
	this->code.push_back(q_assign);
	this->code_point++;
	this->id_table[pos].number_of_id = now_addr;//�ͷ�֮ǰ����Щ�����õ��ڴ�
}
//done
Obj SemanticAnalyser::process_n_e(node *n, int line) {
	//����n_t
	Obj nt = this->process_n_t(n->sub_node[0], line);
	if (nt.addr == -1)return nt;//֮ǰ����,����һ����ַΪ-1��obj

	node*now_neq = n->sub_node[1];//��ǰ�����neq
	int now_addr = nt.addr;//��ǰ�ļ����ĵ�ַ
	bool is_int = nt.ia, is_real = nt.ra;
	while (now_neq->sub_node[0]->symbol.type != s_null) {
		//neq��û�б��ݹ�Ĵ�����,���Ǻ��滹�м���
		Obj next_nt = process_n_t(now_neq->sub_node[1], line);//������һ������
		if (next_nt.addr == -1)return nt;//֮ǰ����,����һ����ַΪ-1��obj
		//����һ���µ���Ԫʽ,������������������
		quaternion q;
		if (now_neq->sub_node[0]->symbol.type == s_plus)q.op.action = "+";
		else q.op.action = "-";
		q.op.line = line;
		q.a.obj = nt;
		q.b.obj = next_nt;

		int pos = (int)this->id_table.size() - 1;
		//Ϊ�ӵĺ�����һ���ڴ�
		q.c.obj.addr = this->id_table[pos].number_of_id;
		this->id_table[pos].number_of_id++;
		//��������code
		q.c.obj.ra = q.a.obj.ra || q.b.obj.ra;
		q.c.obj.ia = !q.c.obj.ra;
		this->code.push_back(q);
		this->code_point++;
		//���µ�ǰ������Ϣ
		nt = q.c.obj;
		now_neq = now_neq->sub_node[2];
	}
	Obj ret;
	//ret.addr=now_addr;
	ret = nt;
	return ret;
}
//done
Obj SemanticAnalyser::process_n_t(node *n, int line) {
	//����n_f
	Obj nf = this->process_n_f(n->sub_node[0]);
	if (nf.addr == -1)return nf;//֮ǰ����,����һ����ַΪ-1��obj

	node*now_ntq = n->sub_node[1];//��ǰ�����ntq
	int now_addr = nf.addr;//��ǰ�ĳ����ĵ�ַ
	bool is_int = nf.ia, is_real = nf.ra;
	while (now_ntq->sub_node[0]->symbol.type != s_null) {
		//ntq��û�б��ݹ�Ĵ�����,���Ǻ��滹�г���
		Obj next_nf = process_n_f(now_ntq->sub_node[1]);//������һ������
		if (next_nf.addr == -1)return nf;//֮ǰ����,����һ����ַΪ-1��obj
		//����һ���µ���Ԫʽ,������������������
		quaternion q;
		if (now_ntq->sub_node[0]->symbol.type == s_mult)q.op.action = "*";
		else q.op.action = "/";
		q.op.line = line;
		q.a.obj = nf;
		q.b.obj = next_nf;
		int pos = (int)this->id_table.size() - 1;
		//Ϊ�˻�����һ���ڴ�
		//q.c.addr=this->id_table[pos].number_of_id;
		q.c.obj.addr = this->id_table[pos].number_of_id;
		this->id_table[pos].number_of_id++;

		//q.c.is_addr=1;
		q.c.obj.ra = q.a.obj.ra || q.b.obj.ra;
		q.c.obj.ia = !q.c.obj.ra;

		//��������code
		this->code.push_back(q);
		this->code_point++;
		//���µ�ǰ������Ϣ
		nf = q.c.obj;
		now_ntq = now_ntq->sub_node[2];
	}
	Obj ret;
	//ret.addr=now_addr;
	ret = nf;
	return ret;
}
//done
Obj SemanticAnalyser::process_n_f(node *n) {
	Obj ret;
	if (n->sub_node[0]->symbol.type == s_lsp) {
		//ʹ�ò���ʽn_f->(n_e)
		Obj ne = this->process_n_e(n->sub_node[1], n->sub_node[0]->symbol.lineNum);
		if (ne.addr == -1)return ne;//������
		ret = ne;
	}
	else {
		//ʹ�ò���ʽn_f->n_obj
		Obj obj = this->process_n_obj(n->sub_node[0]);
		if (obj.addr == -1)return obj;//������
		if (obj.is_int || obj.is_real) {
			//obj��һ��ֵ�����ǵ�ַ
			//Ϊ���ֵ��������һ����ַ
			int pos = (int)this->id_table.size() - 1;
			quaternion make_value;
			make_value.op.action = "=";
			make_value.op.line = obj.line;
			make_value.a.obj.addr = this->id_table[pos].number_of_id;
			make_value.a.obj.ia = obj.is_int;
			make_value.a.obj.ra = obj.is_real;

			this->id_table[pos].number_of_id++;
			make_value.b.obj.is_int = obj.is_int;
			make_value.b.obj.is_real = obj.is_real;
			make_value.b.obj.value = obj.value;
			this->code.push_back(make_value);
			this->code_point++;
			ret = make_value.a.obj;
		}
		else {
			//����һ����ַ
			ret = obj;
		}
	}
	return ret;
}
//done
void SemanticAnalyser::process_n_call_stmt(node*n) {
	//ת��Ϊ��Ԫʽ(call,function,_,paramater_num)
	//����Ƿ����������
	string func_name = n->sub_node[1]->symbol.contain;
	if (this->id_table[0].table.count(func_name) == 0) {
		this->wp->process(not_decleared_func_or_var, n->sub_node[1]);
		this->error = 1;
		return;
	}
	//���������������ķ��ű�
	int pos = (int)this->id_table.size() - 1;
	//Ϊ��������ֵԤ��һ��λ��
	this->id_table[pos].number_of_id++;
	int ret_pos= this->id_table[pos].number_of_id;
	quaternion q;
	q.op.action = "call";
	q.op.line = n->sub_node[1]->symbol.lineNum;
	//������
	q.a.is_func = 1;
	q.a.func_name = func_name;
	q.b.obj.addr = this->id_table[pos].number_of_id - 1;
	//��������
	node* nt = n->sub_node[3];
	int count_para = 0;
	if (nt->sub_node.size() == 2) {
		nt = nt->sub_node[1];
		count_para++;
		while (nt->sub_node.size() == 3) {
			count_para++;
			nt = nt->sub_node[2];
		}
	}
	q.c.obj.value = count_para;
	//�жϲ��������Ƿ�ƥ��
	if (this->id_table[0].table[func_name].para_list.size() != count_para) {
		this->wp->process(too_many_or_to_less_function_para, n->sub_node[1]);
		this->error = 1;
		return;
	}
	//��������,�����������Ԫʽ
	process_n_real_plist(n->sub_node[3], n->sub_node[1]);
	//����call��Ԫʽ
	this->code.push_back(q);
	this->code_point++;
	//�˻ز���ռ�õĿռ�
	//this->id_table[pos].number_of_id -= (int)q.c.obj.value;
	this->id_table[pos].number_of_id = ret_pos;
}
//done
void SemanticAnalyser::process_n_real_plist(node *n, node*fun_id) {
	//�ǿ���,����
	if (n->sub_node[0]->symbol.type == s_null)
		return;

	//Ϊ�����������ռ�
	int pos = (int)this->id_table.size() - 1;
	
	int t = this->id_table[pos].number_of_id;
	Obj obj = process_n_obj(n->sub_node[0]);
	if(id_type( obj.id)!=a_function)
		this->id_table[pos].number_of_id++;//�������Զ��ĸ��Լ�����һ���������÷���ֵ�Ŀռ䣬�����ظ�����
	if (obj.addr == -1)return;//ǰ����˴�,ֱ�ӷ���
	//����Ƿ������Ͳ�ƥ��Ĵ���
	//obj��һ��ֵ�����Ǳ���
	int type = this->id_table[0].table[fun_id->symbol.contain].para_list[0].type;
	if ((obj.is_int || obj.is_real) && !(obj.is_int&&type == a_int_var) && !(obj.is_real&&type == a_real_var)) {
		this->wp->process(type_is_not_correct, fun_id);
		this->error = 1;
		return;
	}
	//obj��һ������
	bool is_real = this->id_type(obj.id) == a_real_var || this->id_type(obj.id) == a_real_list;
	bool is_int = this->id_type(obj.id) == a_int_var || this->id_type(obj.id) == a_int_list;
	if (this->id_type(obj.id) == a_function) {
		int rt = this->id_table[0].table[obj.id].return_type;
		if (rt == s_int)is_int = 1;
		else if (rt == s_real)is_real = 1;
	}
	if ((!obj.is_int && !obj.is_real) && ((is_real&&type == a_int_var) ||
		(is_int&&type == a_real_var))) {
		this->wp->process(type_is_not_correct, fun_id);
		this->error = 1;
		return;
	}

	//������Ԫʽ
	quaternion q;
	q.op.action = "=";
	q.op.line = fun_id->symbol.lineNum;
	q.a.obj.addr = t;
	//q.a.obj.addr = this->id_table[pos].number_of_id - 1;
	q.b.obj = obj;

	//����Ԫʽ����code
	this->code.push_back(q);
	this->code_point++;
	//�����ӽڵ�(n_real_plist_q):
	process_n_real_plist_q(n->sub_node[n->sub_node.size() - 1], fun_id, 1);
}
//done
void SemanticAnalyser::process_n_real_plist_q(node*n, node*fun_id, int para_num) {
	//�ǿ���,����
	if (n->sub_node[0]->symbol.type == s_null)
		return;

	//Ϊ�����������ռ�
	int pos = (int)this->id_table.size() - 1;
	
	int t = this->id_table[pos].number_of_id;
	Obj obj = process_n_obj(n->sub_node[1]);
this->id_table[pos].number_of_id++;
	if (obj.addr == -1)return;//ǰ����˴�,ֱ�ӷ���
	//����Ƿ������Ͳ�ƥ��Ĵ���
	//obj��һ��ֵ�����Ǳ���
	int type = this->id_table[0].table[fun_id->symbol.contain].para_list[para_num].type;
	if ((obj.is_int || obj.is_real) && !(obj.is_int&&type == a_int_var) && !(obj.is_real&&type == a_real_var)) {
		this->wp->process(type_is_not_correct, fun_id);
		this->error = 1;
		return;
	}
	//obj��һ������
	bool is_real = this->id_type(obj.id) == a_real_var || this->id_type(obj.id) == a_real_list;
	bool is_int = this->id_type(obj.id) == a_int_var || this->id_type(obj.id) == a_int_list;
	if (this->id_type(obj.id) == a_function) {
		int rt = this->id_table[0].table[obj.id].return_type;
		if (rt == s_int)is_int = 1;
		else if (rt == s_real)is_real = 1;
	}
	if ((!obj.is_int && !obj.is_real) && ((is_real&&type == a_int_var) ||
		(is_int&&type == a_real_var))) {
		this->wp->process(type_is_not_correct, fun_id);
		this->error = 1;
		return;
	}

	//������Ԫʽ
	quaternion q;
	q.op.action = "=";
	q.op.line = fun_id->symbol.lineNum;

	q.a.obj.addr = t;
	//q.a.obj.addr = this->id_table[pos].number_of_id - 1;
	q.b.obj = obj;
	//����Ԫʽ����code
	this->code.push_back(q);
	this->code_point++;
	//�����ӽڵ�(n_real_plist_q):
	process_n_real_plist_q(n->sub_node[n->sub_node.size() - 1], fun_id, para_num + 1);
}
//done
Obj SemanticAnalyser::process_n_obj(node *n) {
	Symbol s = n->sub_node[0]->symbol;
	int type = s.type;
	Obj obj;
	obj.line = s.lineNum;
	//id������
	if (type == s_inum) {
		obj.is_int = 1;
		obj.value = s.value;
	}
	else if (type == s_rnum) {
		obj.is_real = 1;
		obj.value = s.value;
	}
	else if (type == s_id) {
		//�Ǳ������������е�ֵ
		int addr;
		obj.id = s.contain;
		if ((addr = have_id(s.contain)) == -1) {
			this->wp->process(not_decleared_func_or_var, n->sub_node[0]);
			this->error = 1;
			obj.addr = -1;
			return obj;
		}
		//������id��һ������,��Ȼ�Ǵ��
		if (id_type(s.contain) == a_function) {
			this->wp->process(use_function_like_a_value_or_list, n->sub_node[0]);
			this->error = 1;
			obj.addr = -1;
			return obj;
		}
		obj.id = s.contain;
		if (n->sub_node[1]->sub_node[0]->symbol.type == s_null) {
			//�Ա�������ʽʹ������,����Ҫ����
			if (id_type(s.contain) == a_real_list || id_type(s.contain) == a_int_list) {
				this->wp->process(use_a_list_as_value, n->sub_node[0]);
				this->error = 1;
				obj.addr = -1;
				return obj;
			}
			//ȷʵ�Ǳ�����������
			obj.addr = addr;
			if (id_type(s.contain) == a_real_var)obj.ra = 1;
			else obj.ia = 1;
		}
		else {
			obj.start_addr = addr;
			int last_table = (int)this->id_table.size() - 1;
			for (int i = last_table; i >= 0; i--) {
				if (this->id_table[i].table.count(s.contain)) {
					obj.length = this->id_table[i].table[s.contain].length;
				}
			}
			//���������ʽʹ�ñ���,����Ҫ����
			if (id_type(s.contain) == a_real_var || id_type(s.contain) == a_int_var) {
				this->wp->process(use_a_value_as_list, n->sub_node[0]);
				this->error = 1;
				obj.addr = -1;
				return obj;
			}
			Obj o = process_n_obj(n->sub_node[1]->sub_node[1]);
			if (o.is_real) {
				//[a]�е�a�Ǹ�ʵ��,��Ȼ����
				this->wp->process(list_dice_should_not_be_double, n->sub_node[1]->sub_node[1]->sub_node[0]);
				this->error = 1;
				obj.addr = -1;
				return obj;
			}
			else if (o.is_int) {
				//�Ǹ�����,ֱ��ʹ��
				obj.addr = addr + (int)o.value;
			}
			else {
				//�Ǹ���ַ
				//�����ַ��Ӧ��id�������͵�id
				if (id_type(o.id) != a_int_var&&id_type(o.id) != a_int_list) {
					//�Ȳ鿴�����ַ�ǲ�����Դ��һ������,����������䷵��ֵ
					bool valid = 0, is_func = 0;
					if (id_type(o.id) == a_function) {
						is_func = 1;
						int return_type = this->id_table[0].table[o.id].return_type;
						if (return_type == s_int)valid = 1;//����ֵ����Ҫ���������
					}
					if (!valid) {
						if (is_func)
							this->wp->process(list_dice_should_not_be_double, n->sub_node[1]->sub_node[1]->sub_node[0]->sub_node[0]);
						else
							this->wp->process(list_dice_should_not_be_double, n->sub_node[1]->sub_node[1]->sub_node[0]);
						this->error = 1;
						obj.addr = -1;
						return obj;
					}
				}
				obj.addr = addr;
				obj.move_obj = new Obj;
				*obj.move_obj = o;
				obj.have_move = 1;
			}
			if (id_type(s.contain) == a_real_list)obj.ra = 1;
			else obj.ia = 1;
		}
	}
	else {
		//��һ����������
		process_node(n->sub_node[0]);
		int pos = (int)this->id_table.size() - 1;
		obj.addr = this->id_table[pos].number_of_id - 1;
		obj.id = n->sub_node[0]->sub_node[1]->symbol.contain;
		obj.line = n->sub_node[0]->sub_node[1]->symbol.lineNum;

		int return_type = this->id_table[0].table[obj.id].return_type;
		if (return_type == s_int)obj.ia = 1;
		else obj.ra = 1;
	}
	return obj;
}
//�������,���ص�ַ;���򷵻�-1
int SemanticAnalyser::have_id(string id) {
	int last_table = (int)this->id_table.size() - 1;
	for (int i = last_table; i >= 0; i--) {
		if (this->id_table[i].table.count(id)) {
			return this->id_table[i].table[id].addr;
		}
	}
	return -1;
}
//����id������,
int SemanticAnalyser::id_type(string id) {
	int last_table = (int)this->id_table.size() - 1;
	for (int i = last_table; i >= 0; i--) {
		if (this->id_table[i].table.count(id)) {
			return this->id_table[i].table[id].type;
		}
	}
	return -1;
}
//done
void SemanticAnalyser::process_n_ret_stmt(node*n) {
	string id_name;
	//������Ԫʽ
	quaternion q;
	int type = 0;//�����type��IDTypeö������,����ֻ����������int����real,������������id,����,���ǳ���
	int addr = -1;//����ǳ���,��ôaddr==-1
	Obj obj = this->process_n_obj(n->sub_node[1]);
	q.op.action = "return";
	q.op.line = obj.line;
	if (obj.addr == -1) {
		this->code.push_back(q);
		this->code_point++;
		return;
	}//����objʱ�Ѿ�����,�����ټ�����,ֻҪ������Ԫʽ��ʾ�й�return����}
	 //�ǲ���һ������
	bool is_fun = 0;
	if (obj.is_real) { type = a_real_var; }
	else if (obj.is_int) { type = a_int_var; }
	else {
		//�Ǹ�����,����,���ߺ���
		//�Ȳ鿴�����ַ�ǲ�����Դ��һ������,����������䷵��ֵ
		if (id_type(obj.id) == a_function) {
			int return_type = this->id_table[0].table[obj.id].return_type;
			if (return_type == s_int) { type = a_int_var; }
			else { type = a_real_var; }
			is_fun = 1;
		}
		else if (id_type(obj.id) == a_real_var || id_type(obj.id) == a_real_list) {
			type = a_real_var;
		}
		else if (id_type(obj.id) == a_int_var || id_type(obj.id) == a_int_list) {
			type = a_int_var;
		}
	}
	//��鷵�������ǲ�����ȷ��
	vector<string>&all_func_name = this->id_table[0].order;
	string func_name = all_func_name[all_func_name.size() - 1];
	int func_return_type = this->id_table[0].table[func_name].return_type;
	if ((func_return_type == s_int&&type == a_real_var) ||
		(func_return_type == s_real&&type == a_int_var)) {
		//���Ͳ�ƥ��
		if (is_fun)
			this->wp->process(wrong_return_value, n->sub_node[1]->sub_node[0]->sub_node[0]);
		else
			this->wp->process(wrong_return_value, n->sub_node[1]->sub_node[0]);
		this->error = 1;
		//return;����ֱ�ӷ���,��Ϊ����return�ӽ�ȥ���ܻ���Ϊ�����޷�����
	}

	//������Ԫʽ
	q.a.obj = obj;
	this->code.push_back(q);
	this->code_point++;
}
//done
void SemanticAnalyser::process_n_decl_stmt(node*n) {
	IDType type;
	if (n->sub_node[0]->sub_node[0]->symbol.type == s_int)
			type = a_int_var;
		else
			type = a_real_var;
	node *nq = n;
	do {
		mID id;
		id.name = nq->sub_node[1]->symbol.contain;
		id.type = type;
		//��ȡ���ڵķ��ű���������һ���ȼ���
		int pos = (int)this->id_table.size() - 1;
		if (have_id(id.name) != -1) {
			//����������������иñ�����
			wp->process(decleared_var, nq->sub_node[1]);
			this->error = 1;
			return;
		}
		this->id_table[pos].table[id.name].addr = this->id_table[pos].number_of_id;
		this->id_table[pos].number_of_id++;
		//���������ӽڵ�dfollow�Ƿ�Ϊ����,����Ϊ����,������Ϊ����
		if (nq->sub_node[2]->sub_node[0]->symbol.type != s_null) {
			//������
			node*follow = nq->sub_node[2];
			int length = (int)follow->sub_node[1]->symbol.value;
			this->id_table[pos].number_of_id += (length - 1);
			if (id.type == a_real_var)id.type = a_real_list;
			else id.type = a_int_list;
			this->id_table[pos].table[id.name].length = length;
		}
		this->id_table[pos].table[id.name].type = id.type;
		this->id_table[pos].table[id.name].name = id.name;
		nq = nq->sub_node[3];
	} while (nq->sub_node[0]->symbol.type != s_null);
}
//done
void SemanticAnalyser::process_n_read_stmt(node*n) {
	int type = n->sub_node[1]->sub_node[0]->symbol.type;
	if (type == n_call_stmt || type == s_inum || type == s_rnum) {
		//�����޸ĵ���ֵ,readһ����������
		if (type == n_call_stmt)this->wp->process(cannot_be_reform, n->sub_node[1]->sub_node[0]->sub_node[1]);
		else this->wp->process(cannot_be_reform, n->sub_node[1]->sub_node[0]);
		this->error = 1;
		return;
	}

	Obj o = process_n_obj(n->sub_node[1]);
	if (o.addr == -1)return;
	quaternion q;
	q.op.action = "read";
	q.op.line = o.line;
	q.a.obj = o;
	this->code.push_back(q);
	this->code_point++;
}
//done
void SemanticAnalyser::process_n_write_stmt(node*n) {
	Obj o = process_n_obj(n->sub_node[1]);
	if (o.addr == -1)return;
	quaternion q;
	q.op.action = "write";
	q.op.line = o.line;
	q.a.obj = o;
	this->code.push_back(q);
	this->code_point++;
}
//done
void SemanticAnalyser::process_n_if_stmt(node*n) {
	//Ϊ�˷���������Ԫʽ�Ĺ����в�ͬ��if,else,while�������򲻲�����ͻ
	//�ò�ͬ��id_table���洢ͬһ�����в�ͬ��if,else,while�ı���
	//����id_table��number_of_id�ֶμ�¼���Ǵ�����Ԫʽʱ����ջ֡�����ݵĶ����ĵ�ַ
	//���������ַ��Ӧ���Ǵ�0��ʼ��,��Ӧ���Ǵ�֮ǰ��ʼ��

	//����һ���µ�id_table
	IDTable if_table;
	//�ı��ַƫ����
	int pos = (int)this->id_table.size() - 1;
	if_table.number_of_id = this->id_table[pos].number_of_id;
	this->id_table.push_back(if_table);

	//����n_e_exp
	Inequity eq = process_n_e_exp(n->sub_node[2]);
	//if(eq.op==0)return;//�������������,����һ��Ҫ����,�����ٷ�������
	//ռλ
	quaternion q;
	this->code.push_back(q);
	this->code_point++;
	//�����ܹ������˶������
	int code_length_before = this->code_point;
	process_node(n->sub_node[5]);

	//������������ת��Ԫʽ���������code,ռλ
	quaternion qj;
	this->code.push_back(qj);
	this->code_point++;
	//����������ת��Ԫʽ������
	//�����и�������˼�ĵط�,����stl������ΪҪ���ϵ�����,�������ݵĵ�ַ�����ǻ���
	quaternion* q_if = &this->code[code_length_before - 1];
	q_if->op.action = "jif";
	q_if->op.line = n->sub_node[0]->symbol.lineNum;
	q_if->a.compare = eq;
	q_if->b.obj.value = this->code_point;
	//����else���
	//�����else
	if (n->sub_node[7]->sub_node[0]->symbol.type != s_null) {
		int code_else = this->code_point;
		process_node(n->sub_node[7]->sub_node[2]);

		quaternion *q_else = &this->code[code_else - 1];
		q_else->op.action = "jump";
		q_else->op.line = n->sub_node[0]->symbol.lineNum;
		q_else->b.obj.value = this->code_point;
	}
	//����id_table
	this->id_table.pop_back();
	//������ɺ���ͬʱ������һ��ĵ�ַ,��Ϊ��������ݶ��Ǿֲ�����,���ܱ�������
}
//usless by now
void process_eq_and_obj(double *eq_v, int *eq_a, Obj *obj) {
	if (obj->is_int || obj->is_real) {
		//��һ��ֵ
		*eq_v = obj->value;
	}
	else {
		*eq_a = obj->addr;
	}
}
//done
Inequity SemanticAnalyser::process_n_e_exp(node *n) {
	Obj left = this->process_n_obj(n->sub_node[0]);
	Obj right = this->process_n_obj(n->sub_node[2]);
	Inequity eq;
	if (left.addr == -1 || right.addr == -1) {
		//�����󲿻��Ҳ�ʱ����
		eq.op = 0;
		return eq;
	}
	//�����󲿺��Ҳ�
	eq.left = left;
	eq.right = right;
	//�鿴�Ƚ��������ʲô
	int type = n->sub_node[1]->sub_node[0]->symbol.type;
	if (type == s_bigger)eq.op = '>';
	else if (type == s_smaller)eq.op = '<';
	else if (type == s_notequal)eq.op = '!';
	else if (type == s_equal)eq.op = '=';
	return eq;
}
//done
void SemanticAnalyser::process_n_while_stmt(node*n) {
	//Ϊ�˷���������Ԫʽ�Ĺ����в�ͬ��if,else,while�������򲻲�����ͻ
	//�ò�ͬ��id_table���洢ͬһ�����в�ͬ��if,else,while�ı���
	//����id_table��number_of_id�ֶμ�¼���Ǵ�����Ԫʽʱ����ջ֡�����ݵĶ����ĵ�ַ
	//���������ַ��Ӧ���Ǵ�0��ʼ��,��Ӧ���Ǵ�֮ǰ��ʼ��

	//����һ���µ�id_table
	IDTable while_table;
	//�ı��ַƫ����
	int pos = (int)this->id_table.size() - 1;
	while_table.number_of_id = this->id_table[pos].number_of_id;
	this->id_table.push_back(while_table);

	//����n_e_exp
	Inequity eq = process_n_e_exp(n->sub_node[2]);
	//if(eq.op==0)return;//�������������,����һ��Ҫ����,�����ٷ�������
	//��ȡjif��Ԫʽ��λ��
	int cbefore = this->code_point;
	//jif��Ԫʽռλ
	quaternion q;
	this->code.push_back(q);
	this->code_point++;
	//�������
	this->process_node(n->sub_node[5]);
	//����jif
	this->code[cbefore].op.action = "jif";
	this->code[cbefore].op.line = n->sub_node[0]->symbol.lineNum;
	this->code[cbefore].a.compare = eq;
	this->code[cbefore].b.obj.value = this->code_point + 1;

	//����jump
	quaternion qj;
	qj.op.action = "jump";
	qj.op.line = n->sub_node[0]->symbol.lineNum;
	qj.b.obj.value = cbefore;//���ص�������ת���������ж�
	this->code.push_back(qj);
	this->code_point++;
	//����id_table
	this->id_table.pop_back();
}
SemanticAnalyser::SemanticAnalyser(SyntacticAnalyser &sa, Ui::compiler_finalClass*aui) {
	this->syn = &sa;
	this->code_point = 0;
	this->id_table.push_back(IDTable());
	this->wp = new WrongProcesser(aui);
	this->ui = aui;
	this->start_point = -1;
	this->read_tree();
}
void SemanticAnalyser::read_tree() {
	node* node = this->syn->root;
	process_node(node);
}
//done
bool SemanticAnalyser::must_can_return(int codep) {
	while (codep<this->code_point) {
		quaternion&q = this->code[codep];
		if (q.op.action == "return")return 1;
		else if (q.op.action == "jif") {
			bool can = this->jump_return(codep);
			if (can)return 1;
			else continue;//���ڼ�һ��,�ں������Ѿ�ָ����һ������
		}
		codep++;
	}
	return 0;
}
//done
bool SemanticAnalyser::jump_return(int &codep) {
	quaternion&q = this->code[codep];
	int jump = (int)(q.b.obj.value) - 1;//jump���(�����)�ĵ�ַ
	if (this->code[jump].op.action == "jump") {
		//��jump���,�������while�������if���
		if ((int)this->code[jump].b.obj.value == codep) {
			//while���,����һ������return
			//�����ⲿ�ִ���
			codep = jump + 1;
			return 0;
		}
		else {
			//��else��if���
			//�����,�ȼ��if�����,�ټ��else
			bool if_can_ret = 0;
			bool else_can_ret = 0;
			codep++;
			while (codep<jump) {
				quaternion&q = this->code[codep];
				if (q.op.action == "return") {
					if_can_ret = 1;
					break;
				}
				else if (q.op.action == "jif") {
					bool can = this->jump_return(codep);
					if ((if_can_ret = can))break;
				}
				codep++;
			}
			if (if_can_ret) {
				//if���Է���,��������else�Ƿ���Է���
				codep++;
				while (codep<(int)code[jump].b.obj.value) {
					quaternion&q = this->code[codep];
					if (q.op.action == "return") {
						else_can_ret = 1;
						break;
					}
					else if (q.op.action == "jif") {
						bool can = this->jump_return(codep);
						if ((else_can_ret = can))break;
					}
					codep++;
				}
			}
			codep = (int)code[jump].b.obj.value;//��֤��������
			return (if_can_ret&&else_can_ret);
		}
	}
	//û��else��if,����һ������
	codep = jump + 1;
	return 0;
}