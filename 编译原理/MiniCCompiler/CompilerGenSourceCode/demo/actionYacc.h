#ifndef _ACTION_YACC_H
#define _ACTION_YACC_H
#include "structDefine.h"
#include "supportFunction.h"
#include <string>
#include <stack>
#include <stdlib.h>
extern unsigned int offset;
std::string p;
extern std::deque<StackItem> stack;
extern std::stack<std::string> paramStack;
std::pair<unsigned int, std::string> performAction(unsigned int index, std::map<std::string, std::string>& reduceHead) {
size_t stackSize = stack.size() - 1;
switch(index) {
case 0 ://S->program 

return std::pair<unsigned int, std::string>(1,"S");

case 1 ://program->globalData stmts 

return std::pair<unsigned int, std::string>(2,"program");

case 2 ://program->stmts 

return std::pair<unsigned int, std::string>(1,"program");

case 3 ://globalData->globalStmts 
		
return std::pair<unsigned int, std::string>(1,"globalData");

case 4 ://globalStmts->globalStmts globalStmt 

return std::pair<unsigned int, std::string>(2,"globalStmts");

case 5 ://globalStmts->globalStmt 

return std::pair<unsigned int, std::string>(1,"globalStmts");

case 6 ://globalStmt->static var_decl ; 

return std::pair<unsigned int, std::string>(3,"globalStmt");

case 7 ://stmts->stmts M stmt 
			backpatch(stack[stackSize - 3 + 1]._map["nextlist"], stack[stackSize - 3 + 2]._map["instr"]);
			reduceHead["nextlist"] = stack[stackSize - 3 + 3]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(3,"stmts");

case 8 ://stmts->stmt 
			reduceHead["nextlist"] = stack[stackSize - 1 + 1]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(1,"stmts");

case 9 ://stmt->{ stmts } 
			reduceHead["nextlist"] = stack[stackSize - 3 + 2]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(3,"stmt");

case 10 ://stmt->fun_define 
			returnToGlobalTable();
			
return std::pair<unsigned int, std::string>(1,"stmt");

case 11 ://stmt->if_stmt 
			reduceHead["nextlist"] = stack[stackSize - 1 + 1]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(1,"stmt");

case 12 ://stmt->while_stmt 
			reduceHead["nextlist"] = stack[stackSize - 1 + 1]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(1,"stmt");

case 13 ://stmt->var_decl ; 
			reduceHead["nextlist"] = stack[stackSize - 2 + 1]._map["nextlist"];
			
return std::pair<unsigned int, std::string>(2,"stmt");

case 14 ://stmt->expr_stmt ; 
			
return std::pair<unsigned int, std::string>(2,"stmt");

case 15 ://stmt->return expr ; 
			emit("return",stack[stackSize - 3 + 2]._map["place"],"","");
			setOutLiveVar(stack[stackSize - 3 + 2]._map["place"]);
			
return std::pair<unsigned int, std::string>(3,"stmt");

case 16 ://fun_define->fun_decl_head BlockLeader { stmts } 
						reduceHead["name"] = stack[stackSize - 5 + 1]._map["name"];
						
return std::pair<unsigned int, std::string>(5,"fun_define");

case 17 ://fun_decl_head->type_spec id ( ) 
						reduceHead["name"] = stack[stackSize - 4 + 2]._map["lexeme"];
						createSymbolTable(stack[stackSize - 4 + 2]._map["lexeme"], atoi(stack[stackSize - 4 + 1]._map["width"].c_str()));
						addFunLabel(nextInstr, stack[stackSize - 4 + 2]._map["lexeme"]);
						
return std::pair<unsigned int, std::string>(4,"fun_decl_head");

case 18 ://fun_decl_head->type_spec id ( param_list ) 
						reduceHead["name"] = stack[stackSize - 5 + 2]._map["lexeme"];
						createSymbolTable(stack[stackSize - 5 + 2]._map["lexeme"], atoi(stack[stackSize - 5 + 1]._map["width"].c_str()));
						addToSymbolTable(stack[stackSize - 5 + 4]._map["itemlist"]);
						addFunLabel(nextInstr, stack[stackSize - 5 + 2]._map["lexeme"]);
						
return std::pair<unsigned int, std::string>(5,"fun_decl_head");

case 19 ://param_list->param_list , param 
                reduceHead["itemlist"] = stack[stackSize - 3 + 1]._map["itemlist"]  +   stack[stackSize - 3 + 3]._map["itemlist"];
                
return std::pair<unsigned int, std::string>(3,"param_list");

case 20 ://param_list->param 
                reduceHead["itemlist"] = stack[stackSize - 1 + 1]._map["itemlist"];
                
return std::pair<unsigned int, std::string>(1,"param_list");

case 21 ://param->type_spec id 
		        reduceHead["itemlist"] = makeParam(stack[stackSize - 2 + 2]._map["lexeme"],stack[stackSize - 2 + 1]._map["type"],atoi(stack[stackSize - 2 + 1]._map["width"].c_str()));
				
return std::pair<unsigned int, std::string>(2,"param");

case 22 ://param->type_spec id [ int_literal ] 
				reduceHead["itemlist"] = makeParam(stack[stackSize - 5 + 2]._map["lexeme"],array(atoi(stack[stackSize - 5 + 4]._map["lexval"].c_str()),stack[stackSize - 5 + 1]._map["type"]),atoi(stack[stackSize - 5 + 4]._map["lexval"].c_str()) * atoi(stack[stackSize - 5 + 1]._map["width"].c_str()));
                
return std::pair<unsigned int, std::string>(5,"param");

case 23 ://int_literal->num 
					reduceHead["lexval"] = stack[stackSize - 1 + 1]._map["lexeme"];
					
return std::pair<unsigned int, std::string>(1,"int_literal");

case 24 ://static_var_decl->static var_decl 
					//reduceHead["code"] = stack[stackSize - 2 + 2]._map["code"];
					
return std::pair<unsigned int, std::string>(2,"static_var_decl");

case 25 ://var_decl->type_spec id 
				enter(stack[stackSize - 2 + 2]._map["lexeme"],stack[stackSize - 2 + 1]._map["type"],atoi(stack[stackSize - 2 + 1]._map["width"].c_str()));
				
return std::pair<unsigned int, std::string>(2,"var_decl");

case 26 ://var_decl->type_spec id = expr 
				p = enter(stack[stackSize - 4 + 2]._map["lexeme"],stack[stackSize - 4 + 1]._map["type"],atoi(stack[stackSize - 4 + 1]._map["width"].c_str()));
				emit("",stack[stackSize - 4 + 4]._map["place"],"",p);
				
return std::pair<unsigned int, std::string>(4,"var_decl");

case 27 ://var_decl->type_spec id [ int_literal ] 
				enter(stack[stackSize - 5 + 2]._map["lexeme"],array(atoi(stack[stackSize - 5 + 4]._map["lexval"].c_str()),stack[stackSize - 5 + 1]._map["type"]),atoi(stack[stackSize - 5 + 4]._map["lexval"].c_str()) * atoi(stack[stackSize - 5 + 1]._map["width"].c_str()));
				
return std::pair<unsigned int, std::string>(5,"var_decl");

case 28 ://type_spec->int 
				reduceHead["type"] = "int";
				reduceHead["width"] = "2";
				
return std::pair<unsigned int, std::string>(1,"type_spec");

case 29 ://type_spec->double 
				reduceHead["type"] = "double";
				reduceHead["width"] = "2";
				
return std::pair<unsigned int, std::string>(1,"type_spec");

case 30 ://type_spec->void 
				reduceHead["type"] = "void";
				reduceHead["width"] = "0";
				
return std::pair<unsigned int, std::string>(1,"type_spec");

case 31 ://expr_stmt->id = expr 
				p = lookupPlace(stack[stackSize - 3 + 1]._map["lexeme"]);
				if (p.empty()) error();
				emit("",stack[stackSize - 3 + 3]._map["place"],"",p);
                
return std::pair<unsigned int, std::string>(3,"expr_stmt");

case 32 ://expr->expr + expr 
                reduceHead["place"] = newtemp(stack[stackSize - 3 + 1]._map["place"]);
		emit("ADD", stack[stackSize - 3 + 1]._map["place"], stack[stackSize - 3 + 3]._map["place"], reduceHead["place"]);
                
return std::pair<unsigned int, std::string>(3,"expr");

case 33 ://expr->expr - expr 
                reduceHead["place"] = newtemp(stack[stackSize - 3 + 1]._map["place"]);
		emit("SUB", stack[stackSize - 3 + 1]._map["place"], stack[stackSize - 3 + 3]._map["place"], reduceHead["place"]);
                
return std::pair<unsigned int, std::string>(3,"expr");

case 34 ://expr->expr * expr 
                reduceHead["place"] = newtemp(stack[stackSize - 3 + 1]._map["place"]);
		emit("MUL", stack[stackSize - 3 + 1]._map["place"], stack[stackSize - 3 + 3]._map["place"], reduceHead["place"]);
                
return std::pair<unsigned int, std::string>(3,"expr");

case 35 ://expr->expr / expr 
                reduceHead["place"] = newtemp(stack[stackSize - 3 + 1]._map["place"]);
		emit("DIV", stack[stackSize - 3 + 1]._map["place"], stack[stackSize - 3 + 3]._map["place"], reduceHead["place"]);
                
return std::pair<unsigned int, std::string>(3,"expr");

case 36 ://expr->( expr ) 
                reduceHead["place"] = stack[stackSize - 3 + 2]._map["place"];
                
return std::pair<unsigned int, std::string>(3,"expr");

case 37 ://expr->id 
		reduceHead["place"] = lookupPlace(stack[stackSize - 1 + 1]._map["lexeme"]);
            
return std::pair<unsigned int, std::string>(1,"expr");

case 38 ://expr->id ( arg_list ) 
		p = gen(paramStack.size());
		while (!paramStack.empty()) {			emit("param", paramStack.top(),"","");
			paramStack.pop();
		}		emit("call", p, stack[stackSize - 4 + 1]._map["lexeme"],"");
		enter("#","int",2);
		reduceHead["place"] = newtemp("#");
		emit("","#","",reduceHead["place"]);
		//reduceHead["place"] = "#";
	   
return std::pair<unsigned int, std::string>(4,"expr");

case 39 ://expr->num 
		reduceHead["place"] = addNum(stack[stackSize - 1 + 1]._map["lexeme"]);
	   
return std::pair<unsigned int, std::string>(1,"expr");

case 40 ://arg_list->arg_list , expr 
				paramStack.push(stack[stackSize - 3 + 3]._map["place"]);
				
return std::pair<unsigned int, std::string>(3,"arg_list");

case 41 ://arg_list->expr 
				paramStack.push(stack[stackSize - 1 + 1]._map["place"]);
				
return std::pair<unsigned int, std::string>(1,"arg_list");

case 42 ://arg_list->
				//paramStack.clear();
		  
return std::pair<unsigned int, std::string>(0,"arg_list");

case 43 ://if_stmt->if BlockLeader ( logic_expr ) M stmt 
		backpatch(stack[stackSize - 7 + 4]._map["truelist"], stack[stackSize - 7 + 6]._map["instr"]);
		reduceHead["nextlist"] = merge(stack[stackSize - 7 + 4]._map["falselist"], stack[stackSize - 7 + 7]._map["nextlist"]);
		
return std::pair<unsigned int, std::string>(7,"if_stmt");

case 44 ://if_stmt->if BlockLeader ( logic_expr ) M stmt N else M stmt 
		backpatch(stack[stackSize - 11 + 4]._map["truelist"], stack[stackSize - 11 + 6]._map["instr"]);
		backpatch(stack[stackSize - 11 + 4]._map["falselist"], stack[stackSize - 11 + 10]._map["instr"]);
		reduceHead["nextlist"] = merge(merge(stack[stackSize - 11 + 7]._map["nextlist"], stack[stackSize - 11 + 8]._map["instr"]), stack[stackSize - 11 + 11]._map["nextlist"]);
		
return std::pair<unsigned int, std::string>(11,"if_stmt");

case 45 ://while_stmt->while BlockLeader M ( logic_expr ) M stmt 
		backpatch(stack[stackSize - 8 + 8]._map["nextlist"], stack[stackSize - 8 + 3]._map["instr"]);
		backpatch(stack[stackSize - 8 + 5]._map["truelist"], stack[stackSize - 8 + 7]._map["instr"]);
		reduceHead["nextlist"] = stack[stackSize - 8 + 5]._map["falselist"];
		emit("j","","",stack[stackSize - 8 + 3]._map["instr"]);
		
return std::pair<unsigned int, std::string>(8,"while_stmt");

case 46 ://logic_expr->logic_expr && M logic_expr 
				backpatch(stack[stackSize - 4 + 1]._map["truelist"], stack[stackSize - 4 + 3]._map["instr"]);
				reduceHead["truelist"] = stack[stackSize - 4 + 4]._map["truelist"];
				reduceHead["falselist"] = merge(stack[stackSize - 4 + 1]._map["falselist"], stack[stackSize - 4 + 4]._map["falselist"]);
			
return std::pair<unsigned int, std::string>(4,"logic_expr");

case 47 ://logic_expr->logic_expr || M logic_expr 
				backpatch(stack[stackSize - 4 + 1]._map["falselist"], stack[stackSize - 4 + 3]._map["instr"]);
				reduceHead["truelist"] = merge(stack[stackSize - 4 + 1]._map["truelist"], stack[stackSize - 4 + 4]._map["truelist"]);
				reduceHead["falselist"] = stack[stackSize - 4 + 4]._map["falselist"];
			
return std::pair<unsigned int, std::string>(4,"logic_expr");

case 48 ://logic_expr->! logic_expr 
				reduceHead["truelist"] = stack[stackSize - 2 + 2]._map["falselist"];
				reduceHead["falselist"] = stack[stackSize - 2 + 2]._map["truelist"];
			
return std::pair<unsigned int, std::string>(2,"logic_expr");

case 49 ://logic_expr->( logic_expr ) 
				reduceHead["truelist"] = stack[stackSize - 3 + 2]._map["truelist"];
				reduceHead["falselist"] = stack[stackSize - 3 + 2]._map["falselist"];
			
return std::pair<unsigned int, std::string>(3,"logic_expr");

case 50 ://logic_expr->expr rel expr 
				reduceHead["truelist"] = makelist(nextInstr);
				reduceHead["falselist"] = makelist(nextInstr+1);
				emit("j"+stack[stackSize - 3 + 2]._map["op"], stack[stackSize - 3 + 1]._map["place"], stack[stackSize - 3 + 3]._map["place"], "_");
				emit("j","","","_");
			
return std::pair<unsigned int, std::string>(3,"logic_expr");

case 51 ://logic_expr->expr 
 				reduceHead["truelist"] = makelist(nextInstr);
				reduceHead["falselist"] = makelist(nextInstr+1);
				emit("j!=", stack[stackSize - 1 + 1]._map["addr"], "0", "_");
				emit("j","","","_");
			
return std::pair<unsigned int, std::string>(1,"logic_expr");

case 52 ://logic_expr->true 
				reduceHead["truelist"] = makelist(nextInstr);
				emit("j","","","_");
			
return std::pair<unsigned int, std::string>(1,"logic_expr");

case 53 ://logic_expr->false 
				reduceHead["falselist"] = makelist(nextInstr);
				emit("j","","","_");
			
return std::pair<unsigned int, std::string>(1,"logic_expr");

case 54 ://M->
reduceHead["instr"] = "LABEL_" + gen(nextInstr);

return std::pair<unsigned int, std::string>(0,"M");

case 55 ://N->
 reduceHead["instr"] = makelist(nextInstr);
      emit("j","","","_");
    
return std::pair<unsigned int, std::string>(0,"N");

case 56 ://BlockLeader->
      addLeader(nextInstr);
    
return std::pair<unsigned int, std::string>(0,"BlockLeader");

case 57 ://rel->< 
reduceHead["op"] = "<";

return std::pair<unsigned int, std::string>(1,"rel");

case 58 ://rel->> 
reduceHead["op"] = ">";

return std::pair<unsigned int, std::string>(1,"rel");

case 59 ://rel-><= 
reduceHead["op"] = "<=";

return std::pair<unsigned int, std::string>(1,"rel");

case 60 ://rel-><= 
reduceHead["op"] = ">=";

return std::pair<unsigned int, std::string>(1,"rel");

case 61 ://rel->== 
reduceHead["op"] = "==";

return std::pair<unsigned int, std::string>(1,"rel");

case 62 ://rel->!= 
reduceHead["op"] = "!=";

return std::pair<unsigned int, std::string>(1,"rel");

default: return std::pair<unsigned int, std::string>(0,"");
}
}// end function
std::string getProduction(unsigned int index) {
switch(index) {
case 0 :return "S->program ";
case 1 :return "program->globalData stmts ";
case 2 :return "program->stmts ";
case 3 :return "globalData->globalStmts ";
case 4 :return "globalStmts->globalStmts globalStmt ";
case 5 :return "globalStmts->globalStmt ";
case 6 :return "globalStmt->static var_decl ; ";
case 7 :return "stmts->stmts M stmt ";
case 8 :return "stmts->stmt ";
case 9 :return "stmt->{ stmts } ";
case 10 :return "stmt->fun_define ";
case 11 :return "stmt->if_stmt ";
case 12 :return "stmt->while_stmt ";
case 13 :return "stmt->var_decl ; ";
case 14 :return "stmt->expr_stmt ; ";
case 15 :return "stmt->return expr ; ";
case 16 :return "fun_define->fun_decl_head BlockLeader { stmts } ";
case 17 :return "fun_decl_head->type_spec id ( ) ";
case 18 :return "fun_decl_head->type_spec id ( param_list ) ";
case 19 :return "param_list->param_list , param ";
case 20 :return "param_list->param ";
case 21 :return "param->type_spec id ";
case 22 :return "param->type_spec id [ int_literal ] ";
case 23 :return "int_literal->num ";
case 24 :return "static_var_decl->static var_decl ";
case 25 :return "var_decl->type_spec id ";
case 26 :return "var_decl->type_spec id = expr ";
case 27 :return "var_decl->type_spec id [ int_literal ] ";
case 28 :return "type_spec->int ";
case 29 :return "type_spec->double ";
case 30 :return "type_spec->void ";
case 31 :return "expr_stmt->id = expr ";
case 32 :return "expr->expr + expr ";
case 33 :return "expr->expr - expr ";
case 34 :return "expr->expr * expr ";
case 35 :return "expr->expr / expr ";
case 36 :return "expr->( expr ) ";
case 37 :return "expr->id ";
case 38 :return "expr->id ( arg_list ) ";
case 39 :return "expr->num ";
case 40 :return "arg_list->arg_list , expr ";
case 41 :return "arg_list->expr ";
case 42 :return "arg_list->";
case 43 :return "if_stmt->if BlockLeader ( logic_expr ) M stmt ";
case 44 :return "if_stmt->if BlockLeader ( logic_expr ) M stmt N else M stmt ";
case 45 :return "while_stmt->while BlockLeader M ( logic_expr ) M stmt ";
case 46 :return "logic_expr->logic_expr && M logic_expr ";
case 47 :return "logic_expr->logic_expr || M logic_expr ";
case 48 :return "logic_expr->! logic_expr ";
case 49 :return "logic_expr->( logic_expr ) ";
case 50 :return "logic_expr->expr rel expr ";
case 51 :return "logic_expr->expr ";
case 52 :return "logic_expr->true ";
case 53 :return "logic_expr->false ";
case 54 :return "M->";
case 55 :return "N->";
case 56 :return "BlockLeader->";
case 57 :return "rel->< ";
case 58 :return "rel->> ";
case 59 :return "rel-><= ";
case 60 :return "rel-><= ";
case 61 :return "rel->== ";
case 62 :return "rel->!= ";
default: return "";
}
}
#endif //_ACTION_YACC_H
