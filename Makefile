##
# Build library

all: libaquaporins.a
clean:
	rm -f libaquaporins.a $(lib_OBJS)

SRC-CORE=$(wildcard src/*.c)
SRC-LIBC=$(wildcard src/arch/libc-*.c)

lib_SRC=$(SRC-CORE) $(SRC-LIBC)
lib_OBJS=$(lib_SRC:.c=.o)

INCLUDE_DIR+=include/

$(lib_OBJS): CFLAGS+=$(addprefix -I,$(INCLUDE_DIR))

libaquaporins.a: $(lib_OBJS)
	$(AR) -r $@ $^

cucumber-cpp_ROOT=../third-party/cucumber-cpp

tests: tests/cuketest

cuketest_SRC=tests/steps.cc
cuketest_OBJS=$(cuketest_SRC:.cc=.o)
cuketest_INCLUDE_DIR=$(cucumber-cpp_ROOT)/include
cuketest_LIBS=boost_regex-mt boost_system-mt boost_unit_test_framework-mt cucumber-cpp
cuketest_LIB_DIR=$(cucumber-cpp_ROOT)/build/src

$(cuketest_OBJS): CXXFLAGS+=$(addprefix -I,$(INCLUDE_DIR) $(cuketest_INCLUDE_DIR))
tests/cuketest: $(cuketest_OBJS) $(SRC-CORE:.c=.o)
	$(CXX) -o $@ $(addprefix -L,$(cuketest_LIB_DIR)) $(addprefix -l,$(cuketest_LIBS)) $^
