// $Id: memaddr_node.h,v 1.3 2017/04/06 14:12:48 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_MEMADDRNODE_H__
#define __XPL_MEMADDRNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing memory addressing nodes.
   */
  class memaddr_node: public cdk::unary_expression_node {

  public:
    inline memaddr_node(int lineno, cdk::expression_node *memAddressPointer) : 
    cdk::unary_expression_node(lineno, memAddressPointer){

    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memaddr_node(this, level);
    }

  };

} // xpl

#endif
