name=Diesel
mount_point=/Volumes/$name
syncfrom=/Users/chris/projects/dieselshanghai/apps/MicroMacro/slitscan/bin
syncto=/Volumes/$name/Documents/diesel/show/slitscan

# remote mount address
# afp://allofus:oneofus@devmacmini._afpovertcp._tcp.local/images
# mount_address=afp://lx:123@Fashion Show2 Mac._afpovertcp._tcp.local/lx/Documents/diesel/show/slitscan/bin
mount_address=afp://hellicarlewis:hellicarlewis@Fashion-Show1-Mac.local/hellicarlewis
# lx/Documents/diesel/show/slitscan/test


if [ "$test" != "$mount_point" ]
then
mkdir $mount_point
echo "mounting $mount_point"
mount_afp -i $mount_address $mount_point
else
echo "$mount_point is already mounted"
fi

mkdir $syncto
rsync --verbose  --progress --stats --compress --recursive --times --perms --links --delete --exclude "*.xml" --exclude "*.txt" $syncfrom $syncto

umount $mount_point