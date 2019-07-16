#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/body_args_size.h"
#include "ast/all.h"  // all.h is automatically generated
// ainda tem de se fazer um novo visitor para ver os args todo que uma funcao tem

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  for (size_t i = 0; i < node->size(); i++){
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(_thisFunction){
    _pf.INT(node->value());             // local int variable, so just push it in the stack
  } 
  else{                          // global int variable, so gotta create a DATA segment (doesn't mess with stack)
    _pf.CONST(node->value());   // declare global int variable
  }
}

void xpl::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.DOUBLE(node->value());
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_string  = ++_lbl;

  _pf.RODATA(); // strings are DATA readonly

  /* generate the string */
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl_string)); // give the string a name
  _pf.STR(node->value()); // output string characters

  if(_thisFunction){
    /* local variable - leave the address on the stack */
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl_string)); // the string to be printed
  } 
  /*else{                           // global variable
    _pf.ALIGN();
    _pf.LABEL(*_thisId); 
    _pf.ID(mklbl(lbl_string));
  }*/
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);      // determine the value to be negated

  if(node->argument()->type()->name() == basic_type::TYPE_INT){
    _pf.NEG();    
  } 
  else if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DNEG();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->type()->name() == basic_type::TYPE_POINTER){
    if(node->left()->type()->name() == basic_type::TYPE_POINTER){
      node->left()->accept(this, lvl);
      node->right()->accept(this, lvl);     // right type = int
    } 
    else{
      node->right()->accept(this, lvl);
      node->left()->accept(this, lvl);        // left type = int
    }
    // calculating pointer's offset
    _pf.INT(4);
    _pf.MUL();
    _pf.ADD();
  }
  else{
    node->left()->accept(this, lvl);
    if(node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT){
      _pf.I2D();
    }
    
    node->right()->accept(this, lvl);
    if(node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
      _pf.I2D();
    }
    
    if(node->type()->name() == basic_type::TYPE_DOUBLE){
      _pf.DADD();
    }
    else if(node->type()->name() == basic_type::TYPE_INT){
      _pf.ADD();    
    }
  }
}

void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(node->type()->name() == basic_type::TYPE_POINTER){
    if(node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER){
      // caso especial sub
      if(node->left()->type()->subtype()->name() == node->right()->type()->subtype()->name()){
        node->right()->accept(this, lvl);
        node->left()->accept(this, lvl);
        _pf.SUB();
        if(node->left()->type()->subtype()->name() == basic_type::TYPE_DOUBLE)
          _pf.INT(8);
        else
          _pf.INT(4);
        
        _pf.DIV();
      }
    }
    else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER){           // right type = int
      node->right()->accept(this, lvl);
      node->left()->accept(this, lvl);
    }
    else if(node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_POINTER){           // right type = int
      node->left()->accept(this, lvl);
      node->right()->accept(this, lvl);
    }
    // calculating pointer's offset
    _pf.INT(4);
    _pf.MUL();
    _pf.SUB();
  }
  else{
    node->left()->accept(this, lvl);
    if(node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT){
      _pf.I2D();
    }
    
    node->right()->accept(this, lvl);
    if(node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
      _pf.I2D();
    }
    
    if(node->type()->name() == basic_type::TYPE_DOUBLE){
      _pf.DSUB();
    }
    else if(node->type()->name() == basic_type::TYPE_INT){
      _pf.SUB();    
    }
  }
}

void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);      // put left on stack    
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);     // put right on stack
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DMUL();
  }
  else if(node->type()->name() == basic_type::TYPE_INT){
    _pf.MUL();    
  }
}

void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);      // put left on stack
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);     // put right on stack
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DDIV();
  }
  else if(node->type()->name() == basic_type::TYPE_INT){
    _pf.DIV();    
  }
}

