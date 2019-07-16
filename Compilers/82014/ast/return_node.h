// $Id: return_node.h,v 1.2 2017/03/24 16:32:59 ist182014 Exp $ -*- c++ -*-
#ifndef __CDK_RETURNNODE_H__
#define __CDK_RETURNNODE_H__

namespace xpl {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {

  public:
    inline return_node(int lineno) : cdk::basic_node(lineno){

        }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }
    
  };

} // xpl

#endif
