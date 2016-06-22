for i in `seq 1 10` ;
do
    sleep 3
    ./cgps > client_log/$i.txt &
done