void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {    // not all variables are global
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if(!symbol->isVar()){
    _pf.LOCAL(-node->type()->size());     // return value type size (for special variable)
  }
  else if(!symbol->isGlobal()){     // case it's a local variable
    _pf.LOCAL(symbol->offset());
  }
  else{
    // in case of global
    _pf.ADDR(node->name());
  }
}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DLOAD();        
  } 
  else{
    _pf.LOAD();    
  }
}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    node->rvalue()->accept(this, lvl);
    if(node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.DDUP();
    else                                  // case it's an int
      _pf.DUP();

    node->lvalue()->accept(this, lvl);     // left value always double in this case
    _pf.DSTORE();
  }
  else{
    node->rvalue()->accept(this, lvl);
    _pf.DUP();
    node->lvalue()->accept(this, lvl);
    _pf.STORE();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.TRASH(node->argument()->type()->size());
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);    // put what's to print in stack

  if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.EXTERN("printd");
    _pf.CALL("printd");
    _pf.TRASH(8);                 // trash do argument
  } 
  else if(node->argument()->type()->name() == basic_type::TYPE_INT){
    _pf.EXTERN("printi");
    _pf.CALL("printi");
    _pf.TRASH(4);                 // trash do argument
  }
  else if(node->argument()->type()->name() == basic_type::TYPE_STRING){
    _pf.EXTERN("prints");
    _pf.CALL("prints");
    _pf.TRASH(4);                 // trash do argument
  } 

  if(node->newlineFlag()){
    _pf.EXTERN("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.EXTERN("readd");
    _pf.CALL("readd");
    _pf.DPUSH();
  }
  // default action for unspec = int read
  else if(node->type()->name() == basic_type::TYPE_UNSPEC || node->type()->name() == basic_type::TYPE_INT){
    _pf.EXTERN("readi");
    _pf.CALL("readi");
    _pf.PUSH();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1 = ++_lbl;
  int lbl2 = ++_lbl;

  _lbl_sweep_increment = lbl1;
  _lbl_sweep_end = lbl2;

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1 = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1));
  node->block()->accept(this, lvl);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1  = ++_lbl;
  int lbl2  = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

// --------------------------------------------------------------------------
// ------------------------------ new methods -------------------------------
// --------------------------------------------------------------------------

void xpl::postfix_writer::do_sweep_node(xpl::sweep_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  /* tests always use ints in the cycles so this is a simplification without doubles */
  int lbl_sweep_condition = ++_lbl;
  int lbl_sweep_increment = ++_lbl;
  int lbl_sweep_end = ++_lbl;

  _lbl_sweep_increment = lbl_sweep_increment;
  _lbl_sweep_end = lbl_sweep_end;

  node->start()->accept(this, lvl);
  _pf.DUP();

  // needed for identifier
  node->variable()->accept(this, lvl);
  _pf.STORE();                              // store the start value in the variable's id
  _pf.TRASH(4);

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_sweep_condition));
  node->variable()->accept(this, lvl);
  _pf.LOAD();
  node->end()->accept(this, lvl);

  if(node->sweepPlusFlag() == false)
    _pf.GE();
  else
    _pf.LE();
  _pf.JZ(mklbl(lbl_sweep_end));                // case both the numbers are equal (1), jump to end
  node->block()->accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_sweep_increment));
  node->variable()->accept(this, lvl);
  _pf.LOAD();
  node->increment()->accept(this, lvl);

  if(node->sweepPlusFlag() == true)
    _pf.ADD();
  else
    _pf.SUB();

  _pf.DUP();
  node->variable()->accept(this, lvl);
  _pf.STORE();
  _pf.TRASH(4);
  _pf.JMP(mklbl(lbl_sweep_condition));


  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_sweep_end));                    // finish the sweep cycle
}

void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.JMP(mklbl(_lbl_sweep_end));
}

void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.JMP(mklbl(_lbl_sweep_increment));
}

void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.JMP(mklbl(_thisFunctionEnd));
}

void xpl::postfix_writer::do_indexer_node(xpl::indexer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->firstElement()->accept(this, lvl);
  node->secondElement()->accept(this, lvl);
  _pf.INT(node->firstElement()->type()->subtype()->size());
  _pf.MUL();
  _pf.ADD();
  _pf.LOAD();
}

void xpl::postfix_writer::do_body_node(xpl::body_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->blockNode()->accept(this, lvl);
}

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->declarations())
    node->declarations()->accept(this, lvl);
  if(node->instructions())
    node->instructions()->accept(this, lvl);
}

