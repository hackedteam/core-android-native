#!/bin/bash

error=0
bold=`tput bold`
normal=`tput sgr0`
red='\e[0;31m'
green='\e[0;32m'
yellow='\e[1;33m'
NC='\e[0m' # No Color

##### CLEAN ALL #####
rm -rf legacy_native/obj
rm -rf selinux_native/obj

rm legacy_native/libs/armeabi/*
rm selinux_native/libs/armeabi/*

rm bin/local/*
rm bin/remote/*
rm bin/shared_lib/*
################################

sdk=$(which ndk-build)
cp $(pwd)/selinux_native/jni/Makefiles/Application.mk $(pwd)/selinux_native/jni/Application.mk


###########################################
############ BUILDING LEGACY ##############
###########################################

echo -e "${green}${bold}\n\nBUILDING LEGACY NATIVE${normal}${NC}\n\n"
sleep 1

$sdk -C $(pwd)/legacy_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during building!${normal}${NC}\n\n"
    error=1
fi


cp $(pwd)/legacy_native/libs/armeabi/expl_check $(pwd)/bin/local
cp $(pwd)/legacy_native/libs/armeabi/local_exploit $(pwd)/bin/local
cp $(pwd)/legacy_native/libs/armeabi/suidext $(pwd)/bin/local

###########################################
############ BUILDING SELINUX #############
###########################################

sleep 1
echo -e "\n\n${green}${bold}BUILDING SELINUX NATIVE${normal}${NC}\n\n"
sleep 1

# Build runner
cp $(pwd)/selinux_native/jni/Makefiles/runner.mk $(pwd)/selinux_native/jni/Android.mk

echo -e "${yellow}Compiling runner...\n${NC}"
$sdk -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi

# Update runner binary
perl -e 'print "\x1d\x30\x25\xd9\x28\x70\xf4\xe1\xe6\x53\x68\x78\x07\x3e\xc4\x78"' >> $(pwd)/selinux_native/libs/armeabi/runner

echo -e "${yellow}Generating runner header...\n${NC}"
$(pwd)/selinux_native/jni/gen_runner.py $(pwd)/selinux_native/libs/armeabi/runner $(pwd)/selinux_native/jni/headers/runner_bin.h

perl -e 'print "#define RUNNER_ID \"\\x1d\\x30\\x25\\xd9\\x28\\x70\\xf4\\xe1\\xe6\\x53\\x68\\x78\\x07\\x3e\\xc4\\x78\"\n\n"' >> $(pwd)/selinux_native/jni/headers/runner_bin.h

echo -e "${yellow}Generating selinux_suidext...\n${NC}"
cp $(pwd)/selinux_native/jni/Makefiles/suidext.mk $(pwd)/selinux_native/jni/Android.mk
$sdk -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi


$(pwd)/selinux_native/jni/gen_bin.py $(pwd)/selinux_native/libs/armeabi/selinux_suidext $(pwd)/selinux_native/jni/headers/bin_suidext.h binsuidext
$(pwd)/selinux_native/jni/gen_bin.py $(pwd)/legacy_native/libs/armeabi/suidext $(pwd)/selinux_native/jni/headers/bin_oldsuidext.h binoldsuidext

##### BUILD E COPY REMOTE VECTOR #####

sleep 1
echo -e "\n\n${yellow}${bold}BUILDING BINARIES FOR REMOTE VECTOR${normal}${NC}\n\n"
sleep 1

cp $(pwd)/selinux_native/jni/Makefiles/selinux_remote.mk $(pwd)/selinux_native/jni/Android.mk
$sdk -B -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi

# Copy remote vector binaries
cp $(pwd)/selinux_native/libs/armeabi/selinux_exploit $(pwd)/bin/remote

##### BUILD E COPY SHARED LIBRARIES #####

sleep 1
echo -e "\n\n${yellow}${bold}BUILDING EXPLOITATION SHARED LIBRARY${normal}${NC}\n\n"
sleep 1

$(pwd)/selinux_native/jni/gen_bin.py $(pwd)/bin/local/local_exploit $(pwd)/selinux_native/jni/headers/bin_legacyexp.h bin_legacy_get_root

cp $(pwd)/selinux_native/jni/Makefiles/selinux_shared_lib.mk $(pwd)/selinux_native/jni/Android.mk
$sdk -B -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi

# Copy shared_lib vector binaries
cp $(pwd)/selinux_native/libs/armeabi/libchecksum.so $(pwd)/bin/shared_lib/libexploit.so


sleep 1
echo -e "\n\n${yellow}${bold}BUILDING INSTALLER SHARED LIBRARY${normal}${NC}\n\n"
sleep 1

cp $(pwd)/selinux_native/jni/Makefiles/selinux_shared_lib_installer.mk $(pwd)/selinux_native/jni/Android.mk
$sdk -B -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi

# Copy remote vector binaries
cp $(pwd)/selinux_native/libs/armeabi/libchecksum.so $(pwd)/bin/shared_lib/libinstaller.so


##### BUILD E COPY LOCAL VECTOR #####

sleep 1
echo -e "\n\n${yellow}${bold}BUILDING BINARIES FOR LOCAL VECTOR${normal}${NC}\n\n"
sleep 1

cp $(pwd)/selinux_native/jni/Makefiles/selinux_local.mk $(pwd)/selinux_native/jni/Android.mk
$sdk -B -C $(pwd)/selinux_native/jni/
if [ $? != 0 ]; then
    echo -e "\n\n${red}${bold}ERROR: Something wrong during compilation${normal}${NC}\n\n"
    error=1
fi

cp $(pwd)/selinux_native/libs/armeabi/selinux_check $(pwd)/bin/local
cp $(pwd)/selinux_native/libs/armeabi/selinux_exploit $(pwd)/bin/local
cp $(pwd)/selinux_native/libs/armeabi/selinux_suidext $(pwd)/bin/local
cp $(pwd)/selinux_native/libs/armeabi/selinux4_exploit $(pwd)/bin/local
cp $(pwd)/selinux_native/libs/armeabi/selinux4_check $(pwd)/bin/local

rm $(pwd)/selinux_native/jni/Android.mk
rm $(pwd)/selinux_native/jni/Application.mk

if [ $error == 0 ]; then
    echo -e "\n\n${green}${bold}Build completed. All succesfully done!${normal}${NC}\n\n"
else
    echo -e "\n\n${red}${bold}ERROR: Build failed!${normal}${NC}\n\n"
fi
