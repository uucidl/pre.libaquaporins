include ysr.mk

aqp_OBJS=$(DEST)/src/aqp.o
aqp_OBJS+=$(DEST)/src/alloc-libc.o

$(call mk-c-staticlib-rule,aqp)

libaqp_LIBS=$(aqp_LIBNAME)
libaqp_LIBSPATH=$(aqp_LIBDIR)
libaqp_DEPS=aqp
libaqp=1

cukebins_ROOT=$(TOP)/../third-party/cukebins
cukebins_OBJS=$(DEST)/tests/steps.o
cukebins_INCLUDES=$(cukebins_ROOT)/include
cukebins_LIBSPATH=$(cukebins_ROOT)/build/src /usr/local/lib
cukebins_LIBS=Cukebins boost_system-mt boost_regex-mt boost_unit_test_framework-mt
cukebins_REQUIRES=libaqp
$(call mk-c++-console-prog-rule,cukebins)

tests-run: cukebins-run
	(cd $(TOP)/tests && cucumber)

.PHONY: tests-run
