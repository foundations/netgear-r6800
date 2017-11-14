%define name reaim
%define version 0.8
%define release 1mdk
%define __spec_install_post %{_libdir}/rpm/brp-compress
%define initrddir %([ -d /etc/init.d -a ! -L /etc/init.d ] && echo /etc/init.d || echo /etc/rc.d/init.d)

Name: %{name} 
Summary: Allows DCC through AIM and MSN clients behind a NAT
Version: %{version} 
Release: %{release}
Source: http://prdownloads.sourceforge.net/reaim/reaim-%{version}.tar.gz
URL: http://reaim.sourceforge.net/
Group: Applications/Internet
BuildRoot: %{_tmppath}/%{name}-buildroot
License: GNU General Public License
Requires: shadow-utils chkconfig fileutils

%description
ReAIM is a compact transparent proxy designed to be run on or behind a Linux IPTables based firewall. 
When run behind a simple address-translating firewall, the current AIM and MSN client software does not 
allow direct-connections between users to work. AIM requires direct-connections for file and image transfers.

This proxy is designed to transparently intercept AIM and/or MSN client messages and pass them through a configurable 
filter/rewrite chain. This allows selective modification of the source address provided in direct-connection 
setup packets, and so remote users can be told to use the global internet routable address, rather than the 
actual IP address of the client on the local network.

%prep
%setup


%build
%make


%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

mkdir -p $RPM_BUILD_ROOT%{_sysconfdir}/reaim
mkdir -p $RPM_BUILD_ROOT%{_sbindir}
mkdir -p $RPM_BUILD_ROOT%{_sysconfdir}/logrotate.d
mkdir -p $RPM_BUILD_ROOT%{initrddir}
mkdir -p $RPM_BUILD_ROOT%{_mandir}/man8

cp $RPM_BUILD_DIR/%name-%version/reaim $RPM_BUILD_ROOT%{_sbindir}/reaim
cp $RPM_BUILD_DIR/%name-%version/firewall.sh $RPM_BUILD_ROOT%{_sysconfdir}/reaim/firewall.sh
cp $RPM_BUILD_DIR/%name-%version/redhat/etc/init.d/reaim $RPM_BUILD_ROOT%{initrddir}/reaim
cp $RPM_BUILD_DIR/%name-%version/redhat/etc/logrotate.d/reaim $RPM_BUILD_ROOT%{_sysconfdir}/logrotate.d/reaim
cp $RPM_BUILD_DIR/%name-%version/reaim.8 $RPM_BUILD_ROOT%{_mandir}/man8/reaim.8
cp -a html/ $RPM_BUILD_ROOT%{_sysconfdir}/reaim/html

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
[ "$RPM_BUILD_DIR" != "/" ] && rm -rf $RPM_BUILD_DIR/%name-%version

%files
%defattr(-, root, root)
%attr(755,root,root) %{_sbindir}/reaim
%attr(744,root,root) %{_sysconfdir}/reaim/firewall.sh
%attr(744,root,root) %{initrddir}/reaim
%attr(644,root,root) %{_sysconfdir}/logrotate.d/reaim
%attr(644,root,root) %{_mandir}/man8/reaim.8.bz2

%docdir %{_sysconfdir}/reaim/html
%{_sysconfdir}/reaim/html

%post
if [ "$1" = "1" ]; then
useradd -c "ReAim Proxy Server" -d / -s /sbin/nologin reaim
touch /var/log/reaim.log
chmod 640 /var/log/reaim.log
chown reaim.adm /var/log/reaim.log
chkconfig --add reaim
%{initrddir}/reaim start
fi

echo ""
echo "*******************************************************************************************************************"
echo "* ReAIM setup is NOT COMPLETE! In order for ReAIM to work you must manually set up the appropriate firewall rules *"
echo "* Please read the information in the man page as well as the html documentation located in /etc/reaim/html        *"
echo "* For your convenience, a sample firewall script has been provided to you located in /etc/reaim                   *" 
echo "*******************************************************************************************************************"
echo ""

%preun
%{initrddir}/reaim condstop
chkconfig --del reaim

%postun
if [ "$1" = "0" ]; then
rm -f /var/log/reaim.log
userdel reaim
fi

%changelog
* Wed Mar 26 2003 Sean Donner <siegex@maximus.homelinux.net> 0.7-1mdk
- Spec file finally completed
