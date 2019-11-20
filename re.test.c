#include "tctest.h"
#include "re.h"


void test_re_compile(void) {
	size_t i;
	node_t *n;
	struct re_compile_tests {
		char *re;
		int valid;
	} re_tests[] = {
		{ "^", 1 },
		{ "$", 1 },
		{ "^$", 1 }
	};

	for (i = 0; i < (sizeof(re_tests)/sizeof(re_tests[0])); i++) {
		n = re_compile(re_tests[i].re);
		if (re_tests[i].valid) {
			TCTEST_CHECK(n != NULL, "valid regex not recognized");
		} else {
			TCTEST_CHECK(n == NULL, "invalid regex recognized");
		}
		if (n != NULL) {
			re_free(n);
		}
	}
}

