#!/bin/sh

# push to epitech repository
git push origin master
if [ $? -eq 0 ]
then
  echo '√ Pushed to epitech repository'
elif
  echo '✗ Failed to push to epitech repository'
fi

# push to mirror repository (https://github.com/Liroo/Nanotekspice)
git push --mirror git@github.com:Liroo/Nanotekspice.gi
if [ $? -eq 0 ]
then
  echo '√ Pushed to mirror repository'
elif
  echo '✗ Failed to push to mirror repository'
fi
