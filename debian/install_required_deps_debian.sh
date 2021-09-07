#!/bin/bash

# This script downloads/installs the most recent debian_X pkg from the $GNSSTK_COMMIT (defaults to main) branch of gnsstk repo.

if [ -z $CI_API_TOKEN ]; then
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
fi

DEBIAN_JOB=${1:-package_debian_9}
PKG_TYPE=$2 #If set to pkg_min then only install required dependencies and not optional libs.
            #If set to pkg_max then install both required and optional libs

git clone --depth 1 https://${CI_USER}:${CI_TOKEN}@${REPO_HOST}/sgl-tks/repo_utils.git
export GL_API_TOKEN=$CI_API_TOKEN
./repo_utils/download_package_by_type.sh ${DEBIAN_JOB} ${PKG_TYPE}
sudo dpkg -i packages/debs/*.deb

exit 0
