#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void xpl::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void xpl::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}

void xpl::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void xpl::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_print_node(xpl::print_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_read_node(xpl::read_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_while_node(xpl::while_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);

  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_if_node(xpl::if_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);

  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);

  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);

  closeTag(node, lvl);
}

// new methods
//---------------------------------------------------------------------------

void xpl::xml_writer::do_sweep_node(xpl::sweep_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("sweepPlusFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->sweepPlusFlag() ? "true" : "false") << std::endl;
  closeTag("sweepPlusFlag", lvl + 2);

  openTag("variable", lvl + 2);
  node->variable()->accept(this, lvl + 4);
  closeTag("variable", lvl + 2);

  openTag("start", lvl + 2);
  node->start()->accept(this, lvl + 4);
  closeTag("start", lvl + 2);

  openTag("end", lvl + 2);
  node->end()->accept(this, lvl + 4);
  closeTag("end", lvl + 2);

  openTag("increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);

  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_next_node(xpl::next_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_return_node(xpl::return_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_block_node(xpl::block_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("declarations", lvl + 2);
  node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);

  openTag("instructions", lvl + 2);
  node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_indexer_node(xpl::indexer_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("firstElement", lvl + 2);
  node->firstElement()->accept(this, lvl + 4);
  closeTag("firstElement", lvl + 2);

  openTag("secondElement", lvl + 2);
  node->secondElement()->accept(this, lvl + 4);
  closeTag("secondElement", lvl + 2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_vardecl_node(xpl::vardecl_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("type", lvl + 2);
  typeToString(node->type(), lvl + 4);
  closeTag("type", lvl + 2);

  openTag("varname", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *(node->varname()) << std::endl;
  closeTag("varname", lvl + 2);

  openTag("useFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->useFlag() ? "true" : "false") << std::endl;
  closeTag("useFlag", lvl + 2);

  openTag("publicFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->publicFlag() ? "true" : "false") << std::endl;
  closeTag("publicFlag", lvl + 2);

  if(node->varvalue() != nullptr){
    openTag("varvalue", lvl + 2);
    node->varvalue()->accept(this, lvl + 4);
    closeTag("varvalue", lvl + 2);
  }

  closeTag(node, lvl);
}

void xpl::xml_writer::do_body_node(xpl::body_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->blockNode()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_fundecl_node(xpl::fundecl_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("type", lvl + 2);
  typeToString(node->type(), lvl + 4);
  closeTag("type", lvl + 2);

  openTag("funName", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *(node->funName()) << std::endl;
  closeTag("funName", lvl + 2);

  openTag("useFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->useFlag() ? "true" : "false") << std::endl;
  closeTag("useFlag", lvl + 2);

  openTag("publicFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->publicFlag() ? "true" : "false") << std::endl;
  closeTag("publicFlag", lvl + 2);

  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_fundef_node(xpl::fundef_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("type", lvl + 2);
  typeToString(node->type(), lvl + 4);
  closeTag("type", lvl + 2);

  openTag("funName", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *(node->funName()) << std::endl;
  closeTag("funName", lvl + 2);

  openTag("publicFlag", lvl + 2);
  os() << std::string(lvl + 4, ' ') << (node->publicFlag() ? "true" : "false") << std::endl;
  closeTag("publicFlag", lvl + 2);

  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);

  if(node->returnValue() != nullptr){
    openTag("returnValue", lvl + 2);
    node->returnValue()->accept(this, lvl + 4);
    closeTag("returnValue", lvl + 2);
  }

  openTag("body", lvl + 2);
  node->body()->accept(this, lvl + 4);
  closeTag("body", lvl + 2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_funcall_node(xpl::funcall_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("funName", lvl + 2);
  os() << std::string(lvl + 4, ' ') << *(node->funName()) << std::endl;
  closeTag("funName", lvl + 2);

  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);

  closeTag(node, lvl);
}

void xpl::xml_writer::do_memaddr_node(xpl::memaddr_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

void xpl::xml_writer::do_memalloc_node(xpl::memalloc_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

void xpl::xml_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

void xpl::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

void xpl::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

void xpl::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
