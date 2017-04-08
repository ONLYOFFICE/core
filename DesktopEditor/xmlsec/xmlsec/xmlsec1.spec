Summary: Library providing support for "XML Signature" and "XML Encryption" standards
Name: xmlsec1
Version: 1.2.23
Release: 1
License: MIT
Group: Development/Libraries
Vendor: Aleksey Sanin <aleksey@aleksey.com>
Distribution:  Aleksey Sanin <aleksey@aleksey.com>
Packager: Aleksey Sanin <aleksey@aleksey.com>
Source: ftp://ftp.aleksey.com/pub/xmlsec/releases/xmlsec1-%{version}.tar.gz
BuildRoot: %{_tmppath}/xmlsec1-%{version}-root
URL: http://www.aleksey.com/xmlsec
Requires: libxml2 >= 2.7.4
Requires: libxslt >= 1.0.20
Requires: libtool-ltdl
BuildRequires: libxml2-devel >= 2.7.4
BuildRequires: libxslt-devel >= 1.0.20
Prefix: %{_prefix}
Docdir: %{_docdir}

%define _unpackaged_files_terminate_build  0 
%define _missing_doc_files_terminate_build 0

%description
XML Security Library is a C library based on LibXML2  and OpenSSL. 
The library was created with a goal to support major XML security 
standards "XML Digital Signature" and "XML Encryption". 

%package devel 
Summary: Libraries, includes, etc. to develop applications with XML Digital Signatures and XML Encryption support.
Group: Development/Libraries 
Requires: xmlsec1 = %{version}
Requires: libxml2-devel >= 2.7.4
Requires: libxslt-devel >= 1.0.20
Requires: openssl-devel >= 0.9.8
Requires: zlib-devel
Requires: libtool-ltdl-devel

%description devel
Libraries, includes, etc. you can use to develop applications with XML Digital 
Signatures and XML Encryption support.

%package openssl
Summary: OpenSSL crypto plugin for XML Security Library
Group: Development/Libraries 
Requires: xmlsec1 = %{version}
Requires: libxml2 >= 2.7.4
Requires: libxslt >= 1.0.20
Requires: openssl >= 0.9.8
BuildRequires: openssl-devel >= 0.9.8

%description openssl
OpenSSL plugin for XML Security Library provides OpenSSL based crypto services
for the xmlsec library

%package openssl-devel
Summary: OpenSSL crypto plugin for XML Security Library
Group: Development/Libraries 
Requires: xmlsec1 = %{version}
Requires: xmlsec1-devel = %{version}
Requires: xmlsec1-openssl = %{version}
Requires: libxml2-devel >= 2.7.4
Requires: libxslt-devel >= 1.0.20
Requires: openssl >= 0.9.8
Requires: openssl-devel >= 0.9.8

%description openssl-devel
Libraries, includes, etc. for developing XML Security applications with OpenSSL

%package nss
Summary: NSS crypto plugin for XML Security Library
Group: Development/Libraries 
Requires: xmlsec1 = %{version}
Requires: libxml2 >= 2.7.4
Requires: libxslt >= 1.0.20
Requires: mozilla-nss >= 1.4
BuildRequires: mozilla-nss-devel >= 1.4

%description nss
NSS plugin for XML Security Library provides NSS based crypto services
for the xmlsec library

%package nss-devel
Summary: NSS crypto plugin for XML Security Library
Group: Development/Libraries 
Requires: xmlsec1 = %{version}
Requires: xmlsec1-devel = %{version}
Requires: xmlsec1-nss = %{version}
Requires: libxml2-devel >= 2.7.4
Requires: libxslt-devel >= 1.0.20
Requires: mozilla-nss-devel >= 1.4

%description nss-devel
Libraries, includes, etc. for developing XML Security applications with NSS

%prep
%setup -q

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%else
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%endif
else
%ifarch alpha
  CFLAGS="$RPM_OPT_FLAGS" ./configure --host=alpha-redhat-linux --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix --sysconfdir="/etc" --mandir=%{_mandir}
%endif
fi
if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/include/xmlsec1
mkdir -p $RPM_BUILD_ROOT/usr/lib
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
make prefix=$RPM_BUILD_ROOT%{prefix} mandir=$RPM_BUILD_ROOT%{_mandir} install

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files 
%defattr(-, root, root)

%doc AUTHORS ChangeLog NEWS README Copyright
%doc %{_mandir}/man1/xmlsec1.1*

%{prefix}/lib/libxmlsec1.so.*
%{prefix}/lib/libxmlsec1.so
%{prefix}/bin/xmlsec1

%files devel
%defattr(-, root, root)  

%{prefix}/bin/xmlsec1-config
%{prefix}/include/xmlsec1/xmlsec/*.h
%{prefix}/include/xmlsec1/xmlsec/private/*.h
%{prefix}/lib/libxmlsec1.*a
%{prefix}/lib/pkgconfig/xmlsec1.pc
%{prefix}/lib/xmlsec1Conf.sh
%{prefix}/share/doc/xmlsec1/* 
%{prefix}/share/aclocal/xmlsec1.m4
%doc AUTHORS HACKING ChangeLog NEWS README Copyright
%doc %{_mandir}/man1/xmlsec1-config.1*

%files openssl
%defattr(-, root, root)  

%{prefix}/lib/libxmlsec1-openssl.so.*
%{prefix}/lib/libxmlsec1-openssl.so

%files openssl-devel
%defattr(-, root, root)  

%{prefix}/include/xmlsec1/xmlsec/openssl/*.h
%{prefix}/lib/libxmlsec1-openssl.*a
%{prefix}/lib/pkgconfig/xmlsec1-openssl.pc

%files nss
%defattr(-, root, root)  

%{prefix}/lib/libxmlsec1-nss.so.*
%{prefix}/lib/libxmlsec1-nss.so

%files nss-devel
%defattr(-, root, root)  

%{prefix}/include/xmlsec1/xmlsec/nss/*.h
%{prefix}/lib/libxmlsec1-nss.*a
%{prefix}/lib/pkgconfig/xmlsec1-nss.pc

%changelog
