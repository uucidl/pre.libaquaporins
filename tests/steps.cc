#include <boost/test/unit_test.hpp>
#include <cucumber-cpp/defs.hpp>

extern "C" {
#include "../src/alloc.h"
#include "aqp.h"
}

static int seen_pointers_capacity = 256;
static struct PtrPair {
	void* ptr;
	int size;
}* seen_pointers = (struct PtrPair*) malloc(seen_pointers_capacity * sizeof (*seen_pointers));
static int seen_pointers_n = 0;

static int total_allocated = 0;
static int memtrace = 0;

static void watch_ptr(void* ptr, int size)
{
	if (seen_pointers_n + 1 > seen_pointers_capacity) {
		seen_pointers_capacity *= 2;
		seen_pointers = (struct PtrPair*) realloc (seen_pointers, seen_pointers_n * sizeof *seen_pointers);
	}

	seen_pointers[seen_pointers_n].ptr  = ptr;
	seen_pointers[seen_pointers_n].size = size;
	seen_pointers_n++;
}

extern "C" {
extern void* aqp_malloc(int size)
{
	void* ptr = malloc (size);

	if (memtrace) {
		watch_ptr (ptr, size);
		total_allocated += size;
	}

	return ptr;
}

extern void* aqp_calloc(int count, int size)
{
	void* ptr = calloc (count, size);

	if (memtrace) {
		watch_ptr (ptr, count*size);
		total_allocated += count*size;
	}

	return ptr;
}

extern void* aqp_realloc(void* ptr, int size)
{
	int ptr_size = 0;

	if (memtrace) {
		for (int i = 0; i < seen_pointers_n; i++) {
			if (seen_pointers[i].ptr == ptr) {
				ptr_size = seen_pointers[i].size;
				seen_pointers[i].size = size;
				break;
			}
		}

		total_allocated += size - ptr_size;
	}

	return realloc (ptr, size);
}

extern void aqp_free(void* ptr)
{
	int ptr_size = 0;

	if (memtrace) {
		for (int i = 0; i < seen_pointers_n; i++) {
			if (seen_pointers[i].ptr == ptr) {
				ptr_size = seen_pointers[i].size;
				seen_pointers[i].ptr = NULL;
				seen_pointers[i].size = 0;
				break;
			}
		}

		total_allocated -= ptr_size;
	}
	free (ptr);
}
}

static std::vector<aqp_piece_t> pieces;

GIVEN("^I have created (a|another) piece$") {
	pieces.push_back(aqp_new_piece());
}

THEN("^the first and second piece should be different$") {
	BOOST_CHECK(pieces[0] != pieces[1]);
}


AFTER() {
	/* rollback memory tracing */
	memtrace = 0;
	seen_pointers_n = 0;
	total_allocated = 0;
}

GIVEN("^I want to monitor memory$") {
	memtrace = 1;
}

THEN("^memory should have been allocated$") {
	BOOST_CHECK(total_allocated > 0);
}

THEN("^I delete the piece$") {
	aqp_delete_piece (pieces.back());
	pieces.pop_back();
}


THEN("^memory should not be allocated$") {
	BOOST_CHECK(total_allocated == 0);
}
