#!/bin/bash

echo "Placing startCode binary in /usr/bin ..."
cp ./startCode /usr/bin/
echo "Creating default resources, sample templates etc. in ~/.startCode ..."
mkdir ~/.startCode
cp -rf ./templates ~/.startCode
echo "startCode installed! Refer to README for details"
