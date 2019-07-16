// $Id: memalloc_node.h,v 1.2 2017/03/24 16:32:59 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_MEMALLOCNODE_H__
#define __XPL_MEMALLOCNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_expression_node.h>
namespace xpl {

  /**
   * Class for describing memory alloccation nodes.
   */
  class memalloc_node: public cdk::unary_expression_node {

  public:
    inline memalloc_node(int lineno, cdk::expression_node *memAllocSize) : 
    cdk::unary_expression_node(lineno, memAllocSize){

    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memalloc_node(this, level);
    }

  };

} // xpl

#endif
