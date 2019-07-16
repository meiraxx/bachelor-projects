// $Id: sweep_node.h,v 1.4 2017/04/21 08:11:59 ist182014 Exp $ -*- c++ -*-
#ifndef __CDK_SWEEPNODE_H__
#define __CDK_SWEEPNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing sweep-cycle nodes.
   */
  class sweep_node: public cdk::basic_node {
    bool _sweepPlusFlag;
    cdk::lvalue_node *_variable;
    cdk::expression_node *_start;
    cdk::expression_node *_end;
    cdk::expression_node *_increment;
    cdk::basic_node *_block;

  public:
    inline sweep_node(int lineno, bool sweepPlusFlag, cdk::lvalue_node *variable, cdk::expression_node *start, 
      cdk::expression_node *end, cdk::expression_node *increment, cdk::basic_node *block) : 
        cdk::basic_node(lineno), _sweepPlusFlag(sweepPlusFlag), _variable(variable), _start(start), _end(end), 
        _increment(increment), _block(block){

        }

  public:
    inline bool sweepPlusFlag() {
      return _sweepPlusFlag;
    }
    
    inline cdk::lvalue_node *variable() {
      return _variable;
    }

    inline cdk::expression_node *start() {
      return _start;
    }

    inline cdk::expression_node *end() {
      return _end;
    }

    inline cdk::expression_node *increment() {
      return _increment;
    }

    inline cdk::basic_node *block() {
      return _block;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_node(this, level);
    }
    
  };

} // xpl

#endif
