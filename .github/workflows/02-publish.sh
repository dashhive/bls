#!/bin/bash

OWNER=wmerfalen
REPO='bls-signatures'

# Github Personal Access Token will be the first argument to this script
GITHUB_PAT="$1"

echo "Creating release for Ubuntu x86-64 20.04.05 LTS (Focal) using binary at '${BINARY}'"

curl -4 -L -X POST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${GITHUB_PAT}" \
  https://api.github.com/repos/"$OWNER"/"$REPO"/releases \
	-d '{"tag_name":"ubuntu-x86-64-20-04-05-LTS","target_commitish":"main","name":"ubuntu-x86-64-20-04-05-LTS","body":"Ubuntu x86-64 20.04.05 LTS (Focal)","draft":false,"prerelease":false,"generate_release_notes":false}'
