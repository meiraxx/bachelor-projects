// $Id: print_node.h,v 1.2 2017/03/23 23:53:27 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_PRINTNODE_H__
#define __XPL_PRINTNODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _newlineFlag;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool newlineFlag) :
        cdk::basic_node(lineno), _argument(argument), _newlineFlag(newlineFlag) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool newlineFlag() {
      return _newlineFlag;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // xpl

#endif
