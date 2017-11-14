#!/bin/sh

if [ "$1" = "" ]
then
	echo $0 modelname
	exit 1
fi

MODEL_NAME=$1
LANG_DIR=UI/Netgear/www.multi_v2.3_genie/langs
MAKE_STRINGTAB=image/tools/src/make_stringTab/make_stringTab
IMAGE_DIR=image/$1
USE_JEFFS2_FS=$2
if [ "$USE_JEFFS2_FS" = "1" ]; then
HOSTTOOLS_DIR=./../bcm963xx/hostTools
FLASH_NAND_BLOCK_SIZE=$3
fi

LANGS="ENU PTB DEU FRA NLD SVE RUS PLK DAN CHS JPN KOR ITA ESP CSY ELL ROM TUR"


# Generate GUI language file.
for LANG in $LANGS
do
	echo "Make String table " $LANG".pkg"
#execute command 'sudo apt-get install libxml2-utils' install 'xmllint' tool for do xml file check.
	xmllint --noout $LANG_DIR/$LANG/ui.xml || exit 1
	xmllint --noout $LANG_DIR/$LANG/msg.xml || exit 1

	$MAKE_STRINGTAB $IMAGE_DIR/$LANG.pkg $IMAGE_DIR/${MODEL_NAME}_lang.pid $LANG $LANG_DIR/$LANG/ui.xml $LANG_DIR/$LANG/msg.xml $LANG_DIR/$LANG/hlp.js

if [ "$USE_JEFFS2_FS" = "1" ]; then
	rm -rf $IMAGE_DIR/ONE_LANG
        mkdir $IMAGE_DIR/ONE_LANG
	cp -af $IMAGE_DIR/$LANG.pkg $IMAGE_DIR/ONE_LANG/lang
	$HOSTTOOLS_DIR/mkfs.jffs2 -l -p -n -e $FLASH_NAND_BLOCK_SIZE -r $IMAGE_DIR/ONE_LANG -o $IMAGE_DIR/langpkg_$LANG.img || exit 1
fi

done
