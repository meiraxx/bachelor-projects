#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

#define DEBUG_LOG
#ifdef DEBUG_LOG
#  define LOG(msg) std::cout << msg << std::endl
#else
#  define LOG(msg)
#endif

//---------------------------------------------------------------------------

void xpl::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void xpl::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processUnaryExpressionSignal(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  // case read
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC){
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  }

  if(node->argument()->type()->name() != basic_type::TYPE_INT){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Unary: both identity(+) and symetric(-) operations expect an argument of type int.");
  }

  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpressionSignal(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processBinaryExpressionAdditive(cdk::binary_expression_node * const node, int lvl) { // binary expressions: add some cases
  // initial ops already being done before we call this method

  if(node->left()->type()->name() == basic_type::TYPE_ERROR || node->right()->type()->name() == basic_type::TYPE_ERROR
    || node->left()->type()->name() == basic_type::TYPE_VOID || node->right()->type()->name() == basic_type::TYPE_VOID
    || node->left()->type()->name() == basic_type::TYPE_STRING|| node->right()->type()->name() == basic_type::TYPE_STRING){
      node->type(new basic_type(0, basic_type::TYPE_ERROR));
      throw std::string("Binary: cannot add or sub strings or voids. Error.");
  }
  // double com int e int com double
  if((node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    || (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE))
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  // double com double
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) 
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  // int com int
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT)
        node->type(new basic_type(4, basic_type::TYPE_INT));
  // pointers com ints
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT){
        node->type(new basic_type(4, basic_type::TYPE_POINTER));
        node->type()->_subtype = node->left()->type()->subtype();
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER){
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = node->right()->type()->subtype();
  }

  // case of read (int/double)
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
      node->left()->type(node->right()->type());
      node->type(node->right()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
      node->right()->type(node->left()->type());
      node->type(node->left()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT){
    node->left()->type(node->right()->type());
    node->type(node->right()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
    node->right()->type(node->left()->type());
    node->type(node->left()->type());
  }

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Binary: incompatible types in add or sub";
  }
}

inline void xpl::type_checker::processBinaryExpressionMultiplicative(cdk::binary_expression_node * const node, int lvl) { // binary expressions: add some cases
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() == basic_type::TYPE_ERROR || node->right()->type()->name() == basic_type::TYPE_ERROR
  || node->left()->type()->name() == basic_type::TYPE_VOID || node->right()->type()->name() == basic_type::TYPE_VOID
  || node->left()->type()->name() == basic_type::TYPE_STRING || node->right()->type()->name() == basic_type::TYPE_STRING
  || node->left()->type()->name() == basic_type::TYPE_POINTER || node->right()->type()->name() == basic_type::TYPE_POINTER){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Binary: cannot mul or div strings, pointers or voids. Error.");
  }

  // double * (int || double) = double
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  // int * int = int
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  // case of read (int/double)
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
      node->left()->type(node->right()->type());
      node->type(node->right()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
      node->right()->type(node->left()->type());
      node->type(node->left()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT){
    node->left()->type(node->right()->type());
    node->type(node->right()->type());
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
    node->right()->type(node->left()->type());
    node->type(node->left()->type());
  }
  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Binary: incompatible types in mul or div";
  }
}

inline void xpl::type_checker::processBinaryExpressionEquality(cdk::binary_expression_node * const node, int lvl) { // binary expressions: add some cases
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() == basic_type::TYPE_ERROR || node->right()->type()->name() == basic_type::TYPE_ERROR
  || node->left()->type()->name() == basic_type::TYPE_VOID || node->right()->type()->name() == basic_type::TYPE_VOID
  || node->left()->type()->name() == basic_type::TYPE_STRING || node->right()->type()->name() == basic_type::TYPE_STRING){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Binary: cannot compare void or string types");
  }

  // no booleans in xpl, so it must de done with 0(false) and !0(true), i.e., an int type
  // case of read (int/double)
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
      node->left()->type(node->right()->type());
      node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
      node->right()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT){
    node->left()->type(node->right()->type());
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
    node->right()->type(node->left()->type());
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }

  else if((node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER)
      || (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
      || (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT)
      || (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
      || (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE))
          node->type(new basic_type(4, basic_type::TYPE_INT));

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Binary: incompatible types in mul or div";
  }
}

