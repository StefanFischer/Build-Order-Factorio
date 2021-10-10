#!/bin/bash
mkdir submit-me
cp -r build.sh generator.sh CMakeLists.txt lib/ src/ jsonFiles/ submit-me
zip -r  submit-me.zip submit-me/
rm -r submit-me/
