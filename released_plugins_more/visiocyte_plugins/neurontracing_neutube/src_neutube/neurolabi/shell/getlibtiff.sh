#!/bin/sh
if test $# -eq 0
then
    local_dir=`pwd`
else
    local_dir=$1
fi

package=tiff-3.8.2.tar.gz
ftp_server=ftp.remotesensing.org
remote_dir=pub/libtiff

ftp -n -v $ftp_server <<EOT
user anonymous anonymous
epsv
get $remote_dir/$package $local_dir/$package