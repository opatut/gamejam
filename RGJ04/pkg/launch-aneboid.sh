#!/bin/bash
if [[ $(uname -m) == 'i686' ]]; then
    LD_LIBRARY_PATH='lib32/' ./ameboid32
elif [[ $(uname -m) == 'x86_64' ]]; then
    LD_LIBRARY_PATH='lib64/' ./ameboid64
fi
