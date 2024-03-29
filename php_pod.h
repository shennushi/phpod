/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Felipe Pena <felipe@php.net>                                 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_POD_H
#define PHP_POD_H

extern zend_module_entry pod_module_entry;

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(pod)
	char *dump_path;
	FILE *dump_file;
ZEND_END_MODULE_GLOBALS(pod)

#ifdef ZTS
#define POD_G(v) TSRMG(pod_globals_id, zend_pod_globals *, v)
#else
#define POD_G(v) (pod_globals.v)
#endif

#if ZEND_MODULE_API_NO >= 20100409
#define ZEND_ENGINE_2_4
#endif

#define CHECK_SYMBOL_TABLES()
#define ZEND_VM_SET_OPCODE(new_op) \
	CHECK_SYMBOL_TABLES(); \
	EX(opline) = new_op

#define PX(element) EG(current_execute_data)->element
#define T(offset) (*(temp_variable *)((char *) PX(Ts) + offset))
#define CV_OF(i)     (EG(current_execute_data)->CVs[i])
#define CV_DEF_OF(i) (EG(active_op_array)->vars[i])

#ifdef ZEND_ENGINE_2_4
# define ZNODE znode_op
# define OP_VAR(op) op->var
# define OP1_TYPE(op) op->op1_type
# define OP2_TYPE(op) op->op2_type
# define RES_TYPE(op) op->result_type
# define ZVAL_OP_CONST(op) op->zv
# define RETURN_VALUE_USED(opline) (!((opline)->result_type & EXT_TYPE_UNUSED))
#else
# define ZNODE znode
# define OP_VAR(op) op->u.var
# define OP1_TYPE(op) op->op1.op_type
# define OP2_TYPE(op) op->op2.op_type
# define RES_TYPE(op) op->result.op_type
# define ZVAL_OP_CONST(op) &op->u.constant
# define RETURN_VALUE_USED(opline) (!((opline)->result.u.EA.type & EXT_TYPE_UNUSED))
#endif

PHP_MINIT_FUNCTION(pod);
PHP_MSHUTDOWN_FUNCTION(pod);
PHP_RINIT_FUNCTION(pod);
PHP_RSHUTDOWN_FUNCTION(pod);
PHP_MINFO_FUNCTION(pod);

#endif	/* PHP_POD_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
