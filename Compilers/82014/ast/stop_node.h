// $Id: stop_node.h,v 1.2 2017/03/24 16:32:59 ist182014 Exp $ -*- c++ -*-
#ifndef __CDK_STOPNODE_H__
#define __CDK_STOPNODE_H__

namespace xpl {

  /**
   * Class for describing stop nodes.
   */
  class stop_node: public cdk::basic_node {

  public:
    inline stop_node(int lineno) : cdk::basic_node(lineno){

        }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }
    
  };

} // xpl

#endif
