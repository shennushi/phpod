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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_pod.h"

#undef EX
#define EX(element) execute_data->element

#define CHECK_SYMBOL_TABLES()
#define ZEND_VM_SET_OPCODE(new_op) \
	CHECK_SYMBOL_TABLES(); \
	EX(opline) = new_op

static void (*old_execute)(zend_op_array *op_array TSRMLS_DC);
static void pod_execute(zend_op_array *op_array TSRMLS_DC);

static void pod_dump_op(znode *op);
static void pod_dump_zval(zval **var);

static const char *pod_opcodes[] = {
	"ZEND_NOP",
	"ZEND_ADD",
	"ZEND_SUB",
	"ZEND_MUL",
	"ZEND_DIV",
	"ZEND_MOD",
	"ZEND_SL",
	"ZEND_SR",
	"ZEND_CONCAT",
	"ZEND_BW_OR",
	"ZEND_BW_AND",
	"ZEND_BW_XOR",
	"ZEND_BW_NOT",
	"ZEND_BOOL_NOT",
	"ZEND_BOOL_XOR",
	"ZEND_IS_IDENTICAL",
	"ZEND_IS_NOT_IDENTICAL",
	"ZEND_IS_EQUAL",
	"ZEND_IS_NOT_EQUAL",
	"ZEND_IS_SMALLER",
	"ZEND_IS_SMALLER_OR_EQUAL",
	"ZEND_CAST",
	"ZEND_QM_ASSIGN",
	"ZEND_ASSIGN_ADD",
	"ZEND_ASSIGN_SUB",
	"ZEND_ASSIGN_MUL",
	"ZEND_ASSIGN_DIV",
	"ZEND_ASSIGN_MOD",
	"ZEND_ASSIGN_SL",
	"ZEND_ASSIGN_SR",
	"ZEND_ASSIGN_CONCAT",
	"ZEND_ASSIGN_BW_OR",
	"ZEND_ASSIGN_BW_AND",
	"ZEND_ASSIGN_BW_XOR",
	"ZEND_PRE_INC",
	"ZEND_PRE_DEC",
	"ZEND_POST_INC",
	"ZEND_POST_DEC",
	"ZEND_ASSIGN",
	"ZEND_ASSIGN_REF",
	"ZEND_ECHO",
	"ZEND_PRINT",
	"ZEND_JMP",
	"ZEND_JMPZ",
	"ZEND_JMPNZ",
	"ZEND_JMPZNZ",
	"ZEND_JMPZ_EX",
	"ZEND_JMPNZ_EX",
	"ZEND_CASE",
	"ZEND_SWITCH_FREE",
	"ZEND_BRK",
	"ZEND_CONT",
	"ZEND_BOOL",
	"ZEND_INIT_STRING",
	"ZEND_ADD_CHAR",
	"ZEND_ADD_STRING",
	"ZEND_ADD_VAR",
	"ZEND_BEGIN_SILENCE",
	"ZEND_END_SILENCE",
	"ZEND_INIT_FCALL_BY_NAME",
	"ZEND_DO_FCALL",
	"ZEND_DO_FCALL_BY_NAME",
	"ZEND_RETURN",
	"ZEND_RECV",
	"ZEND_RECV_INIT",
	"ZEND_SEND_VAL",
	"ZEND_SEND_VAR",
	"ZEND_SEND_REF",
	"ZEND_NEW",
	"ZEND_INIT_NS_FCALL_BY_NAME",
	"ZEND_FREE",
	"ZEND_INIT_ARRAY",
	"ZEND_ADD_ARRAY_ELEMENT",
	"ZEND_INCLUDE_OR_EVAL",
	"ZEND_UNSET_VAR",
	"ZEND_UNSET_DIM",
	"ZEND_UNSET_OBJ",
	"ZEND_FE_RESET",
	"ZEND_FE_FETCH",
	"ZEND_EXIT",
	"ZEND_FETCH_R",
	"ZEND_FETCH_DIM_R",
	"ZEND_FETCH_OBJ_R",
	"ZEND_FETCH_W",
	"ZEND_FETCH_DIM_W",
	"ZEND_FETCH_OBJ_W",
	"ZEND_FETCH_RW",
	"ZEND_FETCH_DIM_RW",
	"ZEND_FETCH_OBJ_RW",
	"ZEND_FETCH_IS",
	"ZEND_FETCH_DIM_IS",
	"ZEND_FETCH_OBJ_IS",
	"ZEND_FETCH_FUNC_ARG",
	"ZEND_FETCH_DIM_FUNC_ARG",
	"ZEND_FETCH_OBJ_FUNC_ARG",
	"ZEND_FETCH_UNSET",
	"ZEND_FETCH_DIM_UNSET",
	"ZEND_FETCH_OBJ_UNSET",
	"ZEND_FETCH_DIM_TMP_VAR",
	"ZEND_FETCH_CONSTANT",
	"ZEND_GOTO",
	"ZEND_EXT_STMT",
	"ZEND_EXT_FCALL_BEGIN",
	"ZEND_EXT_FCALL_END",
	"ZEND_EXT_NOP",
	"ZEND_TICKS",
	"ZEND_SEND_VAR_NO_REF",
	"ZEND_CATCH",
	"ZEND_THROW",
	"ZEND_FETCH_CLASS",
	"ZEND_CLONE",
	"ZEND_INIT_METHOD_CALL",
	"ZEND_INIT_STATIC_METHOD_CALL",
	"ZEND_ISSET_ISEMPTY_VAR",
	"ZEND_ISSET_ISEMPTY_DIM_OBJ",
	"ZEND_PRE_INC_OBJ",
	"ZEND_PRE_DEC_OBJ",
	"ZEND_POST_INC_OBJ",
	"ZEND_POST_DEC_OBJ",
	"ZEND_ASSIGN_OBJ",
	"ZEND_INSTANCEOF",
	"ZEND_DECLARE_CLASS",
	"ZEND_DECLARE_INHERITED_CLASS",
	"ZEND_DECLARE_FUNCTION",
	"ZEND_RAISE_ABSTRACT_ERROR",
	"ZEND_DECLARE_CONST",
	"ZEND_ADD_INTERFACE",
	"ZEND_DECLARE_INHERITED_CLASS_DELAYED",
	"ZEND_VERIFY_ABSTRACT_CLASS",
	"ZEND_ASSIGN_DIM",
	"ZEND_ISSET_ISEMPTY_PROP_OBJ",
	"ZEND_HANDLE_EXCEPTION",
	"ZEND_USER_OPCODE",
	"ZEND_JMP_SET",
	"ZEND_DECLARE_LAMBDA_FUNCTION"
};

