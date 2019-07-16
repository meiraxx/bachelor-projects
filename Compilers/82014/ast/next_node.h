// $Id: next_node.h,v 1.1 2017/03/23 23:53:27 ist182014 Exp $ -*- c++ -*-
#ifndef __CDK_NEXTNODE_H__
#define __CDK_NEXTNODE_H__

namespace xpl {

  /**
   * Class for describing next nodes.
   */
  class next_node: public cdk::basic_node {

  public:
    inline next_node(int lineno) : cdk::basic_node(lineno){

        }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }
    
  };

} // xpl

#endif
