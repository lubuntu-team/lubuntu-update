#!/bin/bash
langList=('en_US' 'es_ES' 'zh_CN')
for i in ${langList[@]}; do
    targetFile="translations/lubuntu-update_$i.ts"
    if [ ! -e $targetFile ]; then
        echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>" > $targetFile
	echo "<!DOCTYPE TS>" >> $targetFile
        echo "<TS version=\"2.1\" language=\"$i\"></TS>" >> $targetFile
    fi
done
lupdate *.cpp *.h *.ui -ts translations/*
