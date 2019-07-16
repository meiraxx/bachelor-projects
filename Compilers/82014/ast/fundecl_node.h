// $Id: fundecl_node.h,v 1.7 2017/05/12 01:30:02 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_FUNDECLNODE_H__
#define __XPL_FUNDECLNODE_H__

#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/basic_type.h>

namespace xpl {

  /**
   * Class for describing fundecl nodes.
   */

  class fundecl_node: public cdk::basic_node {
    basic_type *_type;
    std::string *_funName;
    bool _useFlag;
    bool _publicFlag;
    cdk::sequence_node *_arguments;

  public:
    inline fundecl_node(int lineno, basic_type *type, std::string *funName, bool useFlag, bool publicFlag, 
      cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _funName(funName), _useFlag(useFlag), _publicFlag(publicFlag),
        _arguments(arguments) {
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

    inline bool useFlag() {
      return _useFlag;
    }

    inline void setUseFlag() {
      _useFlag = true;
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

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fundecl_node(this, level);
    }

  };

} // xpl

#endif
