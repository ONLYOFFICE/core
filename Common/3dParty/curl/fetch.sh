CURL_FOLDER=curl
if [ ! -d ${CURL_FOLDER} ]; then
  git clone https://github.com/curl/curl.git ${CURL_FOLDER}
fi
cd ${CURL_FOLDER}
git fetch
git checkout curl-7_54_1
cd ..
