# !/bin/bash

git fetch upstream
git merge upstream/master
git add -A
git push origin master