void xpl::postfix_writer::do_vardecl_node(xpl::vardecl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *(node->varname());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if(!symbol)
    return;

  if(_thisFunction){     // locally
    // declarate arguments, followed by local variables

    if(_thisVarOffset >= 8){                          // IP (function arguments)
      symbol->offset(_thisVarOffset);
      _thisVarOffset += node->type()->size();         // offset occupied by the function argument being declared
    } 
    else if(_thisVarOffset <= 0){                     // FP  (function local variables)
      _thisVarOffset -= node->type()->size();
      symbol->offset(_thisVarOffset);                 // offset occupied by the local variable being declared
    }

    symbol->isGlobal(false);

    if(node->varvalue()){
      node->varvalue()->accept(this, lvl);
        
      if(node->type()->name() == basic_type::TYPE_DOUBLE){
        _pf.LOCAL(symbol->offset());
        _pf.DSTORE();
      }
      else{               // int/string/pointer
         _pf.LOCA(symbol->offset());
      }
    }
  }

  else{
    symbol->isGlobal(true);
    if(node->useFlag()){
      _pf.EXTERN(id);
      return;
    }
      
    if(node->varvalue()){
      if(node->varvalue()->type()->name() == basic_type::TYPE_STRING)
        node->varvalue()->accept(this, lvl);
      _pf.DATA();
    }
    else
      _pf.BSS();

    _pf.GLOBAL(*node->varname(), _pf.OBJ());
    
    _pf.ALIGN();
    if(node->type()->name() != basic_type::TYPE_STRING)
      _pf.GLOBAL(*node->varname(), _pf.OBJ());

    _pf.LABEL(id);

    if(node->type()->name() == basic_type::TYPE_STRING)
      _pf.ID(mklbl(_lbl));
    _thisId = &id;                           // used to identify all possible literals' labels (int/string/double)

    if(node->varvalue()){
      if(node->type()->name() != basic_type::TYPE_STRING){
        node->varvalue()->accept(this, lvl);
      }
    }
    else
      _pf.BYTE(node->type()->size());
  } 

}


void xpl::postfix_writer::do_fundecl_node(xpl::fundecl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *(node->funName());

  if(node->useFlag() == true || id == "argc" || id == "argv" || id == "envp"){
        _specialFunctionsVector.push_back(*(node->funName()));
  }
}

void xpl::postfix_writer::do_fundef_node(xpl::fundef_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  xpl::body_args_size * visitor = new xpl::body_args_size(_compiler);     // new visitor that calculates total size of variables to declare within function body
  std::string id = *(node->funName());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);
  if(symbol){
    if(id == "argc" || id == "argv" || id == "envp"){
      throw "Error: cannot redefine RTS functions (argc/argv/envp)";
    }
    else{
      for(size_t i = 0; i < _specialFunctionsVector.size(); i++){
        if(_specialFunctionsVector[i] == id){
          _specialFunctionsVector.pop_back();
        }
      }
    }
  }

  //redefine xpl id
  if(id == "xpl")
    id = "_main";
  else if(id == "main")
    id = ".main";

  int returnValueSize = 0;
  int variable_alloc_size = 0;
  _thisFunction = _symtab.find_local(*(node->funName()));
  _thisFunctionEnd = ++_lbl;


  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL(id, _pf.FUNC());
  _pf.LABEL(id);
  
  // -------------------------------------------------------------------
  // ---------------------------- ARGUMENTS ----------------------------
  // -------------------------------------------------------------------

  _symtab.push();                             // change of context must be done before declaring the function arguments
  if(node->arguments()) {
    _thisVarOffset = 8;  // arguments offset (IP) -- after offset 8, reside the function arguments
    node->arguments()->accept(this, lvl);
  }
  
  _thisVarOffset = 0; // local variables offset (FP)

  // -------------------------------------------------------------------
  // ------------------------- LOCAL VARIABLES -------------------------
  // -------------------------------------------------------------------

  // calculate this function's local variables sizes
  node->accept(visitor, lvl);
  variable_alloc_size = visitor->allocSize();
  delete visitor;
  _pf.ENTER(variable_alloc_size);
  //_symtab.push();                       // when the vars are gonna be initialized, push the context. you can now "redeclare" a variable
  
  returnValueSize = node->type()->size();
  if(node->returnValue()){

    node->returnValue()->accept(this, lvl);
    if(returnValueSize == 8){
      if(node->returnValue()->type()->name() == basic_type::TYPE_INT){
        _pf.I2D();
      }
      else if(node->returnValue()->type()->name() == basic_type::TYPE_DOUBLE){
        // all cool
      }
      _pf.LOCAL(-8);
      _pf.DSTORE();
    }
    else if(returnValueSize == 4){        // every other case (int/string/pointer)
      _pf.LOCA(-4);
    } 
  } 
  else{
    if(returnValueSize == 8){
      //_pf.INT(8);     // return default for int or double functions is 0
      //_pf.LOCAL(-node->type()->size());
      //_pf.STORE();
    }
    else if(node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_POINTER){
      _pf.INT(4);     // return default for int or double functions is 0
      _pf.LOCA(-node->type()->size());
    }
    else if(node->type()->name() == basic_type::TYPE_STRING)   // string don't have default values
      _pf.LOCAL(-4);      // string return unitialized (to be initialized after)
    else if(node->type()->name() == basic_type::TYPE_VOID){         // string don't have default values
      // "_pf.LOCAL(0)" (doesn't do anything)                       // return default is null
    }
  }
  
  node->body()->accept(this, lvl);
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(_thisFunctionEnd));

  if(returnValueSize == 4) {
    _pf.LOCAL(-4);
    _pf.LOAD();
    _pf.POP();                        // specify return value (eax)
  } else if(returnValueSize == 8){
    _pf.LOCAL(-8);
    _pf.DLOAD();
    _pf.DPOP();                       // specify return value (st0)
  }

  _pf.LEAVE();
  _pf.RET();

  _thisFunction = nullptr;                      // we are leaving the function context, so now the symbol we are in is null (= global context)
  _symtab.pop();                                // pop from the context
}

