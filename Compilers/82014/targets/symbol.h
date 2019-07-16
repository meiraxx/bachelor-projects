#ifndef __XPL_SEMANTICS_SYMBOL_H__
#define __XPL_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace xpl {

    class symbol {
      basic_type *_type;
      std::string _name;
      
      bool _isDeclared = false;
      bool _isDefined = false;
      bool _isVar = true;
      std::vector<basic_type*> _arguments;
      int _offset = 0;
      bool _isGlobal = true;
      //std::string _funName;
      

    public:
      inline symbol(basic_type *type, const std::string &name) : 
              _type(type), _name(name) {

      }

      virtual ~symbol() {
        //delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }

      inline bool isVar() {
        return _isVar;
      }

      inline bool isVar(bool isVar) {
        return _isVar = isVar;
      }

      inline bool isGlobal() {
        return _isGlobal;
      }

      inline bool isGlobal(bool isGlobal) {
        return _isGlobal = isGlobal;
      }

      inline bool isDeclared() {
        return _isDeclared;
      }

      inline bool isDeclared(bool isDeclared) {
        return _isDeclared = isDeclared;
      }

      inline bool isDefined() {
        return _isDefined;
      }

      inline bool isDefined(bool isDefined) {
        return _isDefined = isDefined;
      }

      inline void arguments(basic_type* argument) {
        _arguments.push_back(argument);  
      }
      
      inline std::vector<basic_type*> arguments() const {
        return _arguments; 
      }

      inline int offset() {
        return _offset;
      }

      inline int offset(int offset) {
        return _offset = offset;
      }
/*
      inline const std::string &funName() const {
        return _funName;
      }

      inline std::string &funName(std::string &funName) {
        return _funName = funName;
      }
*/
    };

} // xpl

#endif