/*
 * Returns the operand type
 */
static inline const char* pod_op_type_name(int op_type)
{
	switch (op_type) {
		case IS_CONST:   return "IS_CONST";
		case IS_TMP_VAR: return "IS_TMP_VAR";
		case IS_VAR:     return "IS_VAR";
		case IS_UNUSED:  return "IS_UNUSED";
		case IS_CV:      return "IS_CV";
	}
};

/*
 * Dumps an operand 
 */
static void pod_dump_op(znode *op)
{
	printf("- Op type: %s", pod_op_type_name(op->op_type));
	
	if (op->op_type == IS_CONST) {
		switch (Z_TYPE(op->u.constant)) {
			case IS_LONG:
			case IS_RESOURCE:
			case IS_NULL:
			case IS_BOOL:
				printf(" value: '%ld'", Z_LVAL(op->u.constant));
				break;
			case IS_STRING:
				printf(" value: '%s'", Z_STRVAL(op->u.constant));
				break;
			case IS_OBJECT:
				printf(" value: object");				
		}
	}
	printf("\n");
}

/*
 * Dumps the zval
 */
static void pod_dump_zval(zval **var)
{
	printf("(type: %s refcount: %d is_ref: %d)\n", zend_get_type_by_const(Z_TYPE_PP(var)), Z_REFCOUNT_PP(var), Z_ISREF_PP(var));
}

/*
 * Executor
 */
