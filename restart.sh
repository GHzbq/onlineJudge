#!/bin/bash
pkill -9 oj_server 

nohup ./oj_server >> logs/oj_server.log 2>&1 &
