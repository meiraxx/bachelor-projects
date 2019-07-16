// $Id: body_node.h,v 1.5 2017/04/21 05:09:09 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_BODYNODE_H__
#define __XPL_BODYNODE_H__

#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing body nodes.
   */

  class body_node: public cdk::basic_node {
    xpl::block_node *_blockNode;

  public:
    inline body_node(int lineno, xpl::block_node *blockNode) :
        cdk::basic_node(lineno), _blockNode(blockNode) {
    }

  public:

    inline xpl::block_node *blockNode() {
      return _blockNode;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_body_node(this, level);
    }

  };

} // xpl

#endif