void xpl::postfix_writer::do_funcall_node(xpl::funcall_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *(node->funName());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);
  int argumentsSize = 0;

  if(node->arguments()){
    for (int i = node->arguments()->nodes().size() - 1; i >= 0; i--){   // putting args in the stack in reverse order to prepare the function call
      node->arguments()->node(i)->accept(this, lvl);
      if(((cdk::expression_node*) node->arguments()->node(i))->type()->name() == symbol->arguments()[i]->name()){
        // nothing to be done, everything alright (types are good already)
      }
      else{
        if(((cdk::expression_node*) node->arguments()->node(i))->type()->name() == basic_type::TYPE_INT 
          && symbol->arguments()[i]->name() == basic_type::TYPE_DOUBLE){
            _pf.I2D();
            argumentsSize += 4;       // transforming int to double will change args size
        }
      }
      argumentsSize += ((cdk::expression_node*) node->arguments()->node(i))->type()->size();
    }
  }
  
  _pf.CALL(id);

  if(argumentsSize != 0)
    _pf.TRASH(argumentsSize);
  
  if(symbol->type()->name() == basic_type::TYPE_VOID){
    // bananas 
  }
  else if(symbol->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DPUSH();
  }
  else{                         // int/string/pointer
    _pf.PUSH();
  } 
}

void xpl::postfix_writer::do_memaddr_node(xpl::memaddr_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_memalloc_node(xpl::memalloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);

  _pf.INT(node->argument()->type()->size());
  _pf.MUL();  
  _pf.ALLOC();
  _pf.SP();
}

void xpl::postfix_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);        // done xD
}

void xpl::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_do_and = ++_lbl;
  int lbl_and_end = ++_lbl;
  
  node->left()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_do_and));
  node->right()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_do_and));
  _pf.INT(1);                                  // return 1 (true) se ambos os argumentos forem verdadeiros
  _pf.JMP(mklbl(lbl_and_end));

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_do_and));
  _pf.INT(0);                                  // return 0 (false) se um dos argumentos NAO for verdadeiro

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_and_end));
}

void xpl::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_do_or = ++_lbl;
  int lbl_or_end = ++_lbl;
  
  node->left()->accept(this, lvl);
  _pf.JNZ(mklbl(lbl_do_or));                   // se for 1 (true), entao aceita logo diretamente (devolve 1 - true)
  node->right()->accept(this, lvl);
  _pf.JNZ(mklbl(lbl_do_or));                   // sendo o primeiro 0 (false), se o proximo for 1 (true) devolve 1 
  _pf.INT(0);                                  // else, se nenhum for 1 (true), devolve 0 (false)
  _pf.JMP(mklbl(lbl_or_end));

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_do_or));
  _pf.INT(1);

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_or_end));
}

void xpl::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_do_not = ++_lbl;
  int lbl_not_end = ++_lbl;
  
  node->argument()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_do_not));          // se o argumento devolver 1 (true), mudar o retorno para 0 (false)
  _pf.INT(0);
  _pf.JMP(mklbl(lbl_not_end));

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_do_not));       // se o argumento devolver 0 (false), mudar o retorno para 1 (true)
  _pf.INT(1);

  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl_not_end));
}