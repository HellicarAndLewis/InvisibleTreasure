while echo $1 | grep -q ^-; do
    eval $( echo $1 | sed 's/^-//' )=$2
    shift
    shift
done

echo computer = $computer
echo user = $user
echo pass = $pass

ssh $user@$computer.local
sudo shutdown -r now