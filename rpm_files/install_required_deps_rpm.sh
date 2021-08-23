#!/bin/bash

PACKAGE_JOB=${1:-package_redhat}

git clone --depth 1 https://${CI_USER}:${CI_TOKEN}@${REPO_HOST}/sgl-tks/repo_utils.git
export GL_API_TOKEN=$CI_API_TOKEN
./repo_utils/download_package_by_type.sh ${PACKAGE_JOB} 
sudo yum install packages/rpm_files/RPMS/x86_64/*.rpm -y
exit 0
