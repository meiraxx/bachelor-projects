#ifndef __xpl_SEMANTICS_body_args_size_H__
#define __xpl_SEMANTICS_body_args_size_H__

#include <string>
#include <iostream>
#include <cdk/compiler.h>
#include "targets/basic_ast_visitor.h"

namespace xpl {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class body_args_size: public basic_ast_visitor {
	int _allocSize = 0;
	
  public:
    body_args_size(std::shared_ptr<cdk::compiler> compiler)
      :basic_ast_visitor(compiler) {

    }
    
    ~body_args_size(){
      os().flush();
    }
    
  public:
    int allocSize(){
      return _allocSize; 
    }

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl){}
    void do_double_node(cdk::double_node * const node, int lvl){}
    void do_string_node(cdk::string_node * const node, int lvl){}

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl){}

  public:
    void do_add_node(cdk::add_node * const node, int lvl){}
    void do_sub_node(cdk::sub_node * const node, int lvl){}
    void do_mul_node(cdk::mul_node * const node, int lvl){}
    void do_div_node(cdk::div_node * const node, int lvl){}
    void do_mod_node(cdk::mod_node * const node, int lvl){}
    void do_lt_node(cdk::lt_node * const node, int lvl){}
    void do_le_node(cdk::le_node * const node, int lvl){}
    void do_ge_node(cdk::ge_node * const node, int lvl){}
    void do_gt_node(cdk::gt_node * const node, int lvl){}
    void do_ne_node(cdk::ne_node * const node, int lvl){}
    void do_eq_node(cdk::eq_node * const node, int lvl){}

  public:
    void do_identifier_node(cdk::identifier_node * const node, int lvl){}
    void do_rvalue_node(cdk::rvalue_node * const node, int lvl){}
    void do_assignment_node(cdk::assignment_node * const node, int lvl){}

  public:
    void do_evaluation_node(xpl::evaluation_node * const node, int lvl){}
    void do_print_node(xpl::print_node * const node, int lvl){}
    void do_read_node(xpl::read_node * const node, int lvl){}

    // new methods
    void do_stop_node(xpl::stop_node * const node, int lvl){}
    void do_next_node(xpl::next_node * const node, int lvl){}
    void do_return_node(xpl::return_node * const node, int lvl){}
    void do_indexer_node(xpl::indexer_node * const node, int lvl){}
    void do_fundecl_node(xpl::fundecl_node * const node, int lvl){}
    void do_funcall_node(xpl::funcall_node * const node, int lvl){}
    void do_memaddr_node(xpl::memaddr_node * const node, int lvl){}
    void do_memalloc_node(xpl::memalloc_node * const node, int lvl){}
    void do_identity_node(xpl::identity_node * const node, int lvl){}
    void do_and_node(cdk::and_node * const node, int lvl){}
    void do_or_node(cdk::or_node * const node, int lvl){}
    void do_not_node(cdk::not_node * const node, int lvl){}
    
    // used here to calculate size
    void do_fundef_node(xpl::fundef_node * const node, int lvl);
    void do_body_node(xpl::body_node * const node, int lvl);
    void do_block_node(xpl::block_node * const node, int lvl);
    void do_sequence_node(cdk::sequence_node * const node, int lvl);
    void do_vardecl_node(xpl::vardecl_node * const node, int lvl);
    void do_if_node(xpl::if_node * const node, int lvl);
    void do_if_else_node(xpl::if_else_node * const node, int lvl);
    void do_while_node(xpl::while_node * const node, int lvl);
    void do_sweep_node(xpl::sweep_node * const node, int lvl);
  };

} // xpl

#endif