inline void xpl::type_checker::processBinaryExpressionComparative(cdk::binary_expression_node * const node, int lvl) { // binary expressions: add some cases
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() == basic_type::TYPE_ERROR || node->right()->type()->name() == basic_type::TYPE_ERROR
  || node->left()->type()->name() == basic_type::TYPE_VOID || node->right()->type()->name() == basic_type::TYPE_VOID
  || node->left()->type()->name() == basic_type::TYPE_STRING || node->right()->type()->name() == basic_type::TYPE_STRING
  || node->left()->type()->name() == basic_type::TYPE_POINTER || node->right()->type()->name() == basic_type::TYPE_POINTER){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Binary: cannot compare void or string types");
  }

  // no booleans in xpl, so it must de done with 0(false) and !0(true), i.e., an int type
  // case of read (int/double)
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
      node->left()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
      node->right()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT){
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }

  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE
      || node->left()->type()->name() == basic_type::TYPE_INT || node->right()->type()->name() == basic_type::TYPE_INT)
          node->type(new basic_type(4, basic_type::TYPE_INT));

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Binary: incompatible types in mul or div";
  }
}

inline void xpl::type_checker::processBinaryExpressionLogical(cdk::binary_expression_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  // case read
  if(node->left()->type()->name() == basic_type::TYPE_UNSPEC)
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));

  // case read
  if(node->right()->type()->name() == basic_type::TYPE_UNSPEC)
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));    // 0 ou 1

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Binary: or/and must take 2 ints to perform");
  }
}

// arithmetic binary expressions
void xpl::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  processBinaryExpressionAdditive(node, lvl);
}
void xpl::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  // pointers with pointers --> sub case only
  if(node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else
    processBinaryExpressionAdditive(node, lvl);
}
void xpl::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpressionMultiplicative(node, lvl);
}
void xpl::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpressionMultiplicative(node, lvl);
}
void xpl::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) { // resto da divisao inteira (facil --> entre dois inteiros)
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->type()->name() != basic_type::TYPE_INT || node->right()->type()->name() != basic_type::TYPE_INT){ 
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Binary: modulo operation must be provided with 2 integers to perform");
  }
  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

//comparison binary expressions
void xpl::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpressionComparative(node, lvl);
}
void xpl::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpressionComparative(node, lvl);
}
void xpl::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpressionComparative(node, lvl);
}
void xpl::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpressionComparative(node, lvl);
}
void xpl::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpressionEquality(node, lvl);
}
void xpl::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpressionEquality(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if (symbol){
    node->type(symbol->type());
  }
  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Identifier " + id + " undeclared";
  }
}

void xpl::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try{
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    // print already being made on identifier
  }
}

