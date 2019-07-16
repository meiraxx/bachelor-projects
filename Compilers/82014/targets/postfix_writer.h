#ifndef __XPL_SEMANTICS_PF_WRITER_H__
#define __XPL_SEMANTICS_PF_WRITER_H__

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/body_args_size.h"
#include "targets/symbol.h"

namespace xpl {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<xpl::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;

    // functions/vardecls variables
    std::string *_thisId;                       // the global id in which we are going to place the value of the variable

    std::shared_ptr<symbol> _thisFunction;      // the function where a variable or/and its value are being declared

    int _thisFunctionEnd;                       // global variable so that 'return' knows where to jump back after being called

    int _thisVarOffset = 0;                     // this represents the current offset of BOTH the FramePointer and InstructionPointer at multiple lines of postfix_writer.cpp in a function


    // global sweep variables:
    int _lbl_sweep_increment;
    int _lbl_sweep_end;

    // specialFunctionsVector
    public:
      std::vector<std::string> _specialFunctionsVector;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<xpl::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }
    /*
    inline int call_new_visitor(xpl::fundef_node * const node, int lvl){
      int alloc_size = 0;

      xpl::body_args_size * visitor = new xpl::body_args_size(_compiler);     // new visitor that calculates total size of variables to declare within function body
      node->accept(visitor, lvl);
      alloc_size = visitor->allocSize();
      delete visitor;
    }*/

  public:
    void do_sequence_node(cdk::sequence_node * const node, int lvl);

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);

  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);

  public:
    void do_identifier_node(cdk::identifier_node * const node, int lvl);
    void do_rvalue_node(cdk::rvalue_node * const node, int lvl);
    void do_assignment_node(cdk::assignment_node * const node, int lvl);

  public:
    void do_evaluation_node(xpl::evaluation_node * const node, int lvl);
    void do_print_node(xpl::print_node * const node, int lvl);
    void do_read_node(xpl::read_node * const node, int lvl);

  public:
    void do_while_node(xpl::while_node * const node, int lvl);
    void do_if_node(xpl::if_node * const node, int lvl);
    void do_if_else_node(xpl::if_else_node * const node, int lvl);
    // new methods
    void do_sweep_node(xpl::sweep_node * const node, int lvl);
    void do_stop_node(xpl::stop_node * const node, int lvl);
    void do_next_node(xpl::next_node * const node, int lvl);
    void do_return_node(xpl::return_node * const node, int lvl);
    void do_block_node(xpl::block_node * const node, int lvl);
    void do_indexer_node(xpl::indexer_node * const node, int lvl);
    void do_vardecl_node(xpl::vardecl_node * const node, int lvl);
    void do_body_node(xpl::body_node * const node, int lvl);
    void do_fundecl_node(xpl::fundecl_node * const node, int lvl);
    void do_fundef_node(xpl::fundef_node * const node, int lvl);
    void do_funcall_node(xpl::funcall_node * const node, int lvl);
    void do_memaddr_node(xpl::memaddr_node * const node, int lvl);
    void do_memalloc_node(xpl::memalloc_node * const node, int lvl);
    void do_identity_node(xpl::identity_node * const node, int lvl);
    void do_and_node(cdk::and_node * const node, int lvl);
    void do_or_node(cdk::or_node * const node, int lvl);
    void do_not_node(cdk::not_node * const node, int lvl);


  };

} // xpl

#endif