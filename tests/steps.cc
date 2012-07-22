#include <boost/test/unit_test.hpp>
#include <cucumber-cpp/defs.hpp>

#include <string.h>

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
	int slot_i = -1;

	if (memtrace) {
		for (int i = 0; i < seen_pointers_n; i++) {
			if (seen_pointers[i].ptr == ptr) {
				slot_i = i;
				ptr_size = seen_pointers[i].size;
				seen_pointers[i].size = size;
				break;
			}
		}

		total_allocated += size - ptr_size;
	}

	void* new_ptr = realloc (ptr, size);
	if (memtrace && !ptr) {
		watch_ptr (new_ptr, size);
	}

	if (slot_i >= 0) {
		seen_pointers[slot_i].ptr = new_ptr;
	}

	return new_ptr;
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

static void report_leaks()
{
	if (memtrace && total_allocated > 0) {
		printf ("Still %d bytes allocated with pointers:\n", total_allocated);
		for (int i = 0; i < seen_pointers_n; i++) {
			if (seen_pointers[i].ptr) {
				printf ("pointer: %p, size: %d\n", seen_pointers[i].ptr, seen_pointers[i].size);
			}
		}
	}
}

}


static std::vector<aqp_piece_t> pieces;
static std::vector<aqp_segment_t> segments;

GIVEN("^I have created a piece$") {
	if (pieces.empty()) {
		pieces.push_back(aqp_new_piece());
	}
}

GIVEN("^I have created another piece$") {
	pieces.push_back(aqp_new_piece());
}

THEN("^the first and second piece should be different$") {
	BOOST_CHECK(pieces[0] != pieces[1]);
}

AROUND_STEP("@test") {
	try {
		step->call();
	} catch (std::exception e) {
		std::cerr << "hello?" << std::endl;
		BOOST_CHECK_NO_THROW(throw);
		throw;
	} catch (...) {
		std::cerr << "world?" << std::endl;
		BOOST_CHECK_NO_THROW(throw);
		throw;
	}
}

AFTER() {
	/* rollback memory tracing */
	memtrace = 0;
	seen_pointers_n = 0;
	total_allocated = 0;

	segments.clear();

	aqp_piece_t piece;
	for (std::vector<aqp_piece_t>::iterator it = pieces.begin();
	     it != pieces.end();
	     ++it) {
		aqp_delete_piece (*it);
	}

	pieces.clear();
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
	report_leaks();
}

GIVEN("^I have created a segment$") {
	segments.push_back(aqp_new_segment(pieces.back()));
}

GIVEN("^I have created a track '(.+)' with signature '(.+)'$") {
	REGEX_PARAM(std::string, track_name);
	REGEX_PARAM(std::string, track_signature);
}

static aqp_segment_edition_range_t edit_range;
static int edit_range_row;

GIVEN("^I created a write range \\[(\\d+), *(\\d+)\\[ on track '(.+)'$") {
	REGEX_PARAM(int, start);
	REGEX_PARAM(int, end);
	edit_range = aqp_edit_range (segments.back(), start, end);
	edit_range_row = 0;
}

THEN("^I commit the write range$") {
	aqp_commit_range (edit_range);
}

THEN("^I write (\\d+) to the range$") {
	REGEX_PARAM(int, data);

	int field = 0;
	aqp_field_write_int(edit_range, edit_range_row, field, data);
}

THEN("^I advance to the next row$") {
	edit_range_row++;
}

static aqp_segment_range_t read_range;
static int read_range_row;

GIVEN("^I created a read range on track '(.+)'$") {
	int start = 0;
	int end = INT_MAX;

	read_range = aqp_read_range (segments.back(), start, end);
	read_range_row = 0;
}

THEN("^The sum of the range should be (\\d+)") {
	REGEX_PARAM(int, checksum);

	int sum = 0;
	while (read_range_row < 16) {
		int data = aqp_field_read_int(read_range, read_range_row, 0);
		sum += data;
		read_range_row++;
	}

	BOOST_CHECK_EQUAL(checksum, sum);
}
