#!/bin/bash

grep -P '[^\x00]' minion_1.txt && echo "NOT all nulls" || echo "ALL nulls"
grep -P '[^\x00]' minion_2.txt && echo "NOT all nulls" || echo "ALL nulls"
grep -P '[^\x00]' minion_3.txt && echo "NOT all nulls" || echo "ALL nulls"
