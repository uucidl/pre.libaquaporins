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
			if (arrayn_var <= 0) {				\
				arrayn_var = 1;				\
			}						\
			while (arrayn_var < n) {			\
				arrayn_var *= 2;			\
			}						\
			array_var = aqp_realloc (array_var, arrayn_var * sizeof (*array_var)); \
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
