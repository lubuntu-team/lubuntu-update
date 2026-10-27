#!/bin/bash
langList=('en_US' 'es_ES' 'zh_CN' 'eu' 'gl' 'fr_FR')

for i in ${langList[@]}; do
    targetFile="src/translations/lubuntu-update_$i.ts"
    if [ ! -e $targetFile ]; then
        echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>" > $targetFile
	echo "<!DOCTYPE TS>" >> $targetFile
        echo "<TS version=\"2.1\" language=\"$i\"></TS>" >> $targetFile
    fi
done
cd src
lupdate6 *.cpp *.h *.ui -ts translations/*
