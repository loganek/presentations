#!/bin/bash

# HELLO WORLD
./bin/hello-world
hexedit hello-world.htdump # view raw data
hawktracer-converter-rs -s hello-world.htdump --stdout --format debug # debug view of all the events

# TIME MEASUREMENT
./bin/time-measurement # run time-measurement program
hawktracer-converter-rs --source time-measurement.htdump -o time-measurement.json # convert to chrome-trace
hawktracer-converter-rs -s time-measurement.htdump --format debug --stdout # stdout
hawktracer-converter-rs -s time-measurement.htdump --format debug --stdout | grep "fibonacci 1" -B 4 # see
hawktracer-converter-rs --source time-measurement.htdump -o time-measurement.html --format flamegraph # convert to flamegraph

HT_PYTHON_TRACE_ENABLED=1 ./bin/time-measurement.py

# RESOURCE USAGE
./bin/resource-usage --ht-global-timeline-buffer-size 0
./bin/client.py


# ROTATE
HT_PYTHON_TRACE_ENABLED=1 LD_LIBRARY_PATH=lib ./bin/rotate-rs
hawktracer-converter-rs -s rotate.htdump -o rotate.json # convert to chrome-trace
aws s3 ls s3://mkolny-fosdem-2020/backup.jpg # ls backup file (rotate example)