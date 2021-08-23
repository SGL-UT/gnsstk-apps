#!/bin/bash

# This script downloads the most recent RPM files from the main branch of gpstk.

if [ -z $CI_API_TOKEN ]; then
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
fi

CONDA_JOB=${1:-package_conda}

git clone --depth 1 https://${CI_USER}:${CI_TOKEN}@${REPO_HOST}/sgl-tks/repo_utils.git
export GL_API_TOKEN=$CI_API_TOKEN
./repo_utils/download_package_by_type.sh package_conda
mv packages/conda-bld ./conda-bld
exit 0
