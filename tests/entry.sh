#!/bin/sh
grep -e '^.*aqp_.*(.*)' "${GIT_DIR}"/../include/aqp.h | cut -d ' ' -f 2
