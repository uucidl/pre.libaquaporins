#!/bin/sh
HERE=`dirname $0`
export GIT_DIR=${HERE}/../.git
ENTRIES=$(mktemp $(basename $0).XX)
trap 'rm -f $ENTRIES' EXIT

sh ${HERE}/entry.sh > $ENTRIES
cat "$GIT_DIR"/../tests/*.cc | grep -o -f $ENTRIES | grep -v -f - $ENTRIES
