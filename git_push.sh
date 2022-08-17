#!/bin/bash

git config --list
git add * && git commit -m $0 && git push origin main
