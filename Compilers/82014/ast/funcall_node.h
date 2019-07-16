// $Id: funcall_node.h,v 1.4 2017/04/19 21:24:08 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_FUNCALLNODE_H__
#define __XPL_FUNCALLNODE_H__

#include <string>
#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing function call nodes.
   */

  class funcall_node: public cdk::expression_node {
    std::string *_funName;
    cdk::sequence_node *_arguments;

  public:
    inline funcall_node(int lineno, std::string *funName, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _funName(funName), _arguments(arguments) {
    }

  public:

    inline std::string *funName() {
      return _funName;
    }

    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_funcall_node(this, level);
    }

  };

} // xpl

#endif
