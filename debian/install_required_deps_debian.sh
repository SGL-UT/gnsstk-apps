#!/bin/bash

DEBIAN_JOB=${1:-package_debian_9}
git clone --depth 1 https://${CI_USER}:${CI_TOKEN}@${REPO_HOST}/sgl-tks/repo_utils.git
export GL_API_TOKEN=$CI_API_TOKEN
#./repo_utils/download_package_by_type.sh ${DEBIAN_JOB} pkg_min
./repo_utils/download_package_by_type.sh ${DEBIAN_JOB}
sudo dpkg -i packages/debs/*.deb

exit 0
