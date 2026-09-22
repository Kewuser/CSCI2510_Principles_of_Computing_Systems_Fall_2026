#!/bin/bash
LOGFILE="$1"
echo "LINES: $(wc -l < "$LOGFILE")"
echo "STATUS_COUNTS:"
cut -d' ' -f2 "$LOGFILE" | sort | uniq -c |sort -rn |awk '{print $2, $1}'

