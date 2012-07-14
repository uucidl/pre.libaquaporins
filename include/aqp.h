#ifndef aqp_h
#define aqp_h

/**
 * A Piece contains sections, their headers, their content.
 */
struct aqp_Piece;

/**
 * Allocate a new piece.
 */
extern struct aqp_Piece* aqp_piece_alloc();

/**
 * Free all resources related to a piece.
 */
extern void aqp_piece_free(struct aqp_Piece* piece);

#endif
