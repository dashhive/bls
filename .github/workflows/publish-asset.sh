#!/bin/bash

# Arguments:
# 1 -> directory of repo (absolute path) i.e.: '/home/runner/work/bls/bls'
# 2 -> asset file name
# 3 -> ${{ github.repository }}
# 4 -> release ID
# 5 -> ${{ github.ref_name }}
# 6 -> ${{ secrets.PERSONAL_ACCESS_TOKEN }}
SOURCE_DIR="$1"
TARGZ="$2"
GITHUB_ACTION_REPO="$3"
GITHUB_RELEASE_ID="$4"
BRANCH="$5"
GITHUB_ACCESS_TOKEN=$(echo "$6" | tr -d '[:space:]')
GITHUB_TAG_NAME=$(echo "${BRANCH}" | tr -d '[:space:]')
GITHUB_NAME_OF_ASSET=$(echo "$TARGZ" | sed -E "s|XVERSION|${GITHUB_TAG_NAME}|")

GITHUB_USER=$(echo "${GITHUB_ACTION_REPO}" | cut -d '/' -f 1 | tr -d '[:space:]')
GITHUB_REPO=$(echo "${GITHUB_ACTION_REPO}" | cut -d '/' -f 2 | tr -d '[:space:]')
GITHUB_NAME="$GITHUB_TAG_NAME"
GITHUB_PATH_OF_ASSET=$(echo "${SOURCE_DIR}" | sed -E 's|/$||');
GITHUB_PATH_OF_ASSET="${GITHUB_PATH_OF_ASSET}/build/${TARGZ}"

echo 'Path of asset ls -al:'
ls -al $GITHUB_PATH_OF_ASSET
echo "GITHUB_TAG_NAME: '${GITHUB_TAG_NAME}'"
echo "GITHUB_USER: '${GITHUB_USER}'"
echo "GITHUB_REPO: '${GITHUB_REPO}'"
echo "GITHUB_NAME: '${GITHUB_NAME}'"
echo "GITHUB_RELEASE_ID: '${GITHUB_RELEASE_ID}'"
echo "GITHUB_NAME_OF_ASSET: '${GITHUB_NAME_OF_ASSET}'"
echo "GITHUB_PATH_OF_ASSET: '${GITHUB_PATH_OF_ASSET}'"

echo "$GITHUB_PATH_OF_ASSET" | grep -E '.zip$' 
if [[ $? -eq 0 ]]; then
	curl \
		--verbose \
			-X POST \
			-H "Accept: application/vnd.github+json" \
			-H "Authorization: Bearer ${GITHUB_ACCESS_TOKEN}" \
			-H "Content-Type: application/zip" \
			https://uploads.github.com/repos/${GITHUB_USER}/${GITHUB_REPO}/releases/${GITHUB_RELEASE_ID}/assets?name="${GITHUB_NAME_OF_ASSET}" \
			--data-binary "@${GITHUB_PATH_OF_ASSET}"
else
	curl \
		--verbose \
			-X POST \
			-H "Accept: application/vnd.github+json" \
			-H "Authorization: Bearer ${GITHUB_ACCESS_TOKEN}" \
			-H "Content-Type: application/gzip" \
			https://uploads.github.com/repos/${GITHUB_USER}/${GITHUB_REPO}/releases/${GITHUB_RELEASE_ID}/assets?name="${GITHUB_NAME_OF_ASSET}" \
			--data-binary "@${GITHUB_PATH_OF_ASSET}"
fi