void xpl::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);

  // cannot assign procedures and errors. If lvalue has unspecified type the type is unknown too: error.
  if(node->lvalue()->type()->name() == basic_type::TYPE_ERROR || node->rvalue()->type()->name() == basic_type::TYPE_ERROR
  // probably just necessary to check right value because it's the only one that can have a procedure since it can be a function call, but just in case something goes wrong
  || node->lvalue()->type()->name() == basic_type::TYPE_VOID || node->rvalue()->type()->name() == basic_type::TYPE_VOID
  || node->lvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Variable has no type in assignment";
  }

  // casts
  else if(node->lvalue()->type()->name()== basic_type::TYPE_INT && node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    node->type(node->lvalue()->type());
  }
  else if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    node->type(node->lvalue()->type());
  }
  else if(node->lvalue()->type()->name() == basic_type::TYPE_STRING && node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    node->rvalue()->type(new basic_type(4, basic_type::TYPE_STRING));
    node->type(node->lvalue()->type());
  }
  else if(node->lvalue()->type()->name() == basic_type::TYPE_POINTER && node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    // may be a pointer to a read (unspec case)
    if(node->lvalue()->type()->subtype()->name() == basic_type::TYPE_UNSPEC || node->lvalue()->type()->subtype()->name() == basic_type::TYPE_INT 
    || node->lvalue()->type()->subtype()->name() == basic_type::TYPE_DOUBLE || node->lvalue()->type()->subtype()->name() == basic_type::TYPE_STRING){
      // everything alright, pointer's subtype is of a xpl valid type
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type(node->lvalue()->type());
    }
    else{
      node->rvalue()->type(new basic_type(0, basic_type::TYPE_ERROR));
      throw "Assignment: pointer does not have a valid subtype.";
    }
  }

  // an assignment between a pointer and an int is a pointer
  if(node->lvalue()->type()->name() == basic_type::TYPE_POINTER && node->rvalue()->type()->name() == basic_type::TYPE_INT)
    node->type(node->lvalue()->type());

  // an assignment between a real and an int is a real
  else if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_INT)
    node->type(node->lvalue()->type());

  // else, if the types are not the same after all the previous exceptions: error
  else if(node->lvalue()->type()->name() != node->rvalue()->type()->name()){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Assignment between incompatible matches";
  } 

  else{
    node->type(node->lvalue()->type());
  }
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {  // ';' --> nothing to type
  node->argument()->accept(this, lvl + 2);    // just call the type checker in the argument (i.e., expression before ';')
}

