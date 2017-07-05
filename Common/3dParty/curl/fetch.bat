CD /D %~dp0
SET CURL_FOLDER=curl
IF NOT EXIST %CURL_FOLDER% CALL git clone https://github.com/curl/curl.git %CURL_FOLDER%
CD %CURL_FOLDER%
CALL git fetch
CALL git checkout curl-7_54_1
CD ..