static void pod_execute(zend_op_array *op_array TSRMLS_DC)
{
	zend_execute_data *execute_data;
	zend_bool nested = 0;
	zend_bool original_in_execution = EG(in_execution);
	zend_uchar next_opcode = 0;

	if (EG(exception)) {
		return;
	}

	EG(in_execution) = 1;

zend_vm_enter:
	/* Initialize execute_data */
	execute_data = (zend_execute_data *)zend_vm_stack_alloc(
		ZEND_MM_ALIGNED_SIZE(sizeof(zend_execute_data)) +
		ZEND_MM_ALIGNED_SIZE(sizeof(zval**) * op_array->last_var * (EG(active_symbol_table) ? 1 : 2)) +
		ZEND_MM_ALIGNED_SIZE(sizeof(temp_variable)) * op_array->T TSRMLS_CC);

	EX(CVs) = (zval***)((char*)execute_data + ZEND_MM_ALIGNED_SIZE(sizeof(zend_execute_data)));
	memset(EX(CVs), 0, sizeof(zval**) * op_array->last_var);
	EX(Ts) = (temp_variable *)(((char*)EX(CVs)) + ZEND_MM_ALIGNED_SIZE(sizeof(zval**) * op_array->last_var * (EG(active_symbol_table) ? 1 : 2)));
	EX(fbc) = NULL;
	EX(called_scope) = NULL;
	EX(object) = NULL;
	EX(old_error_reporting) = NULL;
	EX(op_array) = op_array;
	EX(symbol_table) = EG(active_symbol_table);
	EX(prev_execute_data) = EG(current_execute_data);
	EG(current_execute_data) = execute_data;
	EX(nested) = nested;
	nested = 1;
	
	if (op_array->start_op) {
		ZEND_VM_SET_OPCODE(op_array->start_op);
	} else {
		ZEND_VM_SET_OPCODE(op_array->opcodes);
	}
	
	EG(opline_ptr) = &EX(opline);

	EX(function_state).function = (zend_function *) op_array;
	EX(function_state).arguments = NULL;
	
	next_opcode = EX(opline)->opcode;

	while (1) {
    	int ret;
#ifdef ZEND_WIN32
		if (EG(timed_out)) {
			zend_timeout(0);
		}
#endif
		printf("+ Opcode %s\n", pod_opcodes[EX(opline)->opcode]);
		pod_dump_op(&EX(opline)->op1);
		pod_dump_op(&EX(opline)->op2);
		printf("\n");
		
		if (next_opcode == ZEND_PRINT) {
			printf("-- Start output:\n");
		}
		
		if ((ret = EX(opline)->handler(execute_data TSRMLS_CC)) > 0) {
			switch (ret) {
				case 1:
					EG(in_execution) = original_in_execution;
					if (next_opcode == ZEND_RETURN) {
						printf("- EG(return_value_ptr_ptr) = %p ", EG(return_value_ptr_ptr));
						pod_dump_zval(EG(return_value_ptr_ptr));
					}
					return;
				case 2:
					op_array = EG(active_op_array);
					goto zend_vm_enter;
				case 3:
					execute_data = EG(current_execute_data);
				default:
					break;
			}
		}
		if (next_opcode == ZEND_PRINT) {
			printf("\n-- End output\n\n");
		}
		next_opcode = EX(opline)->opcode;
	}
	zend_error_noreturn(E_ERROR, "Arrived at end of main loop which shouldn't happen");
}

/* {{{ pod_functions[]
 */
const zend_function_entry pod_functions[] = {	
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ pod_module_entry
 */
zend_module_entry pod_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"POD",
	pod_functions,
	PHP_MINIT(pod),
	PHP_MSHUTDOWN(pod),
	PHP_RINIT(pod),
	PHP_RSHUTDOWN(pod),
	PHP_MINFO(pod),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_POD
ZEND_GET_MODULE(pod)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pod)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pod)
{
	zend_execute = old_execute;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pod)
{
	old_execute = zend_execute;
	
	zend_execute = pod_execute;
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pod)
{
	zend_execute = old_execute;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pod)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "POD support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */