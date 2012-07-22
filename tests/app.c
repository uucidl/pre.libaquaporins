#include "aqp.h"

#define ARRAY_N(array) ((int)(sizeof(array) / sizeof(*array)))

enum TrackType
	{
		int16, int8, note, end
	};

void aqp_new_track (aqp_segment_t segment, int const n, enum TrackType const types[n])
{

}

int main ()
{
	aqp_piece_t piece = aqp_new_piece();

	aqp_segment_t segment = aqp_new_segment(piece);

	char* signature = "(int16 int16) (note int8 int8) (note int8 int8)";

	enum TrackType types0[] = { int16, int16, end };
	enum TrackType types1[] = { note, int8, int8 };

	aqp_new_track (segment, ARRAY_N(types0), types0);
	aqp_new_track (segment, ARRAY_N(types1), types1);

	aqp_delete_piece (piece);
}