void xpl::type_checker::do_print_node(xpl::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);

  if(node->argument()->type()->name() == basic_type::TYPE_POINTER)
    throw "Cannot print type pointer";
  else if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    throw "Cannot print unspecified type";
  else if(node->argument()->type()->name() == basic_type::TYPE_ERROR)
    throw "Cannot print type error";
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  // impossible to know in compile-time, only in run-time.
  node->type(new basic_type(8, basic_type::TYPE_UNSPEC)); // can be of type int or real, so we should instatiate the type with the biggest sizeof, a.k.a. double
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_while_node(xpl::while_node * const node, int lvl) {  // nothing to type, just check the types in its only typeable argument
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_if_node(xpl::if_node * const node, int lvl) {    // nothing to type, just check the types in its only typeable argument
  node->condition()->accept(this, lvl + 4);
}

void xpl::type_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {   // nothing to type, just check the types in its only typeable argument
  node->condition()->accept(this, lvl + 4);
}

// new methods
//---------------------------------------------------------------------------

void xpl::type_checker::do_sweep_node(xpl::sweep_node * const node, int lvl) {  // nothing to type, just check the types in its typeable arguments

  node->variable()->accept(this, lvl + 2);
  node->start()->accept(this, lvl + 2);
  node->end()->accept(this, lvl + 2);
  node->increment()->accept(this, lvl + 2);

  // start type must be first determined in order to determine variable type (assignment -> evaluation from the right to the left)

  // --- start value type check ---
  if(node->start()->type()->name() == basic_type::TYPE_UNSPEC)
    node->start()->type(new basic_type(4, basic_type::TYPE_INT));
  else if(node->start()->type()->name() == basic_type::TYPE_DOUBLE || node->start()->type()->name() == basic_type::TYPE_INT){
    // everything alright already, proceed
  }
  else{
    node->start()->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Sweep: starting value type must be either an int or a double";
  }


  // --- iterator variable type check ---
  if((node->variable()->type()->name() == basic_type::TYPE_UNSPEC || node->variable()->type()->name() == basic_type::TYPE_INT) 
    && node->start()->type()->name() == basic_type::TYPE_INT)
    node->variable()->type(new basic_type(4, basic_type::TYPE_INT));

  else if(((node->variable()->type()->name() == basic_type::TYPE_UNSPEC || node->variable()->type()->name() == basic_type::TYPE_INT
    || node->variable()->type()->name() == basic_type::TYPE_DOUBLE) && node->start()->type()->name() == basic_type::TYPE_DOUBLE)
    || (node->variable()->type()->name() == basic_type::TYPE_DOUBLE && node->start()->type()->name() == basic_type::TYPE_INT))
      node->variable()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else{
    node->variable()->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Sweep: iterator variable type must be a either an int or a double and it must have a compatible type with the type of the starting value";
  }


  // --- end value type check ---
  if(node->end()->type()->name() == basic_type::TYPE_UNSPEC)
    node->end()->type(new basic_type(4, basic_type::TYPE_INT));

  else if(node->end()->type()->name() == basic_type::TYPE_INT || node->end()->type()->name() == basic_type::TYPE_DOUBLE){
    //everything alright, proceed
  }
  else{
    node->end()->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Sweep: end value type must be either an int or a double";
  }


  // --- increment value type check ---
  if(node->increment()->type()->name() == basic_type::TYPE_UNSPEC)
    node->increment()->type(new basic_type(4, basic_type::TYPE_INT));

  else if(node->increment()->type()->name() == basic_type::TYPE_INT || node->increment()->type()->name() == basic_type::TYPE_DOUBLE){
    // everything alright, proceed
  }
  else{
    node->increment()->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Sweep: increment value type must be either an int or a double";
  }

}

void xpl::type_checker::do_stop_node(xpl::stop_node * const node, int lvl) { //nothing to be done here (check if it's in cycle elsewhere)
}

void xpl::type_checker::do_next_node(xpl::next_node * const node, int lvl) { //nothing to be done here (check if it's in cycle elsewhere)
}

void xpl::type_checker::do_return_node(xpl::return_node * const node, int lvl) { //nothing to be done here (check if it's last instruction in block elsewhere)
}

void xpl::type_checker::do_block_node(xpl::block_node * const node, int lvl) { //nothing to be done here
}

void xpl::type_checker::do_indexer_node(xpl::indexer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->firstElement()->accept(this, lvl + 2);
  node->secondElement()->accept(this, lvl + 2);

  //case read
  if(node->firstElement()->type()->name() != basic_type::TYPE_POINTER){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "The indexer identifier must be of type pointer";
  }

  if(node->secondElement()->type()->name() != basic_type::TYPE_INT){
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "The indexer offset must be of type int";
  }

  node->type(node->firstElement()->type()->subtype());          // subtipo apontado pelo ponteiro
}


void xpl::type_checker::do_vardecl_node(xpl::vardecl_node * const node, int lvl) {
  
  const std::string &id = *(node->varname());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(id);

  // checking in the same context
  if (!symbol) {          // se nao estiver declarada ainda neste contexto, declarar
    symbol = std::make_shared<xpl::symbol>(node->type(), id);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);
  }
  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Variable " + id + " redeclared";
  }

  // else, pode-se continuar pois é possível declarar uma variável com um nome igual num contexto diferente

  if (node->varvalue()) {
    node->varvalue()->accept(this, lvl+2);

    // cannot assign procedures and errors. If node has unspecified type the type is unknown too: error.
    if(node->type()->name() == basic_type::TYPE_VOID || node->varvalue()->type()->name() == basic_type::TYPE_VOID
      || node->type()->name() == basic_type::TYPE_ERROR || node->varvalue()->type()->name() == basic_type::TYPE_ERROR
      // just in case, although the node type shouldn't ever be unspec since it should always arrive from yacc when a new vardecl_node is instantiated
      || node->type()->name() == basic_type::TYPE_UNSPEC){  
        node->type(new basic_type(0, basic_type::TYPE_ERROR));
        throw "Variable declaration: variable has no type in assignment";
    }
    // casts
    else if(node->type()->name()== basic_type::TYPE_INT && node->varvalue()->type()->name() == basic_type::TYPE_UNSPEC)
      node->varvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    else if(node->type()->name() == basic_type::TYPE_DOUBLE && node->varvalue()->type()->name() == basic_type::TYPE_UNSPEC)
      node->varvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else if(node->type()->name() == basic_type::TYPE_STRING && node->varvalue()->type()->name() == basic_type::TYPE_UNSPEC)
      node->varvalue()->type(new basic_type(4, basic_type::TYPE_STRING));
    else if(node->type()->name() == basic_type::TYPE_POINTER && node->varvalue()->type()->name() == basic_type::TYPE_UNSPEC){
      // may be a pointer to a read (unspec case)
      node->varvalue()->type(new basic_type(4, basic_type::TYPE_POINTER));
    }

    // an assignment between a pointer and an int is a pointer
    if(node->type()->name() == basic_type::TYPE_POINTER && node->varvalue()->type()->name() == basic_type::TYPE_INT){
      // alright, accept
    }

    // an assignment between a real and an int is a real
    else if(node->type()->name() == basic_type::TYPE_DOUBLE && node->varvalue()->type()->name() == basic_type::TYPE_INT)
      node->type(node->type());

    // else, if the types are not the same after all the previous exceptions: error
    else if(node->type()->name() != node->varvalue()->type()->name()){
      node->type(new basic_type(0, basic_type::TYPE_ERROR));
      throw "Variable declaration: assignment between incompatible matches";
    }
    // else nothing to be done, everything alright: function node type is good
  }
}

