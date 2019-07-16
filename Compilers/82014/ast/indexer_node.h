// $Id: indexer_node.h,v 1.3 2017/04/21 05:09:09 ist182014 Exp $ -*- c++ -*-
#ifndef __CDK_INDEXERNODE_H__
#define __CDK_INDEXERNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing indexer nodes.
   */
  class indexer_node: public cdk::lvalue_node {
    cdk::expression_node *_firstElement;
    cdk::expression_node *_secondElement;

  public:
    inline indexer_node(int lineno, cdk::expression_node *firstElement, cdk::expression_node *secondElement) : 
        cdk::lvalue_node(lineno), _firstElement(firstElement), _secondElement(secondElement){

        }

  public:
    inline cdk::expression_node *firstElement() {
      return _firstElement;
    }

    inline cdk::expression_node *secondElement() {
      return _secondElement;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_indexer_node(this, level);
    }
    
  };

} // xpl

#endif
