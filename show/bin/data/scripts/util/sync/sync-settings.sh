while echo $1 | grep -q ^-; do
    eval $( echo $1 | sed 's/^-//' )=$2
    shift
    shift
done

echo computer = $computer
echo user = $user
echo pass = $pass

#change to working directory
dir=${0%/*}
if [ -d "$dir" ]; then
  cd "$dir"
fi

name=FANSHEN
mount_point=/Volumes/$name
syncfrom=../../../../../bin
syncto=/Volumes/$name/Documents/fanshen

# remote mount address
mount_address=afp://$user:$pass@$computer.local/$computer


if [ "$test" != "$mount_point" ]
then
mkdir $mount_point
echo "mounting $mount_point"
mount_afp -i $mount_address $mount_point
else
echo "$mount_point is already mounted"
fi

mkdir $syncto
rsync --verbose  --progress --stats --compress --recursive --times --perms --links --delete --exclude "*.txt" $syncfrom $syncto

umount $mount_point