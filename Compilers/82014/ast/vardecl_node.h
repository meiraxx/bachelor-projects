// $Id: vardecl_node.h,v 1.6 2017/05/08 18:34:06 ist182014 Exp $ -*- c++ -*-
#ifndef __XPL_VARDECLNODE_H__
#define __XPL_VARDECLNODE_H__

#include <string>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace xpl {

  /**
   * Class for describing variable declaration nodes.
   */

  class vardecl_node: public cdk::basic_node {
    basic_type *_type;
    std::string *_varname;
    bool _useFlag;
    bool _publicFlag;
    cdk::expression_node *_varvalue;
    //bool _inFunc;

  public:
    inline vardecl_node(int lineno, basic_type *type, std::string *varname, bool useFlag, 
      bool publicFlag, cdk::expression_node *varvalue/*, bool inFunc = false*/) :
        cdk::basic_node(lineno), _type(type), _varname(varname), _useFlag(useFlag), 
        _publicFlag(publicFlag), _varvalue(varvalue)/*, _inFunc(inFunc)*/ {
    }

  public:
    inline basic_type *type() {
      return _type;
    }

    inline basic_type *type(basic_type *type) {
      return _type = type;
    }

    inline std::string *varname() {
      return _varname;
    }

    inline bool useFlag() {
      return _useFlag;
    }

    inline bool publicFlag() {
      return _publicFlag;
    }

    inline void setUseFlag() {
      _useFlag = true;
    }

    inline void setPublicFlag() {
      _publicFlag = true;
    }

    inline cdk::expression_node *varvalue() {
      return _varvalue;
    }
/*
    inline bool inFunc() {
      return _inFunc;
    }*/

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_vardecl_node(this, level);
    }

  };

} // xpl

#endif
