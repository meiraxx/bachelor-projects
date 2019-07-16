// $Id: fundef_node.h,v 1.7 2017/05/09 18:44:44 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_FUNDEFNODE_H__
#define __XPL_FUNDEFNODE_H__

namespace xpl {

  /**
   * Class for describing fundef nodes.
   */

  class fundef_node: public cdk::basic_node {
    basic_type *_type;
    std::string *_funName;
    bool _publicFlag;
    cdk::sequence_node *_arguments;
    cdk::expression_node *_returnValue;
    xpl::body_node *_body;

  public:
    inline fundef_node(int lineno, basic_type *type, std::string *funName, bool publicFlag,
      cdk::sequence_node *arguments, cdk::expression_node *returnValue, xpl::body_node *body) :
        cdk::basic_node(lineno), _type(type), _funName(funName), 
        _publicFlag(publicFlag), _arguments(arguments), _returnValue(returnValue), _body(body) {
    }

  public:
    inline basic_type *type() {
      return _type;
    }

    inline basic_type *type(basic_type *type) {
      return _type = type;
    }

    inline std::string *funName() {
      return _funName;
    }

    inline bool publicFlag() {
      return _publicFlag;
    }

    inline void setPublicFlag() {
      _publicFlag = true;
    }
    
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    inline cdk::expression_node *returnValue() {
      return _returnValue;
    }

    inline xpl::body_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fundef_node(this, level);
    }

  };

} // xpl

#endif
