name=Diesel
mount_point=/Volumes/$name
syncfrom=/Users/chris/projects/dieselshanghai/apps/MicroMacro/slitscan/bin
syncto=/Volumes/$name/Documents/diesel/show/slitscan

# remote mount address
mount_address=afp://lx:123@Utilitarian-Slitscan-Mac.local/lx
# mount_address=afp://lx:123@Utilitarian-Micro-Mac.local/lx
# mount_address=afp://lx:123@Leather-Slitscan-Mac.local/lx
# mount_address=afp://lx:123@Denim-Micro-Mac.local/lx
# mount_address=afp://lx:123@Transitional-Slitscan1-Mac.local/lx
# mount_address=afp://lx:123@Transitional-Slitscan2-Mac.local/lx
# mount_address=afp://lx:123@Fashion-Show2-Mac.local/lx

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