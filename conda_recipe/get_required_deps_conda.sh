#!/bin/bash

# This script downloads/installs the most recent conda pkg from the $GNSSTK_COMMIT (defaults to main) branch of gnsstk repo.

if [ -z $CI_API_TOKEN ]; then
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
fi

CONDA_JOB=${1:-package_conda}
PKG_TYPE=$2 #If set to pkg_min then only install required dependencies and not optional libs.
            #If set to pkg_max then install both required and optional libs

export GL_API_TOKEN=$CI_API_TOKEN
python3 -m pip install --upgrade pip
python3 -m pip install tks-ci-tools --extra-index-url https://sgl-artifactory.arlut.utexas.edu/artifactory/api/pypi/pypi-ci/simple/
download_package_by_type.sh "${CONDA_JOB}" ${PKG_TYPE}
mv packages/conda-bld ./conda-bld
exit 0
