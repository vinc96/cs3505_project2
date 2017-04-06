#!/bin/bash
# please place it under repository root directory

echo Automatic git add ., commit and push

read -p "commit message: " msg
git add .
git commit -m "$msg"
git push

echo
echo Finished. Press Enter to Exit...

read


# github command list:
# remember password for 4 hours: 
# 	   git config --global credential.helper "cache --timeout=14400"
# entirely replace local repo with remote: 
# 	   git reset --hard origin/master
# refresh .gitignore cache
#	   git rm -r --cached .
# set default branch 
#      git push -u origin branch name