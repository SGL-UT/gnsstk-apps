#!/bin/bash

# This script downloads the most recent RPM files from the main branch of gpstk.
# The project id for gpstk is "2" in the url below.
# The project id for sgltk is "7" in the url below.
# The project is for mdhtk is "2533" in the url below.
# The project id for mdhpy is "3581" in the url below.

if [ -z $CI_API_TOKEN ]; then
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
fi

REPO_URL=https://repositories.arlut.utexas.edu
TOKEN="--header=PRIVATE-TOKEN:${CI_API_TOKEN}"
BRANCH=${1:-main}
CONDA_JOB=${2:-package_conda}

echo "Downloading compiled binary artifacts from: $CONDA_JOB, $BRANCH"

wget --no-verbose -O gpstk_conda.zip $TOKEN $REPO_URL/api/v4/projects/2/jobs/artifacts/${BRANCH}/download?job=${CONDA_JOB}
if [ $? -ne 0 ];then
   echo "Failed to download artifacts."
   exit 1
fi
unzip -o gpstk_conda.zip
