#!/bin/bash

sdk=$(which ndk-build)
cp $(pwd)/shell_params.h $(pwd)/legacy_native/jni/headers/
cp $(pwd)/shell_params.h $(pwd)/selinux_native/jni/headers/

###########################################
############ BUILDING LEGACY ##############
###########################################

echo -e "\n\nBUILDING LEGACY NATIVE\n\n"
sleep 1

$sdk -C $(pwd)/legacy_native/jni/

###########################################
############ BUILDING SELINUX #############
###########################################

sleep 1
echo -e "\n\nBUILDING LEGACY NATIVE\n\n"
sleep 1

cp $(pwd)/selinux_native/jni/runner.mk $(pwd)/selinux_native/jni/Android.mk

echo -e "Compiling runner...\n"
$sdk -C $(pwd)/selinux_native/jni/

echo -e "Generating runner header...\n"
$(pwd)/selinux_native/jni/gen_runner.py $(pwd)/selinux_native/libs/armeabi/runner $(pwd)/selinux_native/jni/headers/runner_bin.h

cp $(pwd)/selinux_native/jni/put_user.mk $(pwd)/selinux_native/jni/Android.mk
echo -e "Compiling all...\n"
$sdk -C $(pwd)/selinux_native/jni/


###########################################
############ COPYING BINARY   #############
###########################################

cp $(pwd)/selinux_native/libs/armeabi/runner $(pwd)/bin/selinux
cp $(pwd)/selinux_native/libs/armeabi/selinux_check $(pwd)/bin/selinux
cp $(pwd)/selinux_native/libs/armeabi/selinux_exploit $(pwd)/bin/selinux
cp $(pwd)/selinux_native/libs/armeabi/selinux_suidext $(pwd)/bin/selinux

cp $(pwd)/legacy_native/libs/armeabi/expl_check $(pwd)/bin/legacy
cp $(pwd)/legacy_native/libs/armeabi/librunner.so $(pwd)/bin/legacy
cp $(pwd)/legacy_native/libs/armeabi/local_exploit $(pwd)/bin/legacy
cp $(pwd)/legacy_native/libs/armeabi/suidext $(pwd)/bin/legacy