void xpl::type_checker::do_body_node(xpl::body_node * const node, int lvl) { //nothing to be done here
}

void xpl::type_checker::do_fundecl_node(xpl::fundecl_node * const node, int lvl) {
  std::vector<basic_type*> arguments;
  std::string &func_id = *(node->funName());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(func_id);       // finding in the current context (general file)

  if(!symbol){
    //_symtab.push();
    // here we define the function symbol in the general file context: non-existance -> declaration
    symbol = std::make_shared<xpl::symbol>(node->type(), func_id);
    symbol->isVar(false);
    symbol->offset(-1);

    if(!node->arguments()->nodes().empty()){
      // storing all the argument types
      for(size_t i = 0; i < node->arguments()->nodes().size(); i++){
        xpl::vardecl_node *argument = (xpl::vardecl_node*) node->arguments()->node(i);
        argument->accept(this, lvl + 2);
        symbol->arguments(argument->type());
      }
    }
  
    _symtab.insert(func_id, symbol);
    _parent->set_new_symbol(symbol);
  }

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Redeclaration of: " + func_id;              //fixed problem
  }
}

void xpl::type_checker::do_fundef_node(xpl::fundef_node * const node, int lvl) {
      std::vector<basic_type*> arguments;
      std::string &func_id = *(node->funName());
      std::shared_ptr<xpl::symbol> symbol = _symtab.find(func_id);       // finding in the current context (general file)

      if(symbol){
        
        if(symbol->isVar()){
          throw "Variable with the same name is defined.";
        }        // verify if symbol is a function
          
        if(!symbol->arguments().empty()){
          // arguments already stored in declaration vs this definition
          if(symbol->arguments().size() != node->arguments()->nodes().size()){
            node->type(new basic_type(0, basic_type::TYPE_ERROR));
            throw "Function " + func_id + " already declared with a different number of arguments";
          }

          for(size_t i = 0; i < arguments.size(); i++){
            if(symbol->arguments().at(i)->name() != arguments.at(i)->name()){
              node->type(new basic_type(0, basic_type::TYPE_ERROR));
              throw func_id + " already declared with arguments of another type";
            }
          }
        }

        if(symbol->type()->name() != node->type()->name()){
          node->type(new basic_type(0, basic_type::TYPE_ERROR));
          throw "Function " + func_id + " already declared with another return type";
        }

        if(symbol->isDefined()){
          node->type(new basic_type(0, basic_type::TYPE_ERROR));
          throw "Function " + func_id + " already defined";
        }
        else{                                // if symbol is declared but not yet defined
          //_symtab.push();
          // como ja está declarada, não é necessário meter lá os tipos no simbolo

          if(node->returnValue()){     // verify return value type
            node->returnValue()->accept(this, lvl + 2);
            if(node->type()->name() == basic_type::TYPE_UNSPEC && node->returnValue()->type()->name() == basic_type::TYPE_DOUBLE){
              node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
            }
            else if(node->type()->name() == basic_type::TYPE_UNSPEC && node->returnValue()->type()->name() == basic_type::TYPE_INT){
              node->type(new basic_type(4, basic_type::TYPE_INT));
            }
            else if(node->type()->name() == basic_type::TYPE_UNSPEC && node->returnValue()->type()->name() == basic_type::TYPE_STRING){
              node->type(new basic_type(4, basic_type::TYPE_STRING));
            }
            else if(node->type()->name() == basic_type::TYPE_UNSPEC && node->returnValue()->type()->name() == basic_type::TYPE_POINTER){
              node->type(new basic_type(4, basic_type::TYPE_POINTER));
            }
            else if(node->type()->name() == basic_type::TYPE_DOUBLE && node->returnValue()->type()->name() == basic_type::TYPE_INT){
                // integer to double is alright
            }
            else if(node->type()->name() != node->returnValue()->type()->name()){
              node->type(new basic_type(0, basic_type::TYPE_ERROR));
              throw std::string("Function type does not match return value");
            }
            // else, se é igual, já está bem (inclui o caso do void)
          }

          // here we redefine the function symbol in the general file context: declaration --> definition
          symbol->isDefined(true);
          symbol->isVar(false);
          symbol->offset(-1);
        }
        
      }

      else{    // if symbol is not declared nor defined
        //_symtab.push();
        // here we define the function symbol in the general file context: non-existance --> definition
        std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(node->type(), func_id);
        symbol->isDefined(true);
        symbol->isVar(false);
        symbol->offset(-1);

        if(node->arguments()){
          for(size_t i = 0; i < node->arguments()->nodes().size(); i++){
            xpl::vardecl_node *argument = (xpl::vardecl_node*) node->arguments()->node(i);
            symbol->arguments(argument->type());
          }
        }

        if(node->returnValue()){
          node->returnValue()->accept(this, lvl + 2);
          if(node->type()->name() == basic_type::TYPE_DOUBLE && node->returnValue()->type()->name() == basic_type::TYPE_INT){
              // integer to double is alright
          } 
          else if(node->type()->name() != node->returnValue()->type()->name()){
            node->type(new basic_type(0, basic_type::TYPE_ERROR));
            throw std::string("Function type does not match return value");
          }
        }

        _symtab.insert(func_id, symbol);
        _parent->set_new_symbol(symbol);
      }
}

