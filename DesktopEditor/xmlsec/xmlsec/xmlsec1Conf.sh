#
# Configuration file for using the XML library in GNOME applications
#
prefix="/usr"
exec_prefix="${prefix}"
libdir="${exec_prefix}/lib"
includedir="${prefix}/include"

XMLSEC_LIBDIR="${exec_prefix}/lib"
XMLSEC_INCLUDEDIR=" -D__XMLSEC_FUNCTION__=__func__ -DXMLSEC_NO_GOST=1 -DXMLSEC_NO_GOST2012=1 -DXMLSEC_DL_LIBLTDL=1 -I${prefix}/include/xmlsec1   -I/usr/include/libxml2  -I/usr/include/libxml2   -DXMLSEC_OPENSSL_100=1 -DXMLSEC_CRYPTO_OPENSSL=1"
XMLSEC_LIBS="-L${exec_prefix}/lib -lxmlsec1-openssl -lxmlsec1 -lltdl  -lxml2  -lxslt -lxml2  -lssl -lcrypto "
MODULE_VERSION="xmlsec-1.2.23-openssl"

