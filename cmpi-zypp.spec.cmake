#
# spec file for package cmpi-zypp 
#
# Copyright (c) 2009 SUSE LINUX Products GmbH, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           @PACKAGE@
License:        GPL v2 or later
Group:          System/Packages
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Autoreqprov:    on
Summary:        CMPI zypp provider
Version:        @VERSION@
Release:        0
Source:         @PACKAGE@-@VERSION@.tar.bz2
#Source1:        @PACKAGE@-rpmlintrc
Prefix:         /usr
BuildRequires:  cmake
BuildRequires:  libzypp-devel sblim-cmpi-devel 
BuildRequires:  doxygen gcc-c++ gettext-devel 
BuildRequires:  sblim-sfcb cmpi-provider-register
BuildRequires:  sblim-cmpiutil-devel
Recommends:     sblim-sfcb
PreReq:         cmpi-provider-register
%define mofdir  %{_datadir}/%{name}/ 

%description
CMPI zypp provider to implement the Software Update Profile and
the Software Inventory Profile from DMTF.

Authors:
--------
    Michael Andres <ma@suse.de>
    Michael Calmer <mc@suse.de>

%prep
%setup -q

%build
mkdir build
cd build
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$CFLAGS"
cmake -DCMAKE_INSTALL_PREFIX=%{prefix} \
      -DDOC_INSTALL_DIR=%{_docdir} \
      -DLIB=%{_lib} \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_SKIP_RPATH=1 \
      ..
make %{?jobs:-j %jobs} VERBOSE=1

#make check

%install
cd build
make install DESTDIR=$RPM_BUILD_ROOT

%pre
if [ $1 -gt 1 -a -d %{mofdir} ]; then
  /usr/sbin/cmpi-provider-register -r -x -d %{mofdir}
fi

%post 
/sbin/ldconfig
/usr/sbin/cmpi-provider-register -d %{mofdir}

%postun 
/sbin/ldconfig

%preun
if [ "$1" = "0" ] ; then 
  /usr/sbin/cmpi-provider-register -r -d %{mofdir}
fi

%clean
rm -rf "$RPM_BUILD_ROOT"

%files 
%defattr(-,root,root)
%dir %{_datadir}/%{name}
%dir %{_libdir}/cmpi

%{_libdir}/cmpi
%{_datadir}/%{name}
%{_docdir}/%{name}

%changelog
