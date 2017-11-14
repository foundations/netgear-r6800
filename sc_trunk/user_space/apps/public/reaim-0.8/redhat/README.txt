RedHat Installation
===================

Please note that because this is a network service, requiring modifications
to your firewall rules, I do not provide an automated way to set this up.

1. Build reaim, and copy the executable to /usr/sbin
2. Copy the etc/init.d/reaim and etc/logrotate.d/reaim files to the appropriate
   directories on your disk.
3. Run chkconfig --add reaim
4. Run adduser -c "ReAim Proxy Server" -d / -s /sbin/nologin reaim
5. Setup your firewall redirections.
6. Touch /var/log/reaim.log, and chown it to the reaim user.
7. Run /etc/init.d/reaim start

If it doesn't work after this, see the manual page for some troubleshooting tips.
