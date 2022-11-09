#!/bin/bash


# Arguments:
# 1 -> The ${{ github.ref_name }}
# 2 -> The ${{ secrets.PERSONAL_ACCESS_TOKEN }}
# 3 -> ${{ github.action_repository }}
#   i.e.: this will be something like 'dashhive/bls'
#   so in other words, the user/repo
# 
BRANCH="$1"
GITHUB_ACTION_REPO="$3"

GITHUB_USER=$(echo "${GITHUB_ACTION_REPO}" | cut -d '/' -f 1 | tr -d '[:space:]')
GITHUB_REPO=$(echo "${GITHUB_ACTION_REPO}" | cut -d '/' -f 2 | tr -d '[:space:]')
GITHUB_TAG_NAME=$(echo "${BRANCH}" | tr -d '[:space:]')
GITHUB_NAME="$GITHUB_TAG_NAME"
GITHUB_ACCESS_TOKEN=$(echo "$2" | tr -d '[:space:]')

echo "GITHUB_TAG_NAME: '${GITHUB_TAG_NAME}'"
echo "GITHUB_USER: '${GITHUB_USER}'"
echo "GITHUB_REPO: '${GITHUB_REPO}'"
echo "GITHUB_NAME: '${GITHUB_NAME}'"

JSON_STRING=$( jq -n -c \
	--arg github_tag_name "${GITHUB_TAG_NAME}" \
	--arg github_name "${GITHUB_NAME}" \
	'{tag_name: $github_tag_name, name: $github_name, body: "body", draft: false, prerelease: false,generate_release_notes: false}' )

echo "JSON_STRING: '$JSON_STRING'"

curl \
	--silent \
		-X POST \
	  -H "Accept: application/vnd.github+json" \
	  -H "Authorization: Bearer ${GITHUB_ACCESS_TOKEN}" \
		https://api.github.com/repos/${GITHUB_USER}/${GITHUB_REPO}/releases \
  	-d "${JSON_STRING}" | grep '"id": ' -m 1  | cut -d ':' -f 2 | cut -d ',' -f 1 | tr -d '[:space:]' > tmp

echo "release id: "
cat tmp
echo "release_id=$(cat tmp)" >> $GITHUB_OUTPUT
