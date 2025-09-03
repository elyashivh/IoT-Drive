#!/bin/bash

rm -f minion_1.txt && dd if=/dev/zero of=minion_1.txt bs=1M count=8
rm -f minion_2.txt && dd if=/dev/zero of=minion_2.txt bs=1M count=8
rm -f minion_3.txt && dd if=/dev/zero of=minion_3.txt bs=1M count=8