void xpl::type_checker::do_funcall_node(xpl::funcall_node * const node, int lvl) {
  ASSERT_UNSPEC;

  std::string &func_id = *(node->funName());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(func_id);       // iterating in all the existing contexts to find the function's symbol that is being called
  
  if(symbol){        // the ref manual says we can call declared functions aswell...
    std::vector<basic_type*> arguments = symbol->arguments(); // get type vector
    if(!node->arguments()->nodes().empty()){
      if(arguments.size() != node->arguments()->nodes().size()){
          node->type(new basic_type(0, basic_type::TYPE_ERROR));
          throw "Function " + func_id + " called with a different number of arguments than when it was declared/defined";
      }

      // attributing the types from the type vector to each argument's node
      // for cycle gives us assurance that the types are being verified in the correct order (as they were also inserted in the same order)
      // postfix_writer, note: will have to start reading the arguments from the end and insert those in the stack from there (aka, inverse order)
      for(size_t i = 0; i < arguments.size(); i++){
        cdk::expression_node *argument = (cdk::expression_node*) node->arguments()->node(i);
        node->arguments()->node(i)->accept(this, lvl + 2);

        if(arguments.at(i)->name() == basic_type::TYPE_ERROR || arguments.at(i)->name() == basic_type::TYPE_VOID){
          node->type(new basic_type(0, basic_type::TYPE_ERROR));
          throw "Variable has no type in assignment";
        }

        // the function arguments don't have limitations, they can be all xpl types -> d, i, str and ptr
        else if(argument->type()->name() == basic_type::TYPE_UNSPEC && arguments.at(i)->name() == basic_type::TYPE_DOUBLE)
          argument->type(new basic_type(8, basic_type::TYPE_DOUBLE));

        else if(argument->type()->name() == basic_type::TYPE_UNSPEC && arguments.at(i)->name() == basic_type::TYPE_INT)
          argument->type(new basic_type(4, basic_type::TYPE_INT));

        else if(argument->type()->name() == basic_type::TYPE_UNSPEC && arguments.at(i)->name() == basic_type::TYPE_STRING)
          argument->type(new basic_type(4, basic_type::TYPE_STRING));

        else if(argument->type()->name() == basic_type::TYPE_UNSPEC && arguments.at(i)->name() == basic_type::TYPE_POINTER)
          argument->type(new basic_type(4, basic_type::TYPE_POINTER));
        else if(argument->type()->name() == basic_type::TYPE_DOUBLE && arguments.at(i)->name() == basic_type::TYPE_DOUBLE)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_INT && arguments.at(i)->name() == basic_type::TYPE_INT)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_STRING && arguments.at(i)->name() == basic_type::TYPE_STRING)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_POINTER && arguments.at(i)->name() == basic_type::TYPE_POINTER)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_INT && arguments.at(i)->name() == basic_type::TYPE_POINTER)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_POINTER && arguments.at(i)->name() == basic_type::TYPE_INT)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_INT && arguments.at(i)->name() == basic_type::TYPE_DOUBLE)
          continue;
        else if(argument->type()->name() == basic_type::TYPE_DOUBLE && arguments.at(i)->name() == basic_type::TYPE_INT)
          continue;
        else if(argument->type()->name() != arguments.at(i)->name()){
          throw "Calling function " + func_id + ": function declared with different argument types";
        }
      }
    }

    node->type(symbol->type());
  } 
  else{
    throw "Function " + func_id + " undeclared";
  }
}

