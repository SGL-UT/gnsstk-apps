#!/bin/bash

# This script downloads the most recent deb files from the main branch
# of gpstk etc.
PROJ_GPSTK=2
PROJ_SGLTK=7
PROJ_BETATK=3988
PROJ_MDHTK=2533

REPO_URL=https://repositories.arlut.utexas.edu
TOKEN="--header=PRIVATE-TOKEN:${CI_API_TOKEN}"
PACKAGE_JOB=${1:-package_redhat}
GPSTK_BRANCH=${2:-main}
SGLTK_BRANCH=${3:-main}
BETATK_BRANCH=${4:-main}
MDHTK_BRANCH=${5:-master}

echo "Downloading compiled binary artifacts from: $PY_PACKAGE_JOB, $BRANCH"
mkdir packages
pushd packages
wget --no-verbose -O gpstk_rpm.zip $TOKEN $REPO_URL/api/v4/projects/$PROJ_GPSTK/jobs/artifacts/${GPSTK_BRANCH}/download?job=${PACKAGE_JOB}
if [ $? -ne 0 ];then
   echo "Failed to download artifacts."
   exit 1
fi
wget --no-verbose -O sgltk_rpm.zip $TOKEN $REPO_URL/api/v4/projects/$PROJ_SGLTK/jobs/artifacts/${SGLTK_BRANCH}/download?job=${PACKAGE_JOB}
if [ $? -ne 0 ];then
   echo "Failed to download artifacts."
   exit 1
fi
wget --no-verbose -O betatk_rpm.zip $TOKEN $REPO_URL/api/v4/projects/$PROJ_BETATK/jobs/artifacts/${BETATK_BRANCH}/download?job=${PACKAGE_JOB}
if [ $? -ne 0 ];then
   echo "Failed to download artifacts."
   exit 1
fi
wget --no-verbose -O mdhtk_rpm.zip $TOKEN $REPO_URL/api/v4/projects/$PROJ_MDHTK/jobs/artifacts/${MDHTK_BRANCH}/download?job=${PACKAGE_JOB}
if [ $? -ne 0 ];then
   echo "Failed to download artifacts."
   exit 1
fi
unzip gpstk_rpm.zip
unzip sgltk_rpm.zip
unzip betatk_rpm.zip
unzip mdhtk_rpm.zip
echo "Installing all required packages ONLY"
sudo yum install rpm_files/RPMS/x86_64/*.rpm -y
popd
rm -rf packages
