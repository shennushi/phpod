#include "php.h"
#include "php_pod.h"
#include "php_pod_execute.h"

zval **pod_get_zval_cv_lookup(zval ***ptr, zend_uint var, int type, char **var_name TSRMLS_DC)
{
	zend_compiled_variable *cv = &CV_DEF_OF(var);

	if (!EG(active_symbol_table) ||
	    zend_hash_quick_find(EG(active_symbol_table), cv->name, cv->name_len+1, cv->hash_value, (void **)ptr)==FAILURE) {
		switch (type) {
			case BP_VAR_R:
			case BP_VAR_UNSET:
				zend_error(E_NOTICE, "Undefined variable: %s", cv->name);
				/* break missing intentionally */
			case BP_VAR_IS:
				return &EG(uninitialized_zval_ptr);
				break;
			case BP_VAR_RW:
				zend_error(E_NOTICE, "Undefined variable: %s", cv->name);
				/* break missing intentionally */
			case BP_VAR_W:
				Z_ADDREF(EG(uninitialized_zval));
				if (!EG(active_symbol_table)) {
					*ptr = (zval**)EG(current_execute_data)->CVs + (EG(active_op_array)->last_var + var);
					**ptr = &EG(uninitialized_zval);
				} else {
					zend_hash_quick_update(EG(active_symbol_table), cv->name, cv->name_len+1, cv->hash_value, &EG(uninitialized_zval_ptr), sizeof(zval *), (void **)ptr);
				}
				break;
		}
		if (EG(active_symbol_table)) {
			*var_name = cv->name;
		}
	}
	return *ptr;
}
