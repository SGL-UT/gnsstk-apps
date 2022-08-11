#!/bin/bash

# This script downloads/installs the most recent redhat_X rpm from the $GNSSTK_COMMIT (defaults to main) branch of gnsstk repo.

if [ -z $CI_API_TOKEN ]; then
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
fi

PACKAGE_JOB=${1:-package_redhat}
PKG_TYPE=$2 #If set to pkg_min then only install required dependencies and not optional libs.
            #If set to pkg_max then install both required and optional libs

export GL_API_TOKEN=$CI_API_TOKEN
sudo pip3 install tks-ci-tools
download_package_by_type.sh ${PACKAGE_JOB} ${PKG_TYPE}
sudo yum install packages/rpm_files/RPMS/x86_64/*.rpm -y
exit 0
