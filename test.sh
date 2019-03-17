#!/bin/bash

# vous pouvez creer une hierarchie de fichier
# 	soit dans votre depots
#	soit dans en script (et suppression en fin de test)

rm -rf ../tmp
mkdir ../tmp

truthArray=(
    "find"
    "find ."
    "find ../"
    "find . -name tmp.txt"
    "find ./ -print"
    "grep -r -l . -e txt"
    # "find DOSSIER -type f -exec file {} \; | grep image | cut -d : -f 1"
    # "find DOSSIER -exec ls -l -d {} \;""
    # "find DOSSIER -exec COMMANDE \;""
    # "find . -exec ls -l \;"
    # "find DOSSIER -name CHAINE -type f -exec file {} \; | grep image | cut -d : -f 1"
    # "find DOSSIER -print -exec COMMANDE \;""
    # "find DOSSIER -name REGEXP"
    # "grep -r -l DOSSIER -e REGEXP"
)

testArray=(
    "./rsfind"
    "./rsfind ."
    "./rsfind ../"
    "./rsfind . --name tmp.txt"
    "./rsfind ./ --print"
    "rsfind . -t txt"
    # "./rsfind DOSSIER -i"
    # "rsfind DOSSIER -l"
    #'rsfind . --exec "ls -l"' 
    # "rsfind DOSSIER --name CHAINE -i"
    # "rsfind DOSSIER --print --exec "COMMANDE""
    # "rsfind DOSSIER --ename REGEXP"
    # "rsfind DOSSIER -T REGEXP"
)

test_error=false

for ((i = 0; i < ${#truthArray[@]}; i++))
do
    echo "Test: ${testArray[$i]}"
    eval ${truthArray[$i]} > ../tmp/truthTest.txt
    eval ${testArray[$i]} > ../tmp/resTest.txt
    diff ../tmp/truthTest.txt  ../tmp/resTest.txt
    e_code=$?
    
    if [ $e_code != 0 ]
    then
        printf "TEST FAIL : %d\n" "$e_code"
        test_error=true
    else
        printf "TEST OK!\n"
    fi
done

rm -rf ../tmp

if [ $test_error = true ] 
then
    exit 2
else
    exit 0
fi
