#!/bin/bash

echo "enter name of file: "
read name
terminator -e "bash -c './child $name; read'"

sleep 8
