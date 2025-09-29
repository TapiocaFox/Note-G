#!/bin/bash
rm -rf ./docs
mkdir ./docs
cp -r ./src/web/* ./docs/
touch ./docs/.nojekyll