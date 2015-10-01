while echo $1 | grep -q ^-; do
    eval $( echo $1 | sed 's/^-//' )=$2
    shift
    shift
done

echo computer = $computer
echo user = $user
echo pass = $pass

name=FANSHEN
mount_point=/Volumes/$name
syncfrom=/Users/chris/projects/invisibleTreasure/apps/invisibleTreasure/show/bin
syncto=/Volumes/$name/Documents/fanshen

# remote mount address
mount_address=afp://mistress:password@mistress.local/mistress


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