void xpl::type_checker::do_memaddr_node(xpl::memaddr_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  // just to make sure this node accepts lvalues only (can either be: an identifier or an indexer), accept only UNSPEC (and make it pointer), or POINTER

  if(node->argument()->type()->name() == basic_type::TYPE_POINTER || node->argument()->type()->name() == basic_type::TYPE_INT
    || node->argument()->type()->name() == basic_type::TYPE_DOUBLE || node->argument()->type()->name() == basic_type::TYPE_STRING){
    node->type(new basic_type(4, basic_type::TYPE_INT));          // endereco
  }

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Memory addressing can only receive as an argument a pointer";
  }
}

void xpl::type_checker::do_memalloc_node(xpl::memalloc_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  // case read
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC){
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  }

  if(node->argument()->type()->name() == basic_type::TYPE_INT ){
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else{
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw "Memory allocation's argument must be an integer";
  }
}

void xpl::type_checker::do_identity_node(xpl::identity_node * const node, int lvl) {
  processUnaryExpressionSignal(node, lvl);
}

void xpl::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpressionLogical(node, lvl);
}

void xpl::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpressionLogical(node, lvl);
}

void xpl::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  // case read
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC){
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  }

  if (node->argument()->type()->name() != basic_type::TYPE_INT) {
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("Unary: the not(~) operation expects an int as its argument");
  }

  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}