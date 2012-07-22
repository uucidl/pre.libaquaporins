#ifndef growa_h
#define growa_h

#define inita_m(array_var, elementn_var, arrayn_var) do { \
		array_var = 0;				  \
		elementn_var = arrayn_var = 0;		  \
	} while(0)

/**! resize array until we reach n elements */
#define growa_m(n_expr, array_var, elementn_var, arrayn_var) do {	\
		int const n = n_expr;					\
		if (arrayn_var < n) {					\
			int new_arrayn = arrayn_var;			\
			if (new_arrayn <= 0) {				\
				new_arrayn = 1;				\
			}						\
			while (new_arrayn < n) {			\
				new_arrayn *= 2;			\
			}						\
			array_var = aqp_realloc (array_var, new_arrayn * sizeof (*array_var)); \
			memset(array_var + arrayn_var, 0, new_arrayn - arrayn_var); \
			arrayn_var = new_arrayn;			\
		}							\
		elementn_var = n;					\
	} while (0)

#define freea_m(array_var, elementn_var, arrayn_var) do {	\
		aqp_free (array_var);				\
		array_var = 0;					\
		arrayn_var = elementn_var = 0;			\
	} while(0)						\

/**! increment array by one */
#define inca_m(array_var, elementn_var, arrayn_var) \
	growa_m(elementn_var + 1, array_var, elementn_var, arrayn_var)

#endif
