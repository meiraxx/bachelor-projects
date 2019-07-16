// $Id: block_node.h,v 1.3 2017/04/21 05:09:09 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_BLOCKNODE_H__
#define __XPL_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing block nodes.
   */

  class block_node: public cdk::basic_node {
    cdk::sequence_node *_declarations;
    cdk::sequence_node *_instructions;

  public:
    inline block_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instructions) :
        cdk::basic_node(lineno), _declarations(declarations), _instructions(instructions) {
    }

  public:
    inline cdk::sequence_node *declarations() {
      return _declarations;
    }

    inline cdk::sequence_node *instructions() {
      return _instructions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // xpl

#